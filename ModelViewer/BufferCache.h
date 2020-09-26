#pragma once

#include "BoundingBox.h"
#include "Common\DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

class BufferDescriptor
{
public:
	BufferDescriptor(shared_ptr<DeviceResources> deviceResources, wstring bufferContentType, int accessorId,
		void *sysMem, size_t byteWidth, size_t structureByteStride) :
		_deviceResources(deviceResources),
		_bufferContentType(bufferContentType),
		_accessorIdx(accessorId),
		_sysMem(sysMem),
		_byteWidth(byteWidth),
		_structureByteStride(structureByteStride)
	{
	}
	shared_ptr<DeviceResources> DevResources() { return _deviceResources; }
	const shared_ptr<DeviceResources> DevResources() const { return _deviceResources; }

	size_t Hash();

	const wstring ContentType() const { return _bufferContentType; }
	unsigned int AccessorIdx() const { return _accessorIdx; }

	void* SysMem() const { return _sysMem; }
	size_t ByteWidth() const { return _byteWidth; }
	size_t StructureByteStride() const { return _structureByteStride; }

private:
	bool _hashCalculated = false;
	shared_ptr<DeviceResources> _deviceResources;
	size_t _hash;
	unsigned int _accessorIdx;
	wstring _bufferContentType;
	void* _sysMem;
	size_t _byteWidth;
	size_t _structureByteStride;
};

template <>
struct hash<BufferDescriptor>
{
	size_t operator()(const BufferDescriptor& descriptor) const
	{
		size_t res = 0;
		hash<const wchar_t *> myHash;
		res ^= myHash(descriptor.ContentType().c_str());
		hash<unsigned int> myHash2;
		res ^= myHash2(descriptor.AccessorIdx());
		return res;
	}
};

//class BufferWrapper
//{
//public:
//	BufferWrapper(GLTF_BufferData^ data, ComPtr<ID3D11Buffer> buffer) :
//		_data(data),
//		_buffer(buffer)
//	{
//	}
//	BufferWrapper() {}
//	ComPtr<ID3D11Buffer>& Buffer() { return _buffer; }
//
//	GLTF_BufferData^ Data() { return _data; }
//
//private:
//	GLTF_BufferData ^ _data;
//	ComPtr<ID3D11Buffer> _buffer;
//};

class ID3D11BufferWrapper
{
public:
	static shared_ptr<ID3D11BufferWrapper> Create(BufferDescriptor descriptor)
	{
		auto ret = make_shared<ID3D11BufferWrapper>();

		int bindFlags = 0;
		if (descriptor.ContentType() == L"POSITION" ||
			descriptor.ContentType() == L"NORMAL" ||
			descriptor.ContentType() == L"TEXCOORD_0")
		{
			bindFlags = D3D11_BIND_VERTEX_BUFFER;
		}
		else if (descriptor.ContentType() == L"INDICES")
		{
			bindFlags = D3D11_BIND_INDEX_BUFFER;
		}
		else
		{
			/*throw new std::exception("Unknown Buffer Type");*/
			return ret;
		}

		if (descriptor.ContentType() == L"POSITION")
		{
			ret->SetBoundingBox(
				BoundingBox<float>::CreateBoundingBoxFromVertexBuffer(
				(void *)descriptor.SysMem(), descriptor.ByteWidth())
			);
		}

		// Create the buffers...
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = (void *)descriptor.SysMem();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		CD3D11_BUFFER_DESC vertexBufferDesc(static_cast<unsigned int>(descriptor.ByteWidth()), bindFlags);
		vertexBufferDesc.StructureByteStride = static_cast<unsigned int>(descriptor.StructureByteStride());

		auto device = descriptor.DevResources()->GetD3DDevice();
		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, ret->AddressOfBuffer()));
		//descriptor.DevResources()->GetD3DDeviceContext()->Flush();
		return ret;
	}

	BoundingBox<float>& GetBoundingBox() { return _bbox; }
	void SetBoundingBox(BoundingBox<float> bbox) { _bbox = bbox; }

	ComPtr<ID3D11Buffer>& Buffer() { return _buffer; }
	ID3D11Buffer **AddressOfBuffer() { return _buffer.GetAddressOf(); }

private:
	BoundingBox<float> _bbox;
	ComPtr<ID3D11Buffer> _buffer;
};

template<class TBufferWrapper>
class BufferCache
{
public:
	BufferCache() {};
	~BufferCache() {};

	shared_ptr<TBufferWrapper> FindOrCreateBuffer(BufferDescriptor descriptor)
	{
		// get the hash value...
		size_t hash = descriptor.Hash();
		map<size_t, shared_ptr<TBufferWrapper>>::iterator res = _buffers.find(hash);
		if (res != _buffers.end())
			return (*res).second;

		auto ret = TBufferWrapper::Create(descriptor);
		_buffers[hash] = ret;
		return ret;
	}

private:
	map<size_t, shared_ptr<TBufferWrapper>> _buffers;
};


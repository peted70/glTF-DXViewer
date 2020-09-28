#pragma once

#include "GraphContainerNode.h"
#include "Common\DeviceResources.h"
#include <map>
#include "NodeMaterial.h"
#include <future>
#include <experimental/resumable>
#include <pplawait.h>

#include "BoundingBox.h"
#include "ShaderCache.h"

#include "BufferCache.h"

using namespace WinRTGLTFParser;
using namespace Microsoft::WRL;
using namespace std;
using namespace DX;
using namespace DirectX;

class BufferDescriptor;

class MeshNode : public GraphContainerNode
{
public:
	MeshNode(int index);
	virtual ~MeshNode();

	void CompileAndLoadVertexShader();
	void CompileAndLoadPixelShader();

	virtual void Draw(SceneContext& context, XMMATRIX model);
	virtual void CreateDeviceDependentResources();
	virtual void Initialise(const shared_ptr<DeviceResources>& deviceResources);
	virtual void AfterLoad();

	void CreateBuffer(const BufferDescriptor& bd);
	void CreateMaterial(const MaterialDescriptor& md);

	void CreateBuffer(GLTF_BufferData^ data);
	void CreateTexture(GLTF_TextureData^ data);
	void CreateMaterial(GLTF_MaterialData^ data);
	void CreateTransform(GLTF_TransformData^ data);

private:
	class BufferWrapper
	{
	public:
		BufferWrapper(BufferDescriptor data, ComPtr<ID3D11Buffer> buffer);
		BufferWrapper();

		ComPtr<ID3D11Buffer>& Buffer() { return _buffer; }

		const BufferDescriptor& Data() { return _data; }

	private:
		BufferDescriptor _data;
		ComPtr<ID3D11Buffer> _buffer;
	};

	shared_ptr<DeviceResources> m_deviceResources;
	map<wstring, BufferWrapper> _buffers;

	ComPtr<ID3D11SamplerState> _spSampler;
	shared_ptr<NodeMaterial> _material;

	size_t	m_indexCount;

	shared_ptr<NodeMaterial> _currentMaterial;
	BoundingBox<float> _bbox;

	bool m_hasUVs = false;

	shared_ptr<VertexShaderWrapper> m_vertexShaderWrapper;
	shared_ptr<PixelShaderWrapper> m_pixelShaderWrapper;
};


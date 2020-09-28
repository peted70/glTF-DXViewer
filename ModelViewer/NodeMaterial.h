#pragma once

#include <map>

using namespace Microsoft::WRL;
using namespace std;
using namespace WinRTGLTFParser;

class TextureWrapper
{
public:
	TextureWrapper(unsigned int idx,
		unsigned int type,
		ComPtr<ID3D11Texture2D> tex,
		ComPtr<ID3D11ShaderResourceView> textureResourceView,
		ComPtr<ID3D11SamplerState> texSampler) :
		_idx(idx),
		_tex(tex),
		_textureResourceView(textureResourceView),
		_textureSampler(texSampler)
	{
		_type = type;
	}

	ComPtr<ID3D11SamplerState> GetSampler() { return _textureSampler; }
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return _textureResourceView; }
	ComPtr<ID3D11Texture2D> GetTexture() { return _tex; }
	unsigned int GetIndex() { return _idx; }
	unsigned int Type() { return _type; }

private:
	ComPtr<ID3D11SamplerState> _textureSampler;
	ComPtr<ID3D11ShaderResourceView> _textureResourceView;
	ComPtr<ID3D11Texture2D> _tex;
	unsigned int _idx;
	unsigned int _type;
};

class MaterialDescriptor
{
public:
	MaterialDescriptor(wstring name) :
		_Emissivetexture(0),
		_Normaltexture(0),
		_Occlusiontexture(0),
		_Pbrmetallicroughness_Basecolortexture(0),
			_Pbrmetallicroughness_Metallicroughnesstexture(0),
		_metallicFactor(0.0f),
		_roughnessFactor(0.0f),
		_name(name)
	{
		_emmissiveFactor[0] = _emmissiveFactor[1] = _emmissiveFactor[2] = 0.0f;
		_baseColorFactor[0] = _baseColorFactor[2] = _baseColorFactor[2] = 0.5f;
		_baseColorFactor[3] = 1.0f;
	}

	void setEmissiveTexture(unsigned int emissiveTexture) { _Emissivetexture = emissiveTexture; }
	void setNormalTexture(unsigned int normalTexture) { _Normaltexture = normalTexture; }
	void setOcclusionTexture(unsigned int occlusionTexture) { _Occlusiontexture = occlusionTexture; }
	void setPbrmetallicroughness_Basecolortexture(unsigned int pbrmetallicroughness_Basecolortexture) { _Pbrmetallicroughness_Basecolortexture = pbrmetallicroughness_Basecolortexture; }
	void setPbrmetallicroughness_Metallicroughnesstexture(unsigned int pbrmetallicroughness_Metallicroughnesstexture) { _Pbrmetallicroughness_Metallicroughnesstexture = pbrmetallicroughness_Metallicroughnesstexture; }
	void setEmissiveFactor(float r, float g, float b) { _emmissiveFactor[0] = r; _emmissiveFactor[1] = g; _emmissiveFactor[2] = b; };
	void setBaseColorFactor(float r, float g, float b, float a) { _baseColorFactor[0] = r; _baseColorFactor[1] = g; _baseColorFactor[2] = b; _baseColorFactor[3] = a; };
	void setMetallicFactor(float mf) { _metallicFactor = mf; }
	void setRoughnessFactor(float rf) { _roughnessFactor = rf; }

	unsigned int getEmissiveTexture() const { return _Emissivetexture; }
	unsigned int getNormalTexture() const { return _Normaltexture; }
	unsigned int getOcclusionTexture() const { return _Occlusiontexture; }
	unsigned int getPbrmetallicroughness_Basecolortexture() const { return _Pbrmetallicroughness_Basecolortexture; }
	unsigned int getPbrmetallicroughness_Metallicroughnesstexture() const { return _Pbrmetallicroughness_Metallicroughnesstexture; }
	const float *getEmissiveFactor() const { return &_emmissiveFactor[0]; };
	const float *getBaseColorFactor() const { return &_baseColorFactor[0]; };
	float getMetallicFactor() const { return _metallicFactor; }
	float getRoughnessFactor() const { return _roughnessFactor; }

	wstring getName() const { return _name; }

private:
	unsigned int _Emissivetexture;
	unsigned int _Normaltexture;
	unsigned int _Occlusiontexture;
	unsigned int _Pbrmetallicroughness_Basecolortexture;
	unsigned int _Pbrmetallicroughness_Metallicroughnesstexture;
	float _emmissiveFactor[3];
	float _baseColorFactor[4];
	float _metallicFactor;
	float _roughnessFactor;

	wstring _name;
};

class NodeMaterial
{
public:
	NodeMaterial();
	~NodeMaterial();

	void Initialise(const MaterialDescriptor& data);
	void AddTexture(unsigned int idx, 
					unsigned int type,
					ComPtr<ID3D11Texture2D> tex, 
					ComPtr<ID3D11ShaderResourceView> textureResourceView, 
					ComPtr<ID3D11SamplerState> texSampler);
	void AddTexture(shared_ptr<TextureWrapper> tex);

	shared_ptr<TextureWrapper> GetTexture(unsigned int idx) { return _textures[idx]; }

	shared_ptr<TextureWrapper> HasTextureId(unsigned int idx);
	bool HasTexture(unsigned int idx) { return _textures.find(idx) != _textures.end(); }
	size_t GetNumTextures() { return _textures.size(); }

	map<unsigned int, shared_ptr<TextureWrapper>>& Textures() { return _textures; }

	XMFLOAT4& BaseColourFactor() { return _baseColorFactor; }
	void SetBaseColourFactor(XMFLOAT4 bcf) { _baseColorFactor = bcf; }

	XMFLOAT3& EmissiveFactor() { return _emmissiveFactor; }

	float MetallicFactor() { return _metallicFactor; }
	float RoughnessFactor() { return _roughnessFactor; }
	
private:
	map<unsigned int, shared_ptr<TextureWrapper>> _textures;
	wstring _name;

	XMFLOAT3 _emmissiveFactor = { 0.0f, 0.0f, 0.0f };

	unsigned int _Pbrmetallicroughness_Basecolortexture = 0;
	unsigned int _Pbrmetallicroughness_Metallicroughnesstexture = 0;
	unsigned int _Normaltexture = 0;
	unsigned int _Occlusiontexture = 0;
	unsigned int _Emissivetexture = 0;

	XMFLOAT4 _baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
	float _metallicFactor = 1.0f;
	float _roughnessFactor = 1.0f;
};


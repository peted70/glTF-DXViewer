#include "pch.h"
#include "NodeMaterial.h"

NodeMaterial::NodeMaterial()
{
}

NodeMaterial::~NodeMaterial()
{
}

void NodeMaterial::Initialise(const MaterialDescriptor& data)
{
	_name = data.getName();
	_Emissivetexture = data.getEmissiveTexture();
	_Normaltexture = data.getNormalTexture();
	_Occlusiontexture = data.getOcclusionTexture();
	_Pbrmetallicroughness_Basecolortexture = data.getPbrmetallicroughness_Basecolortexture();
	_Pbrmetallicroughness_Metallicroughnesstexture = data.getPbrmetallicroughness_Metallicroughnesstexture();
	const float* ef = data.getEmissiveFactor();
	_emmissiveFactor.x = ef[0];
	_emmissiveFactor.y = ef[1];
	_emmissiveFactor.z = ef[2];
	const float* bcf = data.getBaseColorFactor();
	_baseColorFactor.x = bcf[0];
	_baseColorFactor.y = bcf[1];
	_baseColorFactor.z = bcf[2];
	_baseColorFactor.w = bcf[3];
	_metallicFactor = data.getMetallicFactor();
	_roughnessFactor = data.getRoughnessFactor();
}

shared_ptr<TextureWrapper> NodeMaterial::HasTextureId(unsigned int idx)
{
	auto res = std::find_if(_textures.begin(), _textures.end(), [idx](pair<unsigned int, shared_ptr<TextureWrapper>> tex)
	{
		return tex.second->GetIndex() == idx ? true : false;
	});
	return res != _textures.end() ? (*res).second : nullptr;
}

void NodeMaterial::AddTexture(shared_ptr<TextureWrapper> tex)
{
	_textures[tex->Type()] = tex;
}

void NodeMaterial::AddTexture(unsigned int idx, 
							  unsigned int type,	
							  ComPtr<ID3D11Texture2D> tex, 
							  ComPtr<ID3D11ShaderResourceView> textureResourceView, 
							  ComPtr<ID3D11SamplerState> texSampler)
{
	_textures[type] = make_shared<TextureWrapper>(TextureWrapper(idx, type, tex, textureResourceView, texSampler));
}

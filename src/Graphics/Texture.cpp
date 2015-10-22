#include <Graphics\Texture.hpp>

NS_BEGIN

Texture::Texture(LGUID guid):
	Resource::Resource(guid)
{}

Texture::~Texture()
{}

Sampler* Texture::GetSampler()const
{
	return sampler;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView()const
{
	return srv;
}

NS_END
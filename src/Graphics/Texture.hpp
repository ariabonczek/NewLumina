#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <Core\Common.hpp>
#include <Utility\Resources\Resource.hpp>
#include <Graphics\Sampler.hpp>

NS_BEGIN

class Texture : public Resource
{
public:
	Texture(LGUID guid);
	~Texture();

	/// <summary>
	/// Returns a pointer to the texture's sampler object
	/// </summary>
	Sampler* GetSampler()const;

	/// <summary>
	/// Returns a pointer to the shader resource view
	/// </summary>
	ID3D11ShaderResourceView* GetShaderResourceView()const;
protected:

	void CopyPixels(Image& imageData, uint8* temp, uint32_t imagesize);

	Sampler* sampler;
	ID3D11ShaderResourceView* srv;
};

NS_END

#endif
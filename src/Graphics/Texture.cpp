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

void Texture::CopyPixels(Image& imageData, uint8* temp, uint32_t imagesize)
{
	if (imageData.components == 4)
	{
		std::memcpy(temp, imageData.data, imagesize * sizeof(uint8_t));
	}
	else if (imageData.components == 3)
	{
		// foreach pixel
		for (uint i = 0; i < imageData.width * imageData.height; i++)
		{
			// copy the rgb
			for (uint j = 0; j < 3; j++)
			{
				temp[i * 4 + j] = imageData.data[i * 3 + j];
			}
			// set alpha to 1
			temp[i * 4 + 3] = 1;
		}
	}
	else if (imageData.components == 1)
	{
		// foreach pixel
		for (uint i = 0; i < imageData.width * imageData.height; i++)
		{
			// copy the rgb
			for (uint j = 0; j < 3; j++)
			{
				temp[i * 4 + j] = imageData.data[i];
			}
			// set alpha to 1
			temp[i * 4 + 3] = 1;
		}
	}

}

NS_END
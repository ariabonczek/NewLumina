#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <Core\Common.hpp>
#include <Utility\Resources\Resource.hpp>
#include <Graphics\Texture.hpp>

NS_BEGIN

class Cubemap : public Texture
{
public:
	Cubemap(LGUID guid, bool baked = false);
	~Cubemap();

	void SetTextures(Image* images, ID3D11Device* device);
private:
	bool isBaked;
};

NS_END

#endif
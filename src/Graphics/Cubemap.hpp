#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#include <Core\Common.hpp>
#include <Utility\Resources\Resource.hpp>

NS_BEGIN

class Cubemap : public Resource
{
public:
	Cubemap(LGUID guid, bool baked = false);
	~Cubemap();

	void SetTextures(Image* images, ID3D11Device* device);
private:
	bool isBaked;

	ID3D11ShaderResourceView* srv;
};

NS_END

#endif
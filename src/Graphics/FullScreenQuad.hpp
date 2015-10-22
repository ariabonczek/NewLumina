#ifndef FULL_SCREEN_QUAD_HPP
#define FULL_SCREEN_QUAD_HPP

#include <Core\Common.hpp>
#include <Graphics\Material.hpp>

NS_BEGIN

class FullScreenQuad
{
public:
	FullScreenQuad();
	~FullScreenQuad();

	void Initialize(ID3D11Device* device);
	void Destroy();

	void Render(ID3D11DeviceContext* deviceContext);
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};

NS_END

#endif
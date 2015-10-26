#ifndef RASTERIZER_STATE_HPP
#define RASTERIZER_STATE_HPP

#include <Core\Common.hpp>

NS_BEGIN

enum class FaceMode
{
	Front = 3,
	Back = 2,
	Both = 1
};

enum class FillMode
{
	Solid = 3,
	Wireframe = 2,
	Both = 1
};

class RasterizerState
{
public:
	RasterizerState(FaceMode faceMode, FillMode fillMode, ID3D11Device* device);
	~RasterizerState();

	static RasterizerState* FrontSolid;
	static RasterizerState* FrontWireframe;
	static RasterizerState* BackSolid;
	static RasterizerState* BackWireframe;
	static RasterizerState* BothSolid;
	static RasterizerState* BothWireframe;

	static void InitializeStates(ID3D11Device* device);
	static void DestroyStates();

	ID3D11RasterizerState* GetState()const;
private:
	FaceMode face;
	FillMode fill;
	ID3D11RasterizerState* state;
};

NS_END

#endif
#ifndef BLEND_STATE_HPP
#define BLEND_STATE_HPP

#include <Core\Common.hpp>

NS_BEGIN

enum class BlendType
{
	One = 2,
	Zero = 1,
	SrcAlpha = 5,
	InvSrcAlpha = 6
};

enum class BlendOp
{
	Additive = 1,
	Subtractive = 2
};

class BlendState
{
public:
	BlendState(ID3D11Device* device, bool enabledBlend = true, BlendType source = BlendType::One, 
		BlendType destination = BlendType::Zero, BlendOp operation = BlendOp::Additive,
		BlendType sourceAlpha = BlendType::One, BlendType destinationAlpha = BlendType::Zero, 
		BlendOp alphaOperation = BlendOp::Additive);
	~BlendState();

	static BlendState* None;
	static BlendState* Transparency;
	static BlendState* Additive;
	static BlendState* Subtractive;

	static void InitializeStates(ID3D11Device* device);
	static void DestroyStates();

	ID3D11BlendState* GetState()const;
private:
	ID3D11BlendState* state;
};

NS_END


#endif
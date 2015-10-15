#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include <Core\Common.hpp>

NS_BEGIN

enum class AddressType
{
	Clamp,
	Wrap
};

enum class FilterType
{
	Point,
	Linear,
	Anisotropic
};

enum class ComparisonType
{
	None,
	Less,
	LessEqual,
	Equal,
	GreaterEqual,
	Greater,
	Always
};

class Sampler
{
public:
	Sampler(ID3D11Device* device, AddressType addressType, FilterType filterType, ComparisonType comparisonType = ComparisonType::None);
	~Sampler();

	static Sampler* ClampLinear;
	static Sampler* ClampPoint;
	static Sampler* ClampAnisotropic;
	static Sampler* WrapLinear;
	static Sampler* WrapPoint;
	static Sampler* WrapAnisotropic;

	static void InitializeSamplers(ID3D11Device* device);
	static void DestroySamplers();

	ID3D11SamplerState* GetSamplerState()const;

private:
	ID3D11SamplerState* sampler;

	AddressType addressType;
	FilterType  filterType;
	ComparisonType comparisonType;
};

NS_END

#endif
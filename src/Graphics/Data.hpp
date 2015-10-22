#ifndef DATA_HPP
#define DATA_HPP

#include <Core/Common.hpp>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;


NS_BEGIN

struct Image
{
	uint32 width;
	uint32 height;
	uint32 components;
	uint8* data;
};

struct MeshVertex
{
	Vector3 position;
	Vector2 texCoord;
	Vector3 normal;
	Vector3 tangent;
};

struct MeshData
{
	std::vector<MeshVertex> vertices;
	std::vector<uint16> indices;

	void Append(MeshData& data);
};

struct Color
{
	float r, g, b, a;

	Color();
	Color(float r, float g, float b, float a);

	static Color Red;
	static Color Green;
	static Color Blue;
	static Color White;
	static Color Black;
	static Color Grey;
	static Color Gray;
};

enum class LightType
{
	Directional,
	Point,
	Spot
};

struct LightData
{
	Color color;
	Vector3 direction;
	float intensity;
	Vector3 position;
	float range;
	float spot;
private:
	float pad[3];
};

enum class ShaderType
{
	Vertex,
	Hull,
	Domain,
	Geometry,
	GeometrySO,
	Pixel,
	Compute
};

NS_END

#endif
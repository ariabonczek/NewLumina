#include <Graphics\Data.hpp>

NS_BEGIN

Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
Color Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
Color Color::Grey = Color(0.7f, 0.7f, 0.7f, 1.0f);
Color Color::Gray = Color(0.7f, 0.7f, 0.7f, 1.0f);

Color Color::Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);
Color Color::Magenta = Color(1.0f, 0.0f, 1.0f, 1.0f);
Color Color::Cyan = Color(0.0f, 1.0f, 1.0f, 1.0f);
Color Color::Purple = Color(0.5f, 0.0f, 0.5f, 1.0f);
Color Color::Orange = Color(1.0f, 0.5f, 0.0f, 1.0f);
Color Color::CornflowerBlue = Color(0.392f, 0.584f, 0.929f, 1.0f);

Color::Color()
{}

Color::Color(float r, float g, float b, float a):
	r(r), g(g), b(b), a(a)
{}

void MeshData::Append(MeshData& data)
{
	uint32 verticesOffset = vertices.size();
	uint32 indicesOffset = indices.size();

	vertices.resize(vertices.size() + data.vertices.size());
	indices.resize(indices.size() + data.indices.size());
	
	for (uint32 i = 0; i < data.vertices.size(); ++i)
	{
		vertices[verticesOffset + i] = data.vertices[i];
	}

	for (uint32 i = 0; i < data.indices.size(); ++i)
	{
		indices[indicesOffset + i] = data.indices[i];
	}
}

NS_END
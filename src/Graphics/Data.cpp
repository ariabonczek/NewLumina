#include <Graphics\Data.hpp>

NS_BEGIN

Color Color::Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
Color Color::Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
Color Color::Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
Color Color::White = Color(1.0f, 1.0f, 1.0f, 1.0f);
Color Color::Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
Color Color::Grey = Color(0.7f, 0.7f, 0.7f, 1.0f);
Color Color::Gray = Color(0.7f, 0.7f, 0.7f, 1.0f);

Color::Color()
{}

Color::Color(float r, float g, float b, float a):
	r(r), g(g), b(b), a(a)
{}

void MeshData::Append(MeshData& data)
{
	vertices.resize(vertices.size() + data.vertices.size());
	indices.resize(indices.size() + data.indices.size());

	for (std::vector<MeshVertex>::iterator i = data.vertices.begin(); i != data.vertices.end(); ++i)
	{
		vertices.push_back(*i);
	}

	for (std::vector<uint16>::iterator i = data.indices.begin(); i != data.indices.end(); ++i)
	{
		indices.push_back(*i);
	}
}

NS_END
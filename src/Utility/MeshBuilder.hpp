#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>

#define NOMINMAX
#include <algorithm>

NS_BEGIN

class MeshBuilder
{
public:
	~MeshBuilder();

	static MeshData CreatePlane(float width, float depth, uint32 n, uint32 m);
	static MeshData CreateSphere(float radius = 1.0f, uint32 numSubdivisions = 3, Color color = Color::Gray);
	static MeshData CreateCube(float size = 1.0f, Color color = Color::Gray);
	static MeshData CreateCylinder(float radius = 0.5f, float height = 1.0, uint32 axisDivisions = 20, uint32 heightDivisions = 3, Color color = Color::Gray);
	static MeshData CreateCone(float radius = 0.5f, float height = 1.0f, uint32 axisDivisions = 20, uint32 heightDivisions = 3, Color color = Color::Gray);
	static MeshData CreateTube(float outerRadius = 1.0f, float innerRadius = 0.6f, float height = 1.0f, uint32 axisSubdivisions = 20, uint32 heightDivisions = 3, Color color = Color::Gray);
	static MeshData CreateTorus(float radius = 1.0f, float sectionRadius = 0.5f, uint32 numSubdivisions = 20, Color color = Color::Gray);
private:
	MeshBuilder();

	static void CreateOctahedron(std::vector<MeshVertex>& vertices, std::vector<uint16>& indices, uint32 resolution);
	static uint32 CreateUpperStrip(uint32 steps, uint32 vTop, uint32 vBottom, uint32 t, std::vector<uint16>& indices);
	static uint32 CreateLowerStrip(uint32 steps, uint32 vTop, uint32 vBottom, uint32 t, std::vector<uint16>& indices);
	static uint32 CreateVertexLine(Vector3 from, Vector3 to, uint32 steps, uint32 v, std::vector<MeshVertex>& vertices);
	static uint32 CreateVertexLineC(Vector3 from, Vector3 to, uint32 steps, uint32 v, std::vector<MeshVertex>& vertices);
	static uint32 CreateRing(uint32 axisDivisions, uint32 heightDivisions, uint32 h, uint32 t, std::vector<uint16>& indices);

	static uint32 CreateInnerTubeRing(uint32 axisDivisions, uint32 heightDivisions, uint32 h, uint32 t, std::vector<uint16>& indices);
	static uint32 CreateOuterTubeRing(uint32 axisDivisions, uint32 heightDivisions, uint32 h, uint32 t, std::vector<uint16>& indices);
	
	static uint32 CreateTorusLoop(float theta, float radius, float sectionRadius, uint32 heightDivisions, uint32 v, std::vector<MeshVertex>& vertices);
};

NS_END

#endif
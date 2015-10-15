#include <Utility\MeshBuilder.hpp>

NS_BEGIN

MeshBuilder::~MeshBuilder()
{

}

MeshData MeshBuilder::CreatePlane(float width, float depth, uint32 n, uint32 m)
{
	MeshData data;
	n = std::max(2,(int) n);
	m = std::max(2,(int) m);

	UINT vertexCount = n * m;
	UINT faceCount = (n - 1)*(m - 1) * 2;

	float halfWidth = width * 0.5f;
	float halfDepth = depth * 0.5f;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	data.vertices.resize(n * m);
	data.indices.resize((n-1)*(m-1)*2*3);

	uint32 v = 0, t = 0;

	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = halfWidth - j * dx;
			MeshVertex cVert;
			cVert.position = Vector3(x, 0.0f, z);
			cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
			cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
			cVert.texCoord.x = j * du;
			cVert.texCoord.y = i * dv;
			data.vertices[v++] = cVert;
		}
	}

	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			data.indices[t++] = (i + 1) * n + j + 1;
			data.indices[t++] = i * n + j + 1;
			data.indices[t++] = (i + 1) * n + j;
			data.indices[t++] = (i + 1) * n + j;
			data.indices[t++] = i * n + j + 1;
			data.indices[t++] = i*n + j;
			k += 6;
		}
	}

	return data;
}

// Followed this tutorial: http://www.binpress.com/tutorial/creating-an-octahedron-sphere/162
MeshData MeshBuilder::CreateSphere(float radius, uint32 numSubdivisions, Color color)
{
	MeshData data;
	MeshVertex toAdd;

	numSubdivisions = std::max(0, std::min((int)numSubdivisions, 7));

	int resolution = 1 << numSubdivisions;
	data.vertices.resize((resolution + 1) * (resolution + 1) * 4 - (resolution * 2 - 1) * 3);
	data.indices.resize((1 << (numSubdivisions * 2 + 3)) * 3);

	CreateOctahedron(data.vertices, data.indices, resolution);

	for (uint32 i = 0; i < data.vertices.size(); ++i)
	{
		// Position and Normals
		data.vertices[i].position.Normalize();
		data.vertices[i].normal = data.vertices[i].position;

		Vector3 p = Vector3::Multiply(data.vertices[i].position, radius);
		data.vertices[i].position = p;

		// UVs
		float theta = atan2(data.vertices[i].position.x, data.vertices[i].position.z);
		float phi = asinf(data.vertices[i].position.y);

		data.vertices[i].texCoord.x = theta / PI * 2;
		if (data.vertices[i].texCoord.x < 0.0f)
			data.vertices[i].texCoord.x += 1.0f;

		data.vertices[i].texCoord.y = phi / PI + 0.5f;

		// Tangents
		data.vertices[i].tangent.x = -radius*sinf(phi)*sinf(theta);
		data.vertices[i].tangent.y = 0.0f;
		data.vertices[i].tangent.z = +radius*cosf(phi)*cosf(theta);

		data.vertices[i].tangent.Normalize();
	}

	return data;
}

void MeshBuilder::CreateOctahedron(std::vector<MeshVertex>& vertices, std::vector<uint16>& indices, uint32 resolution)
{
	uint32 v = 0, vBottom = 0, t = 0;

	Vector3 directions[4] =
	{
		-Vector3::Right,
		-Vector3::Forward,
		Vector3::Right,
		Vector3::Forward
	};

	for (uint32 i = 0; i < 4; i++)
	{
		vertices[v++].position = -Vector3::Up;
	}

	// bottom hemisphere
	for (uint32 i = 1; i <= resolution; i++)
	{
		float progress = (float)i / resolution;
		Vector3 from, to;
		vertices[v++].position = to = Vector3::Lerp(-Vector3::Up, Vector3::Forward, progress);
		for (uint32 d = 0; d < 4; d++)
		{
			from = to;
			to = Vector3::Lerp(-Vector3::Up, directions[d], progress);
			t = CreateLowerStrip(i, v, vBottom, t, indices);
			v = CreateVertexLine(from, to, i, v, vertices);
			vBottom += i > 1 ? (i - 1) : 1;
		}
		vBottom = v - 1 - i * 4;
	}

	// top hemisphere
	for (uint32 i = resolution - 1; i >= 1; i--)
	{
		float progress = (float)i / resolution;
		Vector3 from, to;
		vertices[v++].position = to = Vector3::Lerp(Vector3::Up, Vector3::Forward, progress);
		for (uint32 d = 0; d < 4; d++)
		{
			from = to;
			to = Vector3::Lerp(Vector3::Up, directions[d], progress);
			t = CreateUpperStrip(i, v, vBottom, t, indices);
			v = CreateVertexLine(from, to, i, v, vertices);
			vBottom += i + 1;
		}
		vBottom = v - 1 - i * 4;
	}

	for (uint32 i = 0; i < 4; i++)
	{
		indices[t++] = vBottom;
		indices[t++] = v;
		indices[t++] = ++vBottom;
		vertices[v++].position = Vector3::Up;
	}
}

uint32 MeshBuilder::CreateUpperStrip(uint32 steps, uint32 vTop, uint32 vBottom, uint32 t, std::vector<uint16>& indices)
{
	indices[t++] = vBottom;
	indices[t++] = vTop - 1;
	indices[t++] = ++vBottom;
	for (uint32 i = 1; i <= steps; i++)
	{
		indices[t++] = vTop - 1;
		indices[t++] = vTop;
		indices[t++] = vBottom;

		indices[t++] = vBottom;
		indices[t++] = vTop++;
		indices[t++] = ++vBottom;
	}

	return t;
}

uint32 MeshBuilder::CreateLowerStrip(uint32 steps, uint32 vTop, uint32 vBottom, uint32 t, std::vector<uint16>& indices)
{
	for (uint32 i = 1; i < steps; i++)
	{
		indices[t++] = vBottom;
		indices[t++] = vTop - 1;
		indices[t++] = vTop;

		indices[t++] = vBottom++;
		indices[t++] = vTop++;
		indices[t++] = vBottom;
	}
	indices[t++] = vBottom;
	indices[t++] = vTop - 1;
	indices[t++] = vTop;

	return t;
}

uint32 MeshBuilder::CreateVertexLine(Vector3 from, Vector3 to, uint32 steps, uint32 v, std::vector<MeshVertex>& vertices)
{
	for (uint32 i = 1; i <= steps; i++)
	{
		vertices[v++].position = Vector3::Lerp(from, to, (float)i / steps);
	}
	return v;
}

MeshData MeshBuilder::CreateCube(float size, Color color)
{
	MeshData data;
	uint32 v = 0, i = 0;

	data.vertices.resize(24);
	data.indices.resize(36);

	MeshVertex cVert;
	///
	// MeshVertex order when facing quad: bottom left, top left, top right, bottom right
	///

	//Front
	cVert.position = Vector3(-size, -size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, -size);
	cVert.normal = Vector3(0.0f, 0.0f, -1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Back
	cVert.position = Vector3(-size, -size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, size);
	cVert.normal = Vector3(0.0f, 0.0f, 1.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Left
	cVert.position = Vector3(-size, -size, size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, -size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, -size, -size);
	cVert.normal = Vector3(-1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Right
	cVert.position = Vector3(size, -size, -size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, -size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, size);
	cVert.normal = Vector3(1.0f, 0.0f, 0.0f);
	cVert.tangent = Vector3(0.0f, 0.0f, -1.0f);
	cVert.texCoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Top
	cVert.position = Vector3(-size, size, -size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, size, size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, size, -size);
	cVert.normal = Vector3(0.0f, 1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// Bottom
	cVert.position = Vector3(-size, -size, -size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 1.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(-size, -size, size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(0.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 0.0f);
	data.vertices[v++] = cVert;

	cVert.position = Vector3(size, -size, -size);
	cVert.normal = Vector3(0.0f, -1.0f, 0.0f);
	cVert.tangent = Vector3(1.0f, 0.0f, 0.0f);
	cVert.texCoord = Vector2(1.0f, 1.0f);
	data.vertices[v++] = cVert;

	// INDICES
	data.indices[i++] = (uint16)(0);
	data.indices[i++] = (uint16)(1);
	data.indices[i++] = (uint16)(3);
	data.indices[i++] = (uint16)(3);
	data.indices[i++] = (uint16)(1);
	data.indices[i++] = (uint16)(2);

	data.indices[i++] = (uint16)(4);
	data.indices[i++] = (uint16)(6);
	data.indices[i++] = (uint16)(5);
	data.indices[i++] = (uint16)(6);
	data.indices[i++] = (uint16)(4);
	data.indices[i++] = (uint16)(7);

	data.indices[i++] = (uint16)(8);
	data.indices[i++] = (uint16)(9);
	data.indices[i++] = (uint16)(10);
	data.indices[i++] = (uint16)(10);
	data.indices[i++] = (uint16)(11);
	data.indices[i++] = (uint16)(8);

	data.indices[i++] = (uint16)(12);
	data.indices[i++] = (uint16)(13);
	data.indices[i++] = (uint16)(14);
	data.indices[i++] = (uint16)(14);
	data.indices[i++] = (uint16)(15);
	data.indices[i++] = (uint16)(12);

	data.indices[i++] = (uint16)(16);
	data.indices[i++] = (uint16)(17);
	data.indices[i++] = (uint16)(18);
	data.indices[i++] = (uint16)(18);
	data.indices[i++] = (uint16)(19);
	data.indices[i++] = (uint16)(16);

	data.indices[i++] = (uint16)(20);
	data.indices[i++] = (uint16)(22);
	data.indices[i++] = (uint16)(21);
	data.indices[i++] = (uint16)(20);
	data.indices[i++] = (uint16)(23);
	data.indices[i++] = (uint16)(22);

	return data;
}

MeshData MeshBuilder::CreateCylinder(float radius, float height, uint32 axisDivisions, uint32 heightDivisions, Color color)
{
	MeshData data;

	uint32 v = 0, t = 0;
	uint32 numVerts = 2 + (axisDivisions * (heightDivisions + 1));
	uint32 numTris = 2 * axisDivisions + 2 * heightDivisions * axisDivisions;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	height *= 0.5f;

	data.vertices[v++].position = Vector3::Up * -height;

	for (uint32 i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;
		v = CreateVertexLineC(Vector3(x, -height, z), Vector3(x, height, z), heightDivisions, v, data.vertices);
	}

	data.vertices[v++].position = Vector3::Up * height;

	///
	// Indices
	///

	// bottom circle
	uint32 base = 1;
	for (uint32 i = 1; i <= axisDivisions; i++)
	{
		data.indices[t++] = 0;
		data.indices[t++] = base;
		data.indices[t++] = base + (heightDivisions + 1);

		base += heightDivisions + 1;
	}

	data.indices[t - 1] = 1;

	// rings
	for (uint32 i = 1; i <= heightDivisions ; i++)
	{
		t = CreateRing(axisDivisions, heightDivisions + 1, i, t, data.indices);
	}

	// Top circle
	for (uint32 i = 1; i <= axisDivisions; i++)
	{
		data.indices[t++] = (heightDivisions + 1) * i;
		data.indices[t++] = static_cast<uint16>(data.vertices.size()) - 1;
		data.indices[t++] = (heightDivisions + 1) * i + (heightDivisions + 1);
	}

	data.indices[t - 1] = heightDivisions + 1;

	for (uint32 i = 0; i < data.vertices.size(); i++)
	{
		data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

uint32 MeshBuilder::CreateVertexLineC(Vector3 from, Vector3 to, uint32 steps, uint32 v, std::vector<MeshVertex>& vertices)
{
	for (uint32 i = 0; i <= steps; i++)
	{
		vertices[v++].position = Vector3::Lerp(from, to, (float)i / steps);
	}
	return v;
}


MeshData MeshBuilder::CreateCone(float radius, float height, uint32 axisDivisions, uint32 heightDivisions, Color color)
{
	MeshData data;

	uint32 v = 0, t = 0;

	uint32 numVerts = 2 + (axisDivisions * heightDivisions);
	uint32 numTris = axisDivisions * ((2 * heightDivisions) - 1) + axisDivisions;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	height *= 0.5f; // centering our cone at the origion

	///
	// Vertices
	///
	// bottom
	data.vertices[v++].position = Vector3::Up * -height;

	// rings
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;
		v = CreateVertexLine(Vector3::Up * -height, Vector3(x, height, z), heightDivisions , v, data.vertices);
	}

	// top
	data.vertices[v++].position = Vector3::Up * height;


	///
	// Indices
	///
	// bottom "ring"
	uint32 nextAxis;
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		nextAxis = i * heightDivisions;
		data.indices[t++] = 0;
		data.indices[t++] = nextAxis + 1;
		data.indices[t++] = nextAxis + heightDivisions + 1;
	}
	
	// Set the last value back to one
	data.indices[t - 1] = 1;

	// Remaining "rings"
	for (uint32 i = 1; i < heightDivisions; i++)
	{
		t = CreateRing(axisDivisions, heightDivisions, i, t, data.indices);
	}

	// Top circle
	for (uint32 i = 1; i <= axisDivisions; i++)
	{
		data.indices[t++] = heightDivisions * i;
		data.indices[t++] = static_cast<uint16>(data.vertices.size()) - 1;
		data.indices[t++] = heightDivisions * i + heightDivisions;
	}

	data.indices[t - 1] = heightDivisions;

	for (uint32 i = 0; i < data.vertices.size(); i++)
	{
		// BAD
		data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

uint32 MeshBuilder::CreateRing(uint32 axisDivisions, uint32 heightDivisions, uint32 h, uint32 t, std::vector<uint16>& indices)
{
	uint32 base = h;
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		indices[t++] = base;					 // 1 6			// 0
		indices[t++] = base + 1;				 // 2 7			// 1
		indices[t++] = base + heightDivisions;	 // 6 11		// 2

		indices[t++] = base + heightDivisions; 		// 6 11		// 2
		indices[t++] = base + 1;					// 2 7		// 1
		indices[t++] = base + heightDivisions + 1;	// 7 12		// 3

		base += heightDivisions;			// 6 11
	}

	indices[t - 4] = h;
	indices[t - 3] = h;
	indices[t - 1] = h + 1;

	return t;
}

uint32 MeshBuilder::CreateInnerTubeRing(uint32 axisDivisions, uint32 heightDivisions, uint32 h, uint32 t, std::vector<uint16>& indices)
{
	uint32 base = h;
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		uint32 next = base + heightDivisions + 1;

		indices[t++] = base;	
		indices[t++] = next + 1;
		indices[t++] = base + 1;

		indices[t++] = base;
		indices[t++] = next; 	
		indices[t++] = next + 1;

		// NOTE: DIFFERENCE FROM CREATE RING IS THE PLUS ONE, CLEAN THIS UP
		base += heightDivisions + 1;			// 6 11			// 2
	}

	indices[t - 5] = h + 1;
	indices[t - 2] = h;
	indices[t - 1] = h + 1;

	return t;
}

uint32 MeshBuilder::CreateOuterTubeRing(uint32 axisDivisions, uint32 heightDivisions, uint32 h, uint32 t, std::vector<uint16>& indices)
{
	uint32 base = h;
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		uint32 next = base + heightDivisions + 1;

		indices[t++] = base;	
		indices[t++] = base + 1;
		indices[t++] = next;	

		indices[t++] = next; 	
		indices[t++] = base + 1;
		indices[t++] = next + 1;

		// NOTE: DIFFERENCE FROM CREATE RING IS THE PLUS ONE, CLEAN THIS UP
		base += heightDivisions + 1;			// 6 11			// 2
	}

	indices[t - 4] = h;
	indices[t - 3] = h;
	indices[t - 1] = h + 1;

	return t;
}

MeshData MeshBuilder::CreateTube(float outerRadius, float innerRadius, float height, uint32 axisDivisions, uint32 heightDivisions, Color color)
{
	MeshData data;

	uint32 v = 0, t = 0;
	uint32 numVerts = 4 * axisDivisions + (heightDivisions - 1) * axisDivisions * 2;
	uint32 numTris = 4 * axisDivisions + 2 * heightDivisions * axisDivisions * 2;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	height *= 0.5f;

	// Inner "Cylinder"
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * innerRadius;
		float z = sinf(theta) * innerRadius;
		v = CreateVertexLineC(Vector3(x, -height, z), Vector3(x, height, z), heightDivisions, v, data.vertices);
	}

	// Outer "Cylinder"
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		float theta = ((float)i / axisDivisions) * PI * 2;
		float x = cosf(theta) * outerRadius;
		float z = sinf(theta) * outerRadius;
		v = CreateVertexLineC(Vector3(x, -height, z), Vector3(x, height, z), heightDivisions, v, data.vertices);
	}

	///
	// Indices
	///

	// Inner "Cylinder"

	// bottom circle
	uint32 offsetToOuter = axisDivisions * (heightDivisions + 1);
	uint32 offsetToNext = heightDivisions + 1;
	uint32 base = 0;
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		uint32 next = base + offsetToNext;
		uint32 outer = base + offsetToOuter;

		// TODO: better way without branching
		if (i == axisDivisions - 1)
		{
			data.indices[t++] = base;
			data.indices[t++] = outer;
			data.indices[t++] = 0;

			data.indices[t++] = 0;
			data.indices[t++] = outer;
			data.indices[t++] = offsetToOuter;
			break;
		}
		data.indices[t++] = base;
		data.indices[t++] = outer;
		data.indices[t++] = next;
							
		data.indices[t++] = next;
		data.indices[t++] = outer;
		data.indices[t++] = outer + offsetToNext;

		base = next;
	}
	//
	// inner rings
	for (uint32 i = 0; i < heightDivisions; i++)
	{
		t = CreateInnerTubeRing(axisDivisions, heightDivisions, i, t, data.indices);
	}
	
	// outer rings
	for (uint32 i = 0; i < heightDivisions; i++)
	{
		t = CreateOuterTubeRing(axisDivisions, heightDivisions, offsetToOuter + i, t, data.indices);
	}
	
	
	base = heightDivisions;
	for (uint32 i = 0; i < axisDivisions; i++)
	{
		uint32 next = base + offsetToNext;
		uint32 outer = base + offsetToOuter;

		// TODO: better way without branching
		if (i == axisDivisions - 1)
		{
			data.indices[t++] = base;
			data.indices[t++] = heightDivisions + offsetToOuter;
			data.indices[t++] = outer;

			data.indices[t++] = base;
			data.indices[t++] = heightDivisions;
			data.indices[t++] = heightDivisions + offsetToOuter;
			break;
		}
		data.indices[t++] = base;
		data.indices[t++] = next + offsetToOuter;
		data.indices[t++] = outer;

		data.indices[t++] = base;
		data.indices[t++] = next;
		data.indices[t++] = outer + offsetToNext;

		base = next;
	}
	
	for (uint32 i = 0; i < data.vertices.size(); i++)
	{
		//data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

MeshData MeshBuilder::CreateTorus(float radius, float sectionRadius, uint32 numDivisions, Color color)
{
	MeshData data;

	uint32 v = 0, t = 0;
	uint32 numVerts = numDivisions * numDivisions;
	uint32 numTris = 2 * numDivisions * numDivisions;
	data.vertices.resize(numVerts);
	data.indices.resize(numTris * 3);

	// Vertices
	for (uint32 i = 0; i < numDivisions; ++i)
	{
		float theta = ((float)i / numDivisions) * PI * 2;
		v = CreateTorusLoop(theta, radius, sectionRadius, numDivisions, v, data.vertices);
	}

	//Indices
	for (uint32 i = 0; i < numDivisions; ++i)
	{
		uint32 base = i * numDivisions;
		for (uint32 j = 0; j < numDivisions; ++j)
		{
			uint32 next, next_prev, current_prev;

			i == numDivisions - 1 ? next = j : next = base + numDivisions;
			j == 0 ? (next_prev = next + numDivisions - 1, current_prev = base + numDivisions - 1) : (next_prev = next - 1, current_prev = base - 1);

			data.indices[t++] = base;
			data.indices[t++] = next;
			data.indices[t++] = next_prev;

			data.indices[t++] = base;
			data.indices[t++] = next_prev;
			data.indices[t++] = current_prev;
			base++;
		}
	}

	for (uint32 i = 0; i < data.vertices.size(); i++)
	{
		//data.vertices[i].normal = Vector3::Normalize(data.vertices[i].position);
	}

	return data;
}

uint32 MeshBuilder::CreateTorusLoop(float theta, float radius, float sectionRadius, uint32 heightDivisions, uint32 v, std::vector<MeshVertex>& vertices)
{
	float costheta = cosf(theta);
	float sintheta = sinf(theta);
	
	float offset = 0.0f;

	// bitwise-optimize this
	if (heightDivisions % 2 != 0)
	{
		offset = 1.0f / heightDivisions * PI;
	}
	for (uint32 i = 0; i < heightDivisions; ++i)
	{
		float phi = ((float)i / heightDivisions) * PI * 2 + offset;
		
		float cosphi = cosf(phi);
		float xy = radius + sectionRadius * cosphi;

		vertices[v++].position = Vector3(costheta * xy, sectionRadius * sinf(phi), sintheta * xy);
	}

	return v;
}

MeshBuilder::MeshBuilder()
{}

NS_END
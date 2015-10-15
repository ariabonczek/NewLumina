#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>
#include <Objects\BaseRenderer.hpp>
#include <Graphics\Mesh.hpp>
#include <Graphics\Material.hpp>

NS_BEGIN

class MeshRenderer : public BaseRenderer
{
public:
	MeshRenderer(Mesh* mesh);
	~MeshRenderer();

	void Initialize();
	void Render(ID3D11DeviceContext* deviceContext);
	void Destroy();

	bool OnAddToGameObject(GameObject* object);

	void SetMaterial(Material* material);
private:
	Mesh* p_Mesh;
	Material* p_Material;
};

NS_END

#endif
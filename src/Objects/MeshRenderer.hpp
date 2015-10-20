#ifndef MESH_RENDERER_HPP
#define MESH_RENDERER_HPP

#include <Core\Common.hpp>
#include <Objects\Component.hpp>
#include <Objects\BaseRenderer.hpp>
#include <Graphics\Mesh.hpp>
#include <Graphics\Material.hpp>

NS_BEGIN

/// <summary>
///
/// </summary>
class MeshRenderer : public BaseRenderer
{
public:
	MeshRenderer(Mesh* mesh, Material* material = nullptr);
	~MeshRenderer();

	/// <summary>
	///
	/// </summary>
	void Render(ID3D11DeviceContext* deviceContext);

	void OnEnable();
	void OnDisable();

	/// <summary>
	///
	/// </summary>
	bool OnAddToGameObject(GameObject* object);
private:
	Mesh* p_Mesh;
};

NS_END

#endif
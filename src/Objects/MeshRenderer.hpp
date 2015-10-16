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
	void Initialize();

	/// <summary>
	///
	/// </summary>
	void Update();

	/// <summary>
	///
	/// </summary>
	void Render(ID3D11DeviceContext* deviceContext);

	/// <summary>
	///
	/// </summary>
	void Destroy();

	/// <summary>
	///
	/// </summary>
	bool OnAddToGameObject(GameObject* object);

	/// <summary>
	///
	/// </summary>
	void SetMaterial(Material* material);
private:
	Mesh* p_Mesh;
	Material* p_Material;
};

NS_END

#endif
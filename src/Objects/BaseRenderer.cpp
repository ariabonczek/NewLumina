#include <Objects\BaseRenderer.hpp>

NS_BEGIN

BaseRenderer::BaseRenderer(Material* material):
	p_Material(material)
{}

BaseRenderer::~BaseRenderer()
{}

void BaseRenderer::SetMaterial(Material* material)
{
	p_Material = material;
}

Material* BaseRenderer::GetMaterial()const
{
	return p_Material;
}
NS_END
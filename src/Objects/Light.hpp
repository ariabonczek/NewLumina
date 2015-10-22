#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Core\Common.hpp>
#include <Graphics\Data.hpp>
#include <Objects\Component.hpp>

NS_BEGIN

class Light : public Component
{
public:
	Light(LightType type, Color color = Color::White, float intensity = 1.0f);
	~Light();

	void Update();

	bool OnAddToGameObject(GameObject* object);

	void SetShadowTexture(RenderTexture* texture);
	LightData data;
	LightType type;
private:
	RenderTexture* shadowTexture;
	bool hasShadows;
};

NS_END

#endif
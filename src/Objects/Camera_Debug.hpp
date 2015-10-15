#ifndef CAMERA_DEBUG_HPP
#define CAMERA_DEBUG_HPP

#include <Objects\Component.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Camera.hpp>
#include <Math\Math.hpp>

class CameraDebug : public Lumina::Component
{
public:
	CameraDebug();
	~CameraDebug();

	void Initialize();
	void Destroy();
	void Update();
	bool OnAddToGameObject(Lumina::GameObject* gameObject);
private:
	Lumina::Camera* p_Camera;
	float cameraSpeed;
};


#endif
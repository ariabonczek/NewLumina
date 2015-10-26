#include <TestGame\Camera_Debug.hpp>

#include <Objects\Transform.hpp>

CameraDebug::CameraDebug() :
	Lumina::Component(),
	cameraSpeed(15.0f)
{

}

CameraDebug::~CameraDebug()
{

}

void CameraDebug::Initialize()
{
	p_Camera = p_GameObject->GetComponent<Lumina::Camera>();
}

void CameraDebug::Destroy()
{

}

void CameraDebug::Update()
{
	Lumina::Transform* transform = p_GameObject->GetComponent<Lumina::Transform>();
	float dt = Timer::GetFrameTime() * cameraSpeed;

	if (GetAsyncKeyState('W') & 0x8000)
		transform->Translate(transform->GetForward() * dt);
	if (GetAsyncKeyState('S') & 0x8000)
		transform->Translate(-transform->GetForward() * dt);
	if (GetAsyncKeyState('A') & 0x8000)
		transform->Translate(-transform->GetRight() * dt);
	if (GetAsyncKeyState('D') & 0x8000)
		transform->Translate(transform->GetRight() * dt);
	if (GetAsyncKeyState('Q') & 0x8000)
		transform->Translate(Vector3::Up * dt);
	if (GetAsyncKeyState('E') & 0x8000)
		transform->Translate(Vector3::Up * -dt);

	dt *= 6.0f;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		transform->Rotate(Quaternion::CreateFromAxisAngle(transform->GetRight(), -dt));
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		transform->Rotate(Quaternion::CreateFromAxisAngle(Vector3::Up, -dt));
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		transform->Rotate(Quaternion::CreateFromAxisAngle(Vector3::Up, dt));
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		transform->Rotate(Quaternion::CreateFromAxisAngle(transform->GetRight(), dt));

}

bool CameraDebug::OnAddToGameObject(Lumina::GameObject* gameObject)
{
	Lumina::Component::OnAddToGameObject(gameObject);
	return true;
}
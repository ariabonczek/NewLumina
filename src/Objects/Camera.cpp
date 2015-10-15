#include <Objects\Camera.hpp>
#include <Objects\Transform.hpp>
#include <Objects\GameObject.hpp>
#include <Config.hpp>

NS_BEGIN

Camera::Camera() :
	Component()
{

}

Camera::~Camera()
{}

void Camera::Initialize()
{
	right = Vector3::Right;
	up = Vector3::Up;
	look = Vector3::Forward;

	cachedTransform = p_GameObject->GetComponent<Transform>();
	SetLens(0.25f * 3.1415926535f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

	UpdateViewMatrix();
}

void Camera::Destroy()
{}

void Camera::Update()
{
	if (dirty)
		UpdateViewMatrix();
}

Vector3 Camera::GetLook()const
{
	return look;
}
Vector3 Camera::GetRight()const
{
	return right;
}
Vector3 Camera::GetUp()const
{
	return up;
}
float Camera::GetNearZ()const
{
	return nearZ;
}
float Camera::GetFarZ()const
{
	return farZ;
}
float Camera::GetAspectRatio()const
{
	return aspectRatio;
}
float Camera::GetFovX()const
{
	return fovX;
}
float Camera::GetFovY()const
{
	return fovY;
}
float Camera::GetNearWidth()const
{
	return nearWidth;
}
float Camera::GetNearHeight()const
{
	return nearHeight;
}
float Camera::GetFarWidth()const
{
	return farWidth;
}
float Camera::GetFarHeight()const
{
	return farHeight;
}
Matrix Camera::GetView()const
{
	return view;
}
Matrix Camera::GetProj()const
{
	return projection;
}

void Camera::SetLens(float fovY, float aspect, float zNear, float zFar)
{
	this->fovY = fovY;
	this->aspectRatio = aspect;
	this->nearZ = zNear;
	this->farZ = zFar;

	nearHeight = 2.0f * nearZ * tanf(0.5f * fovY);
	farHeight = 2.0f * farZ * tanf(0.5f * fovY);

	projection = Matrix::CreatePerspectiveFov(fovY, aspect, zNear, zFar);
}

void Camera::LookAt(Vector3 target)
{
	view = Matrix::CreateLookAt(cachedTransform->GetWorldPosition(), target, Vector3::Up);
	right = view.Right();
	up = view.Up();
	look = view.Forward();
	dirty = false;
	// TODO: Change the transform rotation based on new view 
}

void Camera::UpdateViewMatrix()
{
	if (cachedTransform)
	{
		// TODO: Investigate possible optimization = check if view is changed before updating it
		// Note: Transform should have GetForward() etc, use those when they work

		right = cachedTransform->GetRight();
		up = cachedTransform->GetUp();
		look = cachedTransform->GetForward();

		Vector3 pos = cachedTransform->GetWorldPosition();

		float x = -Vector3::Dot(pos, right);
		float y = -Vector3::Dot(pos, up);
		float z = -Vector3::Dot(pos, look);

		view = Matrix::CreateLookAt(pos, pos + look, up);
	}
}

void Camera::Pitch(float angle)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(cachedTransform->GetRight(), angle);
	Matrix rotation = Matrix::CreateFromQuaternion(q);

	right = Vector3::Normalize(right * rotation);
	look = Vector3::Normalize(look * rotation);
	up = Vector3::Cross(look, right);
	dirty = true;
}

void Camera::Yaw(float angle)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(cachedTransform->GetUp(), angle);
	Matrix rotation = Matrix::CreateFromQuaternion(q);

	right = Vector3::Normalize(right * rotation);
	look = Vector3::Normalize(look * rotation);
	up = Vector3::Cross(look, right);
	dirty = true;
}

void Camera::Roll(float angle)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(cachedTransform->GetForward(), angle);
	Matrix rotation = Matrix::CreateFromQuaternion(q);

	right = Vector3::Normalize(right * rotation);
	look = Vector3::Normalize(look * rotation);
	up = Vector3::Cross(look, right);
	dirty = true;
}

void Camera::RotateX(float angle)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::Right, angle);
	Matrix rotation = Matrix::CreateFromQuaternion(q);

	right = Vector3::Normalize(right * rotation);
	look = Vector3::Normalize(look * rotation);
	up = Vector3::Cross(look, right);
	dirty = true;
}

void Camera::RotateY(float angle)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::Up, angle);
	Matrix rotation = Matrix::CreateFromQuaternion(q);

	right = Vector3::Normalize(right * rotation);
	look = Vector3::Normalize(look * rotation);
	up = Vector3::Cross(look, right);
	dirty = true;
}

void Camera::RotateZ(float angle)
{
	Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::Forward, angle);
	Matrix rotation = Matrix::CreateFromQuaternion(q);

	right = Vector3::Normalize(right * rotation);
	look = Vector3::Normalize(look * rotation);
	up = Vector3::Cross(look, right);
	dirty = true;
}

NS_END
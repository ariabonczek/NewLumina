#include <Objects/Transform.hpp>
#include <Objects\GameObject.hpp>
#include <Objects\Camera.hpp>

NS_BEGIN

Transform::Transform() :
	localPosition(0.0f, 0.0f, 0.0f),
	localRotation(0.0f, 0.0f, 0.0f, 1.0f),
	localScale(1.0f, 1.0f, 1.0f),
	eulerAngles(0.0f, 0.0f, 0.0f),
	forward(0.0f, 0.0f, 1.0f),
	up(0.0f, 1.0f, 0.0f),
	right(1.0f, 0.0f, 0.0f),
	parent(nullptr),
	dirty(false)
{
	children.reserve(3);
	worldCache = Matrix::Identity;
}

Transform::~Transform()
{}

void Transform::Initialize()
{
	UpdateWorldMatrix();
}

void Transform::Destroy()
{}

void Transform::Update()
{
	if (dirty)
		UpdateWorldMatrix();
}

void Transform::Translate(Vector3 v)
{
	localPosition = localPosition + v;
	dirty = true;
}

void Transform::Rotate(Quaternion rotation)
{
	Matrix m = Matrix::CreateFromQuaternion(rotation);

	forward = (forward * m).Normalized();
	right = (right * m).Normalized();
	up = Vector3::Cross(forward, right);

	localRotation = (rotation * localRotation);
	dirty = true;
}

void Transform::SetLocalPosition(float x, float y, float z) { localPosition = Vector3(x, y, z); dirty = true; }
void Transform::SetLocalPosition(Vector3 position) { localPosition = position; dirty = true; }
void Transform::SetLocalRotation(Quaternion rotation) { localRotation = rotation; dirty = true; }
void Transform::SetLocalScale(float x, float y, float z) { localScale = Vector3(x, y, z); dirty = true; }
void Transform::SetLocalScale(Vector3 scale) { localScale = scale; dirty = true; }

Vector3 Transform::GetLocalPosition()const { return localPosition; }
Quaternion Transform::GetLocalRotation()const { return localRotation; }
Vector3 Transform::GetLocalScale()const { return localScale; }
Vector3 Transform::GetEulerAngles()const { throw "Lumina::Transform::GetEulerAngles()const not implemented"; }

Vector3 Transform::GetWorldPosition()
{
	if (IsBatman()) return localPosition;
	else
	{
		return localPosition + parent->GetWorldPosition();
	}
}
Quaternion Transform::GetWorldRotation()
{
	if (IsBatman()) return localRotation.Normalized();
	else
	{
		return localRotation * parent->GetWorldRotation();
	}
}

Vector3 Transform::GetWorldScale()
{
	if (IsBatman()) return localScale;
	else
	{
		return localScale * parent->GetWorldScale();
	}
}

Matrix Transform::GetWorldMatrix()
{
	if (dirty)
	{
		UpdateWorldMatrix();
	}
	if (IsBatman()) return worldCache;
	else
	{
		return worldCache * parent->GetWorldMatrix();
	}
}

Matrix Transform::GetWorldInverseTranspose()
{
	return Matrix::Transpose(Matrix::Inverse(worldCache));
}

void Transform::SetParent(Transform* transform)
{
	if (parent == transform)
		return;

	// TODO: Make this safe
	parent = transform;
	parent->AddChild(this);
}

void Transform::AddChild(Transform* transform)
{
	for (std::vector<Transform*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		if (transform == *it)
		{
			return;
		}
	}

	transform->SetParent(this);
	children.push_back(transform);
}

Transform* Transform::GetParent()
{
	// I NEED AN ADULT
	return parent;
}

std::vector<Transform*> Transform::GetChildren()
{
	// I AM AN ADULT
	return children;
}

void Transform::UpdateWorldMatrix()
{
	// TODO: Optimize this: call overhead
	if (!dirty)
		return;

	Matrix r, s, t;
	Vector3 worldPosition = GetWorldPosition();
	Quaternion worldRotation = GetWorldRotation();
	Vector3 worldScale = GetWorldScale();

	r = Matrix::CreateFromQuaternion(worldRotation);
	s = Matrix::CreateScale(worldScale);
	t = Matrix::CreateTranslation(worldPosition);

	worldCache = s*r*t;

	//forward = Vector3::Normalize(Vector3::Forward * r);
	//right = Vector3::Normalize(Vector3::Right * r);
	//up = Vector3::Cross(forward, right);

	if (Camera * c = p_GameObject->GetComponent<Camera>())
	{
		c->UpdateViewMatrix();
	}

	for (uint i = 0; i < children.size(); i++)
	{
		children.at(i)->UpdateWorldMatrix();
	}
	dirty = false;
}

bool Transform::IsBatman()const
{
	if (parent)
		return false;
	return true;
}

Vector3 Transform::GetForward()const
{
	return forward;
}

Vector3 Transform::GetUp()const
{
	return up;
}

Vector3 Transform::GetRight()const
{
	return right;
}

bool Transform::OnAddToGameObject(GameObject* object)
{
	Component::OnAddToGameObject(object);
	return true;
}

NS_END
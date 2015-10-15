#include <TestGame\TestScene.hpp>
#include <Lumina.hpp>
#include <TestGame\Camera_Debug.hpp>
#include <TestGame\TransformRotator.hpp>

using namespace Lumina;

TestScene::TestScene() :
	Lumina::Scene("TestScene")
{}

TestScene::~TestScene()
{}

void TestScene::LoadAssets(ID3D11Device* device)
{
	VertexShader* vs = static_cast<VertexShader*>(ResourceManager::LoadShader(L"Shaders/DirectX/defaultVertex.cso", ShaderType::Vertex, device));
	PixelShader* ps = static_cast<PixelShader*>(ResourceManager::LoadShader(L"Shaders/DirectX/defaultPixel.cso", ShaderType::Pixel, device));
	
	Material* mat = new Material();
	mat->SetVertexShader(vs);
	mat->SetPixelShader(ps);
	mat->SetShaderVariable<Color>("tint", &Color::White, ShaderType::Pixel);

	Material* mat2 = new Material();
	mat2->SetVertexShader(vs);
	mat2->SetPixelShader(ps);
	mat2->SetShaderVariable<Color>("tint", &Color::White, ShaderType::Pixel);

	GameObject* testObject = new GameObject("Test");
	Mesh* cube = new Mesh(MeshBuilder::CreateCube(1.0f), 0, device);
	MeshRenderer* meshRenderer = new MeshRenderer(cube);
	meshRenderer->SetMaterial(mat);

	testObject->AddComponent<MeshRenderer>(meshRenderer);
	testObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	testObject->AddComponent<TransformRotator>(new TransformRotator(Vector3::Up, 0.f));
	AddObject(testObject);

	Mesh* plane = new Mesh(MeshBuilder::CreatePlane(10.0f, 10.0f, 10, 10), 1, device);
	GameObject* obj2 = new GameObject("Two");
	MeshRenderer* mr2 = new MeshRenderer(plane);
	mr2->SetMaterial(mat2);
	obj2->AddComponent<MeshRenderer>(mr2);
	obj2->GetComponent<Transform>()->SetLocalPosition(Vector3(1.0f, 0.0f, 0.0f));

	AddObject(obj2);

	GameObject* camera = new GameObject("Camera");
	camera->AddComponent<Camera>(new Camera());
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -10.0f));
	camera->AddComponent<CameraDebug>(new CameraDebug());
	SetActiveCamera(camera->GetComponent<Camera>());
	AddObject(camera);
}

void TestScene::DestroyAssets()
{
	
}
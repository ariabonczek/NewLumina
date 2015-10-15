#include <Scenes\TestScene.hpp>
#include <Lumina.hpp>
#include <Objects\Camera_Debug.hpp>

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

	GameObject* testObject = new GameObject("Test");
	Mesh* m = new Mesh(MeshBuilder::CreateCube(1.0f), 0, device);
	MeshRenderer* meshRenderer = new MeshRenderer(m);
	meshRenderer->SetMaterial(mat);
	testObject->AddComponent<MeshRenderer>(meshRenderer);
	testObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, -5.0f, 0.0f));

	GameObject* secondObject = new GameObject("Second");
	Mesh* m2 = new Mesh(MeshBuilder::CreatePlane(10.0f, 10.0f, 10, 10), 1, device);
	MeshRenderer* mr2 = new MeshRenderer(m2);
	mr2->SetMaterial(mat);
	secondObject->AddComponent<MeshRenderer>(mr2);
	secondObject->GetComponent<Transform>()->SetLocalPosition(0.0f, 0.0f, 0.0f);
	
	AddObject(testObject);
	AddObject(secondObject);

	GameObject* camera = new GameObject("Camera");
	camera->AddComponent<Camera>(new Camera());
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));
	camera->AddComponent<CameraDebug>(new CameraDebug());
	SetActiveCamera(camera->GetComponent<Camera>());
	AddObject(camera);
}

void TestScene::DestroyAssets()
{
	
}
#include <Scenes\TestScene.hpp>
#include <Lumina.hpp>

using namespace Lumina;

TestScene::TestScene() :
	Lumina::Scene("TestScene")
{}

TestScene::~TestScene()
{}

void TestScene::LoadAssets(ID3D11Device* device)
{
	VertexShader* vs = static_cast<VertexShader*>(ResourceManager::GetInstance().LoadShader(L"Shaders/DirectX/defaultVertex.cso", ShaderType::Vertex, device));
	PixelShader* ps = static_cast<PixelShader*>(ResourceManager::GetInstance().LoadShader(L"Shaders/DirectX/defaultPixel.cso", ShaderType::Pixel, device));
	Material* mat = new Material();
	mat->SetVertexShader(vs);
	mat->SetPixelShader(ps);

	GameObject* testObject = new GameObject("Test");
	Mesh* m = new Mesh(MeshBuilder::CreateCube(1.0f), 0, device);
	MeshRenderer* meshRenderer = new MeshRenderer(m);
	meshRenderer->SetMaterial(mat);
	testObject->AddComponent<MeshRenderer>(meshRenderer);
	testObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 10.0f));

	//GameObject* secondObject = new GameObject("Second");
	//MeshRenderer* m2 = new MeshRenderer(m);
	//m2->SetMaterial(mat);
	//secondObject->AddComponent<MeshRenderer>(m2);
	

	AddObject(testObject);

	//AddObject(secondObject);
	GameObject* camera = new GameObject("Camera");
	camera->AddComponent<Camera>(new Camera());
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -10.0f));

	SetActiveCamera(camera->GetComponent<Camera>());
	AddObject(camera);
}

void TestScene::DestroyAssets()
{
	
}
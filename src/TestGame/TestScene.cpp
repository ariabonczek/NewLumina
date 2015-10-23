#include <TestGame\TestScene.hpp>

#include <TestGame\Camera_Debug.hpp>
#include <TestGame\TransformRotator.hpp>

using namespace Lumina;

TestScene::TestScene() :
	Lumina::Scene("TestScene")
{}

TestScene::~TestScene()
{}

void TestScene::LoadAssets()
{
	VertexShader* vs = static_cast<VertexShader*>(ResourceManager::LoadShader(L"Shaders/DirectX/defaultVertex.cso", ShaderType::Vertex));
	PixelShader* ps = static_cast<PixelShader*>(ResourceManager::LoadShader(L"Shaders/DirectX/defaultPixel.cso", ShaderType::Pixel));
	
	VertexShader* quadVS = (VertexShader*)ResourceManager::LoadShader(L"Shaders/DirectX/fullScreenQuadVertex.cso", ShaderType::Vertex);
	PixelShader* tintPS = (PixelShader*)ResourceManager::LoadShader(L"Shaders/DirectX/postFXTint.cso", ShaderType::Pixel);

	Material* mat = new Material();
	mat->SetVertexShader(vs);
	mat->SetPixelShader(ps);
	mat->SetShaderVariable<Color>("tint", &Color::White, ShaderType::Pixel);
	mat->SetShaderVariable<float>("roughness", 1.0f, ShaderType::Pixel);
	mat->SetShaderVariable<float>("metalness", 0.04f, ShaderType::Pixel);
	mat->SetShaderVariable<uint32>("tileX", 1, ShaderType::Pixel);
	mat->SetShaderVariable<uint32>("tileY", 1, ShaderType::Pixel);
	mat->SetTexture("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_diff.jpg"), ShaderType::Pixel);
	mat->SetTexture("_Normal", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_nor.jpg"), ShaderType::Pixel);

	Material* mat2 = new Material();
	mat2->SetVertexShader(vs);
	mat2->SetPixelShader(ps);
	mat2->SetShaderVariable<Color>("tint", &Color::White, ShaderType::Pixel);
	mat2->SetShaderVariable<float>("roughness", 1.0f, ShaderType::Pixel);
	mat2->SetShaderVariable<float>("metalness", 0.04f, ShaderType::Pixel);
	mat2->SetShaderVariable<uint32>("tileX", 2, ShaderType::Pixel);
	mat2->SetShaderVariable<uint32>("tileY", 2, ShaderType::Pixel);
	mat2->SetTexture("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_diff.jpg"), ShaderType::Pixel);
	mat2->SetTexture("_Normal", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_nor.jpg"), ShaderType::Pixel);

	Material* ppMat = new Material();
	ppMat->SetVertexShader(quadVS);
	ppMat->SetPixelShader(tintPS);
	ppMat->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);

	Mesh* cube = ResourceManager::CreateCube();
	Mesh* plane = ResourceManager::CreatePlane(25.0f, 25.0f, 10, 10);

	RenderTexture* tex = ResourceManager::CreateRenderTexture();

	GameObject* testObject = new GameObject("Test");

	testObject->AddComponent<MeshRenderer>(new MeshRenderer(cube, mat));
	testObject->AddComponent<TransformRotator>(new TransformRotator(Vector3::Up, 0.f));
	testObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

	GameObject* twoObject = new GameObject("Two");

	twoObject->AddComponent<MeshRenderer>(new MeshRenderer(plane, mat2));
	twoObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, -3.0f, 0.0f));

	GameObject* camera = new GameObject("Camera");
	camera->AddComponent<Camera>(new Camera());
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -10.0f));
	camera->AddComponent<CameraDebug>(new CameraDebug());

	GameObject* light = new GameObject("Light");
	light->AddComponent<Light>(new Light(LightType::Spot, Color::White, 2.0f));
	light->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(0.0f, -1.0f, 0.0f));
	light->GetComponent<Light>()->data.range = 40.0f;
	light->GetComponent<Light>()->data.spot = 10.0f;
	light->GetComponent<Transform>()->SetLocalPosition(0.0, 10.0f, 0.0f);
	
	AddObject(testObject);
	AddObject(twoObject);
	AddObject(camera);
	AddObject(light);

	PostProcess* postProcess = new PostProcess();
	postProcess->SetMaterial(ppMat);
	AddPostProcess(postProcess);

	SetActiveCamera(camera->GetComponent<Camera>());
	SetAmbientLight(Color(0.05f, 0.05f, 0.05f, 1.0f));
}

void TestScene::DestroyAssets()
{
	
}
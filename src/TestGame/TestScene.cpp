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
	//
	// Load Resources
	//
	
	VertexShader* vs = static_cast<VertexShader*>(ResourceManager::LoadShader(L"Shaders/defaultVertex.cso", ShaderType::Vertex));
	PixelShader* ps = static_cast<PixelShader*>(ResourceManager::LoadShader(L"Shaders/defaultPixel.cso", ShaderType::Pixel));
	PixelShader* bumpedps = (PixelShader*)ResourceManager::LoadShader(L"Shaders/bumpedPixel.cso", ShaderType::Pixel);
	PixelShader* ibPS = (PixelShader*)ResourceManager::LoadShader(L"Shaders/sampleConvulvedCubemap.cso", ShaderType::Pixel);

	VertexShader* quadVS = (VertexShader*)ResourceManager::LoadShader(L"Shaders/fullScreenQuadVertex.cso", ShaderType::Vertex);
	PixelShader* tintPS = (PixelShader*)ResourceManager::LoadShader(L"Shaders/postFXTint.cso", ShaderType::Pixel);
	PixelShader* gammaPS = (PixelShader*)ResourceManager::LoadShader(L"Shaders/gammaCorrection.cso", ShaderType::Pixel);

	Mesh* cube = ResourceManager::LoadMesh("Meshes/sphere.obj");
	Mesh* plane = ResourceManager::CreatePlane(25.0f, 25.0f, 10, 10);

	RenderTexture* tex = ResourceManager::CreateRenderTexture();

	//
	// Create Materials
	//

	Material* mat = new Material();
	mat->SetVertexShader(vs);
	mat->SetPixelShader(ps);
	mat->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);
	mat->SetShaderVariable<float>("roughness", 0.1, ShaderType::Pixel);
	mat->SetShaderVariable<float>("metalness", 0.04f, ShaderType::Pixel);
	mat->SetShaderVariable<uint32>("tileX", 1, ShaderType::Pixel);
	mat->SetShaderVariable<uint32>("tileY", 1, ShaderType::Pixel);
	mat->SetTexture("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_diff.jpg"), ShaderType::Pixel);

	Material* mat3 = new Material();
	mat3->SetVertexShader(vs);
	mat3->SetPixelShader(ps);
	mat3->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);
	mat3->SetShaderVariable<float>("roughness", 0.7f, ShaderType::Pixel);
	mat3->SetShaderVariable<float>("metalness", 0.04f, ShaderType::Pixel);
	mat3->SetShaderVariable<uint32>("tileX", 1, ShaderType::Pixel);
	mat3->SetShaderVariable<uint32>("tileY", 1, ShaderType::Pixel);
	mat3->SetTexture("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_diff.jpg"), ShaderType::Pixel);

	Material* mat4 = new Material();
	mat4->SetVertexShader(vs);
	mat4->SetPixelShader(ps);
	mat4->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);
	mat4->SetShaderVariable<float>("roughness", 1.0f, ShaderType::Pixel);
	mat4->SetShaderVariable<float>("metalness", 0.04f, ShaderType::Pixel);
	mat4->SetShaderVariable<uint32>("tileX", 1, ShaderType::Pixel);
	mat4->SetShaderVariable<uint32>("tileY", 1, ShaderType::Pixel);
	mat4->SetTexture("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_diff.jpg"), ShaderType::Pixel);

	Material* mat2 = new Material();
	mat2->SetVertexShader(vs);
	mat2->SetPixelShader(bumpedps);
	mat2->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);
	mat2->SetShaderVariable<float>("roughness", 0.8f, ShaderType::Pixel);
	mat2->SetShaderVariable<float>("metalness", 0.04f, ShaderType::Pixel);
	mat2->SetShaderVariable<uint32>("tileX", 2, ShaderType::Pixel);
	mat2->SetShaderVariable<uint32>("tileY", 2, ShaderType::Pixel);
	mat2->SetTexture("_Albedo", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_diff.jpg"), ShaderType::Pixel);
	mat2->SetTexture("_Normal", "_Sampler", ResourceManager::LoadTexture2D("Textures/brick_nor.jpg"), ShaderType::Pixel);

	Material* ibMat = new Material();
	ibMat->SetVertexShader(vs);
	ibMat->SetPixelShader(ibPS);
	ibMat->SetTexture("_Cubemap", "_Sampler", ResourceManager::LoadCubemap(
		"Textures/emeraldfog/right.jpg", "Textures/emeraldfog/left.jpg", "Textures/emeraldfog/top.jpg",
		"Textures/emeraldfog/top.jpg", "Textures/emeraldfog/front.jpg", "Textures/emeraldfog/back.jpg"),ShaderType::Pixel);

	Material* ppMat = new Material();
	ppMat->SetVertexShader(quadVS);
	ppMat->SetPixelShader(tintPS);
	ppMat->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);

	Material* gammaMat = new Material();
	gammaMat->SetVertexShader(quadVS);
	gammaMat->SetPixelShader(gammaPS);


	//
	// Create GameObjects
	//

	GameObject* testObject = new GameObject("Test");

	testObject->AddComponent<MeshRenderer>(new MeshRenderer(cube, ibMat));
	testObject->AddComponent<TransformRotator>(new TransformRotator(Vector3::Up, 0.f));
	testObject->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

	//GameObject* testObject2 = new GameObject("Test");
	//
	//testObject2->AddComponent<MeshRenderer>(new MeshRenderer(cube, mat3));
	//testObject2->AddComponent<TransformRotator>(new TransformRotator(Vector3::Up, 0.f));
	//testObject2->GetComponent<Transform>()->SetLocalPosition(Vector3(2.0f, 0.0f, 0.0f));
	//
	//GameObject* testObject3 = new GameObject("Test");
	//
	//testObject3->AddComponent<MeshRenderer>(new MeshRenderer(plane, mat4));
	//testObject3->AddComponent<TransformRotator>(new TransformRotator(Vector3::Up, 0.f));
	//testObject3->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, -3.0f, 0.0f));

	GameObject* camera = new GameObject("Camera");
	camera->AddComponent<Camera>(new Camera());
	camera->AddComponent<CameraDebug>(new CameraDebug());
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -10.0f));
	camera->GetComponent<Camera>()->SetLens(0.15f * 3.1415926535f, (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 110.0f);
	camera->GetComponent<Camera>()->SetSkybox(ResourceManager::LoadCubemap(
		"Textures/emeraldfog/right.jpg", "Textures/emeraldfog/left.jpg", "Textures/emeraldfog/top.jpg",
		"Textures/emeraldfog/top.jpg", "Textures/emeraldfog/front.jpg", "Textures/emeraldfog/back.jpg"));

	GameObject* light = new GameObject("Light");
	light->AddComponent<Light>(new Light(LightType::Directional, Color::White, 1.0f));
	light->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	light->GetComponent<Light>()->data.range = 25.0f;
	light->GetComponent<Light>()->data.spot = 5.0f;
	light->GetComponent<Light>()->data.bidirectionalColor = Vector3(1.0f, 1.0f, 1.0f);
	light->GetComponent<Transform>()->SetLocalPosition(-5.0f, 0.0f, -5.0f);
	AddObject(light);

	AddObject(testObject);
	//AddObject(testObject2);
	//AddObject(testObject3);
	AddObject(camera);

	//GameObject* light2 = new GameObject("Light");
	//light2->AddComponent<Light>(new Light(LightType::Point, Color::Blue, 2.0f));
	//light2->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light2->GetComponent<Light>()->data.range = 25.0f;	
	//light2->GetComponent<Light>()->data.spot = 5.0f;
	//light2->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light2->GetComponent<Transform>()->SetLocalPosition(10.0f, 5.0f, 10.0f);
	//AddObject(light2);
	
	//GameObject* light3 = new GameObject("Light");
	//light3->AddComponent<Light>(new Light(LightType::Point, Color::Green, 2.0f));
	//light3->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light3->GetComponent<Light>()->data.range = 25.0f;
	//light3->GetComponent<Light>()->data.spot = 5.0f;
	//light3->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light3->GetComponent<Transform>()->SetLocalPosition(-10.0f, 5.0f, -10.0f);
	//AddObject(light3);
	//
	//GameObject* light4 = new GameObject("Light");
	//light4->AddComponent<Light>(new Light(LightType::Point, Color::Red, 2.0f));
	//light4->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light4->GetComponent<Light>()->data.range = 25.0f;
	//light4->GetComponent<Light>()->data.spot = 5.0f;
	//light4->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light4->GetComponent<Transform>()->SetLocalPosition(10.0f, 5.0f, -10.0f);
	//AddObject(light4);
	//
	//GameObject* light5 = new GameObject("Light");
	//light5->AddComponent<Light>(new Light(LightType::Point, Color::Purple, 2.0f));
	//light5->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light5->GetComponent<Light>()->data.range = 25.0f;
	//light5->GetComponent<Light>()->data.spot = 5.0f;
	//light5->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light5->GetComponent<Transform>()->SetLocalPosition(0.0f, 5.0f, 0.0f);
	//AddObject(light5);
	//
	//GameObject* light6 = new GameObject("Light");
	//light6->AddComponent<Light>(new Light(LightType::Point, Color::Orange, 2.0f));
	//light6->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light6->GetComponent<Light>()->data.range = 25.0f;
	//light6->GetComponent<Light>()->data.spot = 5.0f;
	//light6->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light6->GetComponent<Transform>()->SetLocalPosition(0.0f, 5.0f, 10.0f);
	//AddObject(light6);
	//
	//GameObject* light7 = new GameObject("Light");
	//light7->AddComponent<Light>(new Light(LightType::Point, Color::Yellow, 2.0f));
	//light7->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light7->GetComponent<Light>()->data.range = 25.0f;
	//light7->GetComponent<Light>()->data.spot = 5.0f;
	//light7->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light7->GetComponent<Transform>()->SetLocalPosition(0.0f, 5.0f, -10.0f);
	//AddObject(light7);
	//
	//GameObject* light8 = new GameObject("Light");
	//light8->AddComponent<Light>(new Light(LightType::Point, Color::Magenta, 2.0f));
	//light8->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light8->GetComponent<Light>()->data.range = 25.0f;
	//light8->GetComponent<Light>()->data.spot = 5.0f;
	//light8->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light8->GetComponent<Transform>()->SetLocalPosition(-10.0f, 5.0f, 0.0f);
	//AddObject(light8);
	//
	//GameObject* light9 = new GameObject("Light");
	//light9->AddComponent<Light>(new Light(LightType::Point, Color::Cyan, 2.0f));
	//light9->GetComponent<Light>()->data.direction = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	//light9->GetComponent<Light>()->data.range = 25.0f;
	//light9->GetComponent<Light>()->data.spot = 5.0f;
	//light9->GetComponent<Light>()->data.bidirectionalColor = Vector3(0.2f, 0.2f, 0.2f);
	//light9->GetComponent<Transform>()->SetLocalPosition(10.0f, 5.0f, 0.0f);
	//AddObject(light9);

	SetActiveCamera(camera->GetComponent<Camera>());
	//SetAmbientLight(Color(0.05f, 0.05f, 0.05f, 1.0f));
}

void TestScene::DestroyAssets()
{
	
}
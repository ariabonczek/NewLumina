#include <TestGame\DancingFlames.hpp>

#include <TestGame\Camera_Debug.hpp>

using namespace Lumina;

DancingFlamesScene::DancingFlamesScene() :
	Lumina::Scene("DancingFlamesScene")
{}

DancingFlamesScene::~DancingFlamesScene()
{}

void DancingFlamesScene::LoadAssets()
{
	// Setup camera
	GameObject* camera = new GameObject("Camera");
	camera->AddComponent<Camera>(new Camera());
	camera->GetComponent<Transform>()->SetLocalPosition(Vector3(0.0f, 0.0f, -10.0f));
	camera->AddComponent<CameraDebug>(new CameraDebug());
	AddObject(camera);
	SetActiveCamera(camera->GetComponent<Camera>());

	// Load Resources for GameObjects
	VertexShader* vs = static_cast<VertexShader*>(ResourceManager::LoadShader(L"Shaders/defaultVertex.cso", ShaderType::Vertex));
	PixelShader* ps = static_cast<PixelShader*>(ResourceManager::LoadShader(L"Shaders/singleColorPixel.cso", ShaderType::Pixel));

	Material* sphereMat = new Material();
	sphereMat->SetVertexShader(vs);
	sphereMat->SetPixelShader(ps);
	sphereMat->SetShaderVariable<Color>("tint", Color::White, ShaderType::Pixel);
	sphereMat->SetShaderVariable<float>("roughness", 1.0f, ShaderType::Pixel);
	sphereMat->SetShaderVariable<float>("metalness", 0.0f, ShaderType::Pixel);

	Mesh* sphere = ResourceManager::LoadMesh("Meshes/sphere.obj");
	
	// Create GameObjects
	GameObject* sphereObject = new GameObject("Sphere");
	sphereObject->AddComponent<MeshRenderer>(new MeshRenderer(sphere, sphereMat));

	AddObject(sphereObject);

	const uint NUM_FLAMES = 4;
	const float radius = 10.0f;
	// FLAMES
	for (uint i = 0; i < NUM_FLAMES; ++i)
	{
		GameObject* flame = new GameObject("flame");
		Light* light = new Light(LightType::Point, Color(0.6117f, 0.1647f, 0.0f, 1.0f), 1.0f);
		light->data.range = 15.0f;
		flame->AddComponent<Light>(light);
		flame->GetComponent<Transform>()->SetLocalPosition(cosf((float)i / NUM_FLAMES * 6.28) * radius, 5.0f, sinf((float)i / NUM_FLAMES * 6.28) * radius);
		AddObject(flame);
	}

	for (uint i = 0; i < NUM_FLAMES; ++i)
	{
		GameObject* flame = new GameObject("flame");
		Light* light = new Light(LightType::Point, Color(0.6117f, 0.1647f, 0.0f, 1.0f), 1.0f);
		light->data.range = 15.0f;
		flame->AddComponent<Light>(light);
		flame->GetComponent<Transform>()->SetLocalPosition(cosf((float)i / NUM_FLAMES * 6.28) * radius, -5.0f, sinf((float)i / NUM_FLAMES * 6.28) * radius);
		AddObject(flame);
	}
}

void DancingFlamesScene::DestroyAssets()
{

}
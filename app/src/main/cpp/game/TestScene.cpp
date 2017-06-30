#include <engine/core/matrix4f.h>
#include <utils.h>
#include <engine/rendering/vertex.h>
#include <vector>
#include <engine/core/gameobject.h>
#include <engine/components/camera.h>
#include <logwrapper.h>
#include <unordered_map>
#include <engine/components/lightsfactory.h>
#include "TestScene.h"

using namespace std;

const string TestScene::TEST_IMAGE_KEY = "test";
const string TestScene::BRICKS_IMAGE_KEY = "bricks";
const string TestScene::MONKEY_MODEL_KEY = "monkey";
const string TestScene::TAG = "TestScene";

TestScene::TestScene(const unordered_map<string, string> &paths, JoystickInput &leftJoystickInput,
					 JoystickInput &rightJoystickInput)
{
	m_testImagePath = paths.at(TEST_IMAGE_KEY);
	m_bricksImagePath = paths.at(BRICKS_IMAGE_KEY);
	m_monkeyModelPath = paths.at(MONKEY_MODEL_KEY);

	m_controller = make_shared<TestController>(leftJoystickInput, rightJoystickInput);
}

void TestScene::makeOpenGLDependentSetup()
{
	m_rootGameObject = make_shared<GameObject>();
	m_rootGameObject->setEngine(m_coreEngine);
	m_camera.reset();

	vector<Vertex> vertices;
	vector<unsigned int> indices;

	float fieldDepth = 10;
	float fieldWidth = 10;

	vertices.push_back(Vertex(Vector3f(-fieldWidth, -1, -fieldDepth), Vector2f(0, 0)));
	vertices.push_back(Vertex(Vector3f(-fieldWidth, -1, fieldDepth * 3), Vector2f(0, 1)));
	vertices.push_back(Vertex(Vector3f(fieldWidth * 3, -1, -fieldDepth), Vector2f(1, 0)));
	vertices.push_back(Vertex(Vector3f(fieldWidth * 3, -1, fieldDepth * 3), Vector2f(1, 1)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	m_landMesh = make_shared<Mesh>();
	m_landMesh->setVertices(vertices, indices, true);
	m_landMaterial = make_shared<Material>();
	m_bricksTexture.reset();
	m_bricksTexture = make_shared<Texture>(m_bricksImagePath);
	m_landMaterial->addTexture("diffuse", m_bricksTexture.get());
	m_landMaterial->addFloat("specularIntensity", 1);
	m_landMaterial->addFloat("specularPower", 8);
	m_landMeshRenderer = make_shared<MeshRenderer>(m_landMesh.get(), m_landMaterial.get());
	m_landGameObject = make_shared<GameObject>();
	m_landGameObject->addComponent(m_landMeshRenderer.get());
	m_rootGameObject->addChild(m_landGameObject.get());

	m_directionalLightGameObject = make_shared<GameObject>();
	m_directionalLight.reset();
	m_directionalLight = LightsFactory::createDirectionalLight(m_coreEngine->renderingEngine(), Vector3f(1, 1, 1), 0.4);
	m_directionalLightGameObject->transform().setRotation(Quaternion(Vector3f(1, 0, 0), Utils::toRadians(-45)));
	m_directionalLightGameObject->addComponent(m_directionalLight.get());
	m_rootGameObject->addChild(m_directionalLightGameObject.get());

	m_pointLightGameObject = make_shared<GameObject>();
	m_pointLight.reset();
	m_pointLight = LightsFactory::createPointLight(m_coreEngine->renderingEngine(), Vector3f(0, 1, 0), 0.4,
												   Attenuation(0, 0, 1));
	m_pointLightGameObject->addComponent(m_pointLight.get());
	m_rootGameObject->addChild(m_pointLightGameObject.get());

	m_spotLightGameObject = make_shared<GameObject>();
	m_spotLight.reset();
	m_spotLight = LightsFactory::createSpotLight(m_coreEngine->renderingEngine(), Vector3f(0, 1, 1), 0.8,
												 Attenuation(0, 0, 0.1), 0.7);
	m_spotLightGameObject->addComponent(m_spotLight.get());
	m_spotLightGameObject->transform().setRotation(Quaternion(Vector3f(0, 1, 0), Utils::toRadians(90)));
	m_spotLightGameObject->transform().setTranslation(Vector3f(5, 0, 5));
	m_rootGameObject->addChild(m_spotLightGameObject.get());

	m_monkeyMesh.reset();
	m_monkeyMesh = make_shared<Mesh>(m_monkeyModelPath);
	m_monkeyMaterial = make_shared<Material>();
	m_testTexture.reset();
	m_testTexture = make_shared<Texture>(m_testImagePath);
	m_monkeyMaterial->addTexture("diffuse", m_testTexture.get());
	m_monkeyMaterial->addFloat("specularIntensity", 1);
	m_monkeyMaterial->addFloat("specularPower", 8);
	m_monkeyMeshRenderer = make_shared<MeshRenderer>(m_monkeyMesh.get(), m_monkeyMaterial.get());
	m_monkeyGameObject = make_shared<GameObject>();
	m_monkeyGameObject->addComponent(m_monkeyMeshRenderer.get());
	m_monkeyGameObject->transform().setTranslation(Vector3f(5, 5, 5));
	m_rootGameObject->addChild(m_monkeyGameObject.get());
}

void TestScene::onOpenGLResized(int width, int height)
{
	if (m_camera.use_count() > 0) {
		m_camera->reset(Utils::toRadians(70), float(width) / float(height), 0.01, 1000);
	} else {
		m_camera = make_shared<Camera>(Utils::toRadians(70), float(width) / float(height), 0.01, 1000);

		m_cameraGameObject = make_shared<GameObject>();
		m_cameraGameObject->addComponent(m_camera.get());
		m_rootGameObject->addChild(m_cameraGameObject.get());
	}
}

void TestScene::setEngine(CoreEngine *coreEngine)
{
	m_coreEngine = coreEngine;
}

void TestScene::update(float dt) {
	SceneWithRootObject::update(dt);

	Vector3f forward = m_camera->transform().rotation().calculateForward();
	Vector3f right = m_camera->transform().rotation().calculateRight();
	m_camera->transform().setTranslation(
			m_camera->transform().translation() + forward * m_controller->calculateMoveSpeed() * dt
	);
	m_camera->transform().setTranslation(
			m_camera->transform().translation() + right * m_controller->calculateStrafeSpeed() * dt
	);

	m_camera->transform().rotate(m_camera->yAxis, Utils::toRadians(m_controller->calculateYawRotationSpeed() * dt));
	m_camera->transform().rotate(m_camera->transform().rotation().calculateRight(),
								 Utils::toRadians(m_controller->calculatePitchRotationSpeed() * dt));

}

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

const string TestScene::DEFAULT_NORMAL_MAP_IMAGE_KEY = "default_normal_map";
const string TestScene::DEFAULT_DISPLACEMENT_MAP_IMAGE_KEY = "default_displacement_map";
const string TestScene::TEST_IMAGE_KEY = "test";
const string TestScene::BRICKS_IMAGE_KEY = "bricks";
const string TestScene::BRICKS_NORMAL_MAP_IMAGE_KEY = "bricks_normal_map";
const string TestScene::BRICKS_DISPLACEMENT_MAP_IMAGE_KEY = "bricks_displacement_map";
const string TestScene::MONKEY_MODEL_KEY = "monkey";
const string TestScene::LAND_MODEL_KEY = "land";
const string TestScene::TAG = "TestScene";

TestScene::TestScene(const unordered_map<string, string> &paths, JoystickInput &leftJoystickInput,
					 JoystickInput &rightJoystickInput)
{
	m_defaultNormalMapImagePath = paths.at(DEFAULT_NORMAL_MAP_IMAGE_KEY);
	m_defaultDisplacementMapImagePath = paths.at(DEFAULT_DISPLACEMENT_MAP_IMAGE_KEY);
	m_testImagePath = paths.at(TEST_IMAGE_KEY);
	m_bricksImagePath = paths.at(BRICKS_IMAGE_KEY);
	m_bricksNormalMapImagePath = paths.at(BRICKS_NORMAL_MAP_IMAGE_KEY);
	m_bricksDisplacementMapImagePath = paths.at(BRICKS_DISPLACEMENT_MAP_IMAGE_KEY);
	m_monkeyModelPath = paths.at(MONKEY_MODEL_KEY);
	m_landModelPath = paths.at(LAND_MODEL_KEY);

	m_controller = make_shared<TestController>(leftJoystickInput, rightJoystickInput);
}

void TestScene::makeOpenGLDependentSetup()
{
	m_rootGameObject = make_shared<GameObject>();
	m_rootGameObject->setEngine(m_coreEngine);
	m_camera.reset();

	m_defaultNormalMapTexture.reset();
	m_defaultDisplacementMapTexture.reset();
	m_defaultNormalMapTexture = make_shared<Texture>(m_defaultNormalMapImagePath);
	m_defaultDisplacementMapTexture = make_shared<Texture>(m_defaultDisplacementMapImagePath);

	m_landMesh.reset();
	m_monkeyPlaneMesh.reset();
	m_landMesh = make_shared<Mesh>(m_landModelPath);
	m_bricksTexture.reset();
	m_bricksTexture = make_shared<Texture>(m_bricksImagePath);
	m_bricksNormalMapTexture.reset();
	m_bricksDisplacementMapTexture.reset();
	m_bricksNormalMapTexture = make_shared<Texture>(m_bricksNormalMapImagePath);
	m_bricksDisplacementMapTexture = make_shared<Texture>(m_bricksDisplacementMapImagePath);
	m_landMaterial = make_shared<Material>(
			m_bricksTexture.get(),
			0.5f,
			4.0f,
			m_bricksNormalMapTexture.get(),
			m_bricksDisplacementMapTexture.get(),
			0.03f
	);
	m_landMeshRenderer = make_shared<MeshRenderer>(m_landMesh.get(), m_landMaterial.get());
	m_landGameObject = make_shared<GameObject>();
	m_landGameObject->addComponent(m_landMeshRenderer.get());
	m_landGameObject->transform().setTranslation(Vector3f(0, -1, 0));
	m_landGameObject->transform().setScale(Vector3f(4, 4, 4));
	m_rootGameObject->addChild(m_landGameObject.get());

	m_directionalLightGameObject = make_shared<GameObject>();
	m_directionalLight.reset();
	m_directionalLight.reset(LightsFactory::createDirectionalLight(m_coreEngine->renderingEngine(), Vector3f(1, 1, 1),
																   0.4));
	m_directionalLightGameObject->transform().setRotation(Quaternion(Vector3f(1, 0, 0), Utils::toRadians(-45)));
	m_directionalLightGameObject->addComponent(m_directionalLight.get());
	m_rootGameObject->addChild(m_directionalLightGameObject.get());

	m_pointLightGameObject = make_shared<GameObject>();
	m_pointLight.reset();
	m_pointLight.reset(LightsFactory::createPointLight(m_coreEngine->renderingEngine(), Vector3f(0, 1, 0), 0.4,
													   Attenuation(0, 0, 1)));
	m_pointLightGameObject->addComponent(m_pointLight.get());
	m_rootGameObject->addChild(m_pointLightGameObject.get());

	m_spotLightGameObject = make_shared<GameObject>();
	m_spotLight.reset();
	m_spotLight.reset(LightsFactory::createSpotLight(m_coreEngine->renderingEngine(), Vector3f(0, 1, 1), 0.8,
													 Attenuation(0, 0, 0.1), 0.7));
	m_spotLightGameObject->addComponent(m_spotLight.get());
	m_spotLightGameObject->transform().setRotation(Quaternion(Vector3f(0, 1, 0), Utils::toRadians(90)));
	m_spotLightGameObject->transform().setTranslation(Vector3f(5, 0, 5));
	m_rootGameObject->addChild(m_spotLightGameObject.get());

	m_monkeyMesh.reset();
	m_monkeyMesh = make_shared<Mesh>(m_monkeyModelPath);
	m_testTexture.reset();
	m_testTexture = make_shared<Texture>(m_testImagePath);
	m_monkeyMaterial = make_shared<Material>(
			m_testTexture.get(),
			1.0f,
			8.0f,
			m_defaultNormalMapTexture.get(),
			m_defaultDisplacementMapTexture.get()
	);
	m_monkeyMeshRenderer = make_shared<MeshRenderer>(m_monkeyMesh.get(), m_monkeyMaterial.get());
	m_monkeyGameObject = make_shared<GameObject>();
	m_monkeyGameObject->addComponent(m_monkeyMeshRenderer.get());
	m_monkeyGameObject->transform().setTranslation(Vector3f(5, 10, 5));
	m_lookAtComponent = make_shared<LookAtComponent>();
	m_monkeyGameObject->addComponent(m_lookAtComponent.get());
	m_rootGameObject->addChild(m_monkeyGameObject.get());

	m_monkeyPlaneMesh = make_shared<Mesh>(m_landModelPath);
	m_monkeyPlaneMeshRenderer = make_shared<MeshRenderer>(m_monkeyPlaneMesh.get(), m_landMaterial.get());
	m_monkeyPlaneGameObject = make_shared<GameObject>();
	m_monkeyPlaneGameObject->addComponent(m_monkeyPlaneMeshRenderer.get());
	m_monkeyPlaneGameObject->transform().setTranslation(Vector3f(0, 0, -5));
	m_monkeyPlaneGameObject->transform().setRotation(Quaternion(Vector3f(1, 0, 0), Utils::toRadians(90)));
	m_monkeyGameObject->addChild(m_monkeyPlaneGameObject.get());
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

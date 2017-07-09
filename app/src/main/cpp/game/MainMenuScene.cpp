#include "MainMenuScene.h"
#include <utils.h>
#include <engine/core/transform.h>
#include <engine/core/gameobject.h>
#include <fstream>
#include <libs/json-parser/json.h>
#include <logwrapper.h>

const std::string MainMenuScene::TEXTURES_DIR_PATH_KEY = "TEXTURES_DIR_PATH";
const std::string MainMenuScene::MODELS_DIR_PATH_KEY = "MODELS_DIR_PATH_KEY";
const std::string MainMenuScene::SCENES_DIR_PATH_KEY = "SCENES_DIR_PATH_KEY";

const string MainMenuScene::TAG = "MainMenuScene";

MainMenuScene::MainMenuScene(const std::unordered_map<string, string> &dirPaths, JoystickInput &leftJoystickInput,
							 JoystickInput &rightJoystickInput)
{
	m_texturesDirPath = dirPaths.at(TEXTURES_DIR_PATH_KEY);
	m_modelsDirPath = dirPaths.at(MODELS_DIR_PATH_KEY);
	m_scenesDirPath = dirPaths.at(SCENES_DIR_PATH_KEY);

	m_controller = make_shared<TestController>(leftJoystickInput, rightJoystickInput);
}

json_value *MainMenuScene::findJsonValue(json_value *jsonObject, std::string name)
{
	for (int i = 0; i < jsonObject->u.object.length; i++) {
		if (name == jsonObject->u.object.values[i].name)
			return jsonObject->u.object.values[i].value;
	}

	std::string msg = "No object found for name: " + name;
	Log::e(TAG, msg);
	throw std::runtime_error(msg);
}

void MainMenuScene::makeOpenGLDependentSetup()
{
	m_rootGameObject = make_shared<GameObject>();
	m_rootGameObject->setEngine(m_coreEngine);
	m_camera.reset();

	for (auto textureEntry : m_textures)
		delete textureEntry.second;
	m_textures.clear();

	for (auto materialEntry : m_materials)
		delete materialEntry.second;
	m_materials.clear();

	for (auto meshEntry : m_meshes)
		delete meshEntry.second;
	m_meshes.clear();

	std::ifstream is(m_scenesDirPath + "main_menu_scene.json", std::ios::binary);
	std::streampos fileSize = is.tellg();
	is.seekg(0, std::ios::end);
	fileSize = is.tellg() - fileSize;
	is.seekg(0, std::ios::beg);

	char *sceneJson = new char[fileSize];
	is.read(sceneJson, (streamsize) fileSize);
	is.close();

	json_value *sceneJsonObject = json_parse(sceneJson, (size_t) fileSize);

	json_value *texturesJsonObject = findJsonValue(sceneJsonObject, "textures");
	for (int i = 0; i < texturesJsonObject->u.object.length; i++) {
		auto entry = texturesJsonObject->u.object.values[i];
		auto textureJsonObject = entry.value;
		auto filenameJsonString = findJsonValue(textureJsonObject, "filename");
		Log::i(TAG, std::string("Loading texture: ") + filenameJsonString->u.string.ptr);
		m_textures.insert({entry.name, new Texture(m_texturesDirPath + filenameJsonString->u.string.ptr)});
	}

	json_value *materialsJsonObject = findJsonValue(sceneJsonObject, "materials");
	for (int i = 0; i < materialsJsonObject->u.object.length; i++) {
		auto entry = materialsJsonObject->u.object.values[i];
		Log::i(TAG, std::string("Loading material: ") + entry.name);
		auto materialJsonObject = entry.value;

		Texture *diffuse = m_textures.at(findJsonValue(materialJsonObject, "diffuse")->u.string.ptr);
		float specularIntensity = (float) findJsonValue(materialJsonObject, "specularIntensity")->u.dbl;
		float specularPower = (float) findJsonValue(materialJsonObject, "specularPower")->u.dbl;
		Texture *normalMap = m_textures.at(findJsonValue(materialJsonObject, "normalMap")->u.string.ptr);
		Texture *displacementMap = m_textures.at(findJsonValue(materialJsonObject, "displacementMap")->u.string.ptr);
		float displacementMapScale = (float) findJsonValue(materialJsonObject, "displacementMapScale")->u.dbl;
		float displacementMapOffset = (float) findJsonValue(materialJsonObject, "displacementMapOffset")->u.dbl;

		Material *material = new Material(diffuse, specularIntensity, specularPower, normalMap, displacementMap,
										  displacementMapScale, displacementMapOffset);
		m_materials.insert({entry.name, material});
	}

	json_value *meshesJsonObject = findJsonValue(sceneJsonObject, "meshes");
	for (int i = 0; i < meshesJsonObject->u.object.length; i++) {
		auto entry = meshesJsonObject->u.object.values[i];
		auto meshJsonObject = entry.value;
		auto filenameJsonString = findJsonValue(meshJsonObject, "filename");
		Log::i(TAG, std::string("Loading mesh: ") + filenameJsonString->u.string.ptr);
		m_meshes.insert({entry.name, new Mesh(m_modelsDirPath + filenameJsonString->u.string.ptr)});
	}

	// TODO Implement game objects loading

	delete sceneJson;

	/*m_defaultNormalMapTexture.reset();
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
	m_monkeyGameObject->addChild(m_monkeyPlaneGameObject.get());*/
}

void MainMenuScene::onOpenGLResized(int width, int height)
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

void MainMenuScene::setEngine(CoreEngine *coreEngine)
{
	m_coreEngine = coreEngine;
}

void MainMenuScene::update(float dt) {
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

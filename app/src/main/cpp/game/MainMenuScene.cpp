#include "MainMenuScene.h"
#include <utils.h>
#include <engine/core/transform.h>
#include <engine/core/gameobject.h>
#include <fstream>
#include <logwrapper.h>
#include <engine/components/meshrenderer.h>
#include <engine/components/lightsfactory.h>
#include <engine/core/coreengine.h>

const std::string MainMenuScene::TEXTURES_DIR_PATH_KEY = "TEXTURES_DIR_PATH";
const std::string MainMenuScene::MODELS_DIR_PATH_KEY = "MODELS_DIR_PATH_KEY";
const std::string MainMenuScene::SCENES_DIR_PATH_KEY = "SCENES_DIR_PATH_KEY";

const string MainMenuScene::TAG = "MainMenuScene";

const std::string MainMenuScene::GAME_OBJECT_TYPE_CAMERA = "CAMERA";
const std::string MainMenuScene::GAME_OBJECT_TYPE_MESH = "MESH";
const std::string MainMenuScene::GAME_OBJECT_TYPE_DIRECTIONAL_LIGHT = "DIRECTIONAL_LIGHT";

MainMenuScene::MainMenuScene(const std::unordered_map<string, string> &dirPaths, JoystickInput &leftJoystickInput,
							 JoystickInput &rightJoystickInput)
{
	m_texturesDirPath = dirPaths.at(TEXTURES_DIR_PATH_KEY);
	m_modelsDirPath = dirPaths.at(MODELS_DIR_PATH_KEY);
	m_scenesDirPath = dirPaths.at(SCENES_DIR_PATH_KEY);

	m_controller = make_shared<TestController>(leftJoystickInput, rightJoystickInput);
}

MainMenuScene::~MainMenuScene()
{
	freeMemory();
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

void MainMenuScene::freeMemory()
{
	for (auto textureEntry : m_textures)
		delete textureEntry.second;
	m_textures.clear();

	for (auto materialEntry : m_materials)
		delete materialEntry.second;
	m_materials.clear();

	for (auto meshEntry : m_meshes)
		delete meshEntry.second;
	m_meshes.clear();

	for (auto gameObjectEntry : m_gameObjects)
		delete gameObjectEntry.second;
	m_gameObjects.clear();

	for (auto gameComponentEntry : m_gameComponents)
		delete gameComponentEntry;
	m_gameComponents.clear();
}

Vector3f MainMenuScene::parseTranslation(json_value *transformationJsonObject)
{
	auto translationJsonArray = findJsonValue(transformationJsonObject, "translation");
	return Vector3f((float) translationJsonArray->u.array.values[0]->u.dbl,
					(float) translationJsonArray->u.array.values[1]->u.dbl,
					(float) translationJsonArray->u.array.values[2]->u.dbl);
}

Quaternion MainMenuScene::parseRotation(json_value *transformationJsonObject)
{
	auto rotationJsonArray = findJsonValue(transformationJsonObject, "rotation");
	auto rotationAxis = Vector3f((float) rotationJsonArray->u.array.values[0]->u.dbl,
								 (float) rotationJsonArray->u.array.values[1]->u.dbl,
								 (float) rotationJsonArray->u.array.values[2]->u.dbl);
	float rotationAngle = (float) rotationJsonArray->u.array.values[3]->u.dbl;
	return Quaternion(rotationAxis, Utils::toRadians(rotationAngle));
}

void MainMenuScene::loadMeshGameObject(json_value *gameObjectJsonObject, std::string name)
{
	Log::i(TAG, std::string("Loading game object: ") + name);

	auto meshJsonString = findJsonValue(gameObjectJsonObject, "mesh");
	auto materialJsonString = findJsonValue(gameObjectJsonObject, "material");
	auto transformationJsonObject = findJsonValue(gameObjectJsonObject, "transformation");

	GameObject *gameObject = new GameObject();
	m_gameObjects.insert({name, gameObject});
	MeshRenderer *meshRenderer = new MeshRenderer(m_meshes.at(meshJsonString->u.string.ptr),
												  m_materials.at(materialJsonString->u.string.ptr));
	m_gameComponents.push_back(meshRenderer);
	gameObject->addComponent(meshRenderer);

	auto translation = parseTranslation(transformationJsonObject);
	Log::i(TAG, "\ttranslation: " + std::string(translation));
	gameObject->transform().setTranslation(translation);

	auto rotation = parseRotation(transformationJsonObject);
	Vector3f rotationAxis;
	float rotationAngle;
	rotation.calculateAxisAndAngle(rotationAxis, rotationAngle);
	Log::i(TAG, "\trotation: axis: " + std::string(rotationAxis) + "; angle: " + Utils::toString(rotationAngle));
	gameObject->transform().setRotation(rotation);

	auto scaleJsonArray = findJsonValue(transformationJsonObject, "scale");
	auto scale = Vector3f((float) scaleJsonArray->u.array.values[0]->u.dbl,
						  (float) scaleJsonArray->u.array.values[1]->u.dbl,
						  (float) scaleJsonArray->u.array.values[2]->u.dbl);
	Log::i(TAG, "\tscale: " + std::string(scale));
	gameObject->transform().setScale(scale);

	m_rootGameObject->addChild(gameObject);
}

void MainMenuScene::loadDirectionalLightGameObject(json_value *gameObjectJsonObject, std::string name)
{
	Log::i(TAG, std::string("Loading directional light: ") + name);

	auto colorJsonArray = findJsonValue(gameObjectJsonObject, "color");
	auto color = Vector3f((float) colorJsonArray->u.array.values[0]->u.dbl,
						  (float) colorJsonArray->u.array.values[1]->u.dbl,
						  (float) colorJsonArray->u.array.values[2]->u.dbl);
	Log::i(TAG, "\tcolor: " + std::string(color));

	float intensity = (float) findJsonValue(gameObjectJsonObject, "intensity")->u.dbl;
	Log::i(TAG, "\tintensity: " + Utils::toString(intensity));

	auto directionalLight = LightsFactory::createDirectionalLight(m_coreEngine->renderingEngine(), color, intensity);
	m_gameComponents.push_back(directionalLight);

	auto transformationJsonObject = findJsonValue(gameObjectJsonObject, "transformation");
	auto rotationJsonArray = findJsonValue(transformationJsonObject, "rotation");
	auto rotationAxis = Vector3f((float) rotationJsonArray->u.array.values[0]->u.dbl,
								 (float) rotationJsonArray->u.array.values[1]->u.dbl,
								 (float) rotationJsonArray->u.array.values[2]->u.dbl);
	float rotationAngle = (float) rotationJsonArray->u.array.values[3]->u.dbl;
	auto rotation = Quaternion(rotationAxis, Utils::toRadians(rotationAngle));
	Log::i(TAG, "\trotation: axis: " + std::string(rotationAxis) + "; angle: " + Utils::toString(rotationAngle));
	auto gameObject = new GameObject();
	m_gameObjects.insert({name, gameObject});
	gameObject->addComponent(directionalLight);
	gameObject->transform().setRotation(rotation);
	m_rootGameObject->addChild(gameObject);
}

void MainMenuScene::makeOpenGLDependentSetup()
{
	m_rootGameObject = make_shared<GameObject>();
	m_rootGameObject->setEngine(m_coreEngine);
	m_camera.reset();

	freeMemory();

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

	json_value *objectsJsonObject = findJsonValue(sceneJsonObject, "objects");
	for (int i = 0; i < objectsJsonObject->u.object.length; i++) {
		auto entry = objectsJsonObject->u.object.values[i];
		auto type = findJsonValue(entry.value, "type")->u.string.ptr;
		if (type == GAME_OBJECT_TYPE_MESH) {
			loadMeshGameObject(entry.value, entry.name);
		} else if (type == GAME_OBJECT_TYPE_DIRECTIONAL_LIGHT) {
			loadDirectionalLightGameObject(entry.value, entry.name);
		} else if (type == GAME_OBJECT_TYPE_CAMERA) {
			loadCamera(entry.value);
		} else {
			std::string msg = string("Unknown game object type: ") + type;
			Log::e(TAG, msg);
			throw std::runtime_error(msg);
		}
	}

	delete sceneJson;
}

void MainMenuScene::loadCamera(json_value *gameObjectJsonObject)
{
	Log::i(TAG, std::string("Loading camera transformation:"));

	auto transformationJsonObject = findJsonValue(gameObjectJsonObject, "transformation");

	auto translation = parseTranslation(transformationJsonObject);
	Log::i(TAG, "\ttranslation: " + std::string(translation));
	m_cameraStartPosition = translation;

	auto rotation = parseRotation(transformationJsonObject);
	Vector3f rotationAxis;
	float rotationAngle;
	rotation.calculateAxisAndAngle(rotationAxis, rotationAngle);
	Log::i(TAG, "\trotation: axis: " + std::string(rotationAxis) + "; angle: " + Utils::toString(rotationAngle));
	m_cameraStartRotation = rotation;
}

void MainMenuScene::onOpenGLResized(int width, int height)
{
	if (m_camera.use_count() > 0) {
		m_camera->reset(Utils::toRadians(70), float(width) / float(height), 0.1, 10000);
	} else {
		m_camera = make_shared<Camera>(Utils::toRadians(70), float(width) / float(height), 0.1, 10000);

		m_cameraGameObject = make_shared<GameObject>();
		m_cameraGameObject->addComponent(m_camera.get());
		m_cameraGameObject->transform().setTranslation(m_cameraStartPosition);
		m_cameraGameObject->transform().setRotation(m_cameraStartRotation);
		m_rootGameObject->addChild(m_cameraGameObject.get());
	}
}

void MainMenuScene::setEngine(CoreEngine *coreEngine)
{
	m_coreEngine = coreEngine;
}

//static float g_timePassed = 0;
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

	/*g_timePassed += dt;
	if (g_timePassed >= 1) {
		g_timePassed = 0;
		Vector3f axis;
		float angle;
		m_camera->transform().rotation().calculateAxisAndAngle(axis, angle);
		Log::d(TAG, "Camera:");
		Log::d(TAG, "\taxis: " + string(axis) + "; angle: " + Utils::toString(angle));
		Log::d(TAG, "\tposition: " + string(m_camera->transform().translation()));
	}*/
}

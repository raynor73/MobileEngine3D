#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H


#include <engine/core/scenewithrootobject.h>
#include <unordered_map>
#include <engine/components/camera.h>
#include <engine/rendering/texture.h>
#include <json-parser/json.h>
#include <engine/rendering/mesh.h>
#include <engine/rendering/material.h>
#include "JoystickInput.h"
#include "TestController.h"

class MainMenuScene : public SceneWithRootObject
{
public:
	static const std::string TEXTURES_DIR_PATH_KEY;
	static const std::string MODELS_DIR_PATH_KEY;
	static const std::string SCENES_DIR_PATH_KEY;

	MainMenuScene(const std::unordered_map<string, string> &dirPaths, JoystickInput &leftJoystickInput,
				  JoystickInput &rightJoystickInput);

	void makeOpenGLDependentSetup() override;
	void onOpenGLResized(int width, int height) override;
	void update(float dt) override;
	void setEngine(CoreEngine *) override;

private:
	static const string TAG;

	CoreEngine *m_coreEngine;

	std::string m_texturesDirPath;
	std::string m_modelsDirPath;
	std::string m_scenesDirPath;

	std::unordered_map<std::string, Texture *> m_textures;
	std::unordered_map<std::string, Material *> m_materials;
	std::unordered_map<std::string, Mesh *> m_meshes;

	std::shared_ptr<TestController> m_controller;
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<GameObject> m_cameraGameObject;

	json_value *findJsonValue(json_value *jsonObject, std::string name);
};


#endif //MAINMENUSCENE_H

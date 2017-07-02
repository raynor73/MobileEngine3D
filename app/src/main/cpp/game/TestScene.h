#ifndef TESTSCENE_H
#define TESTSCENE_H


#include <engine/core/scenewithrootobject.h>
#include <engine/components/meshrenderer.h>
#include <engine/core/coreengine.h>
#include <engine/components/spotlight.h>
#include "JoystickInput.h"
#include "TestController.h"
#include "lookatcomponent.h"

using namespace std;

class TestScene : public SceneWithRootObject
{
public:
	static const string TEST_IMAGE_KEY;
	static const string BRICKS_IMAGE_KEY;
	static const string BRICKS_NORMAL_MAP_IMAGE_KEY;
	static const string MONKEY_MODEL_KEY;
	static const string LAND_MODEL_KEY;

	TestScene(const unordered_map<string, string> &paths, JoystickInput &leftJoystickInput,
			  JoystickInput &rightJoystickInput);

	void makeOpenGLDependentSetup() override;
	void onOpenGLResized(int width, int height) override;
	void update(float dt) override;
	void setEngine(CoreEngine *) override;

private:
	static const string TAG;

	CoreEngine *m_coreEngine;

	string m_bricksImagePath;
	string m_bricksNormalMapImagePath;
	string m_testImagePath;
	string m_monkeyModelPath;
	string m_landModelPath;

	shared_ptr<TestController> m_controller;
	shared_ptr<Camera> m_camera;
	shared_ptr<GameObject> m_cameraGameObject;
	shared_ptr<GameObject> m_landGameObject;
	shared_ptr<GameObject> m_monkeyGameObject;
	shared_ptr<Material> m_landMaterial;
	shared_ptr<Material> m_monkeyMaterial;
	shared_ptr<Texture> m_testTexture;
	shared_ptr<Texture> m_bricksTexture;
	shared_ptr<Texture> m_bricksNormalMapTexture;
	shared_ptr<Mesh> m_landMesh;
	shared_ptr<Mesh> m_monkeyMesh;
	shared_ptr<MeshRenderer> m_landMeshRenderer;
	shared_ptr<MeshRenderer> m_monkeyMeshRenderer;
	shared_ptr<GameObject> m_directionalLightGameObject;
	shared_ptr<DirectionalLight> m_directionalLight;
	shared_ptr<GameObject> m_pointLightGameObject;
	shared_ptr<PointLight> m_pointLight;
	shared_ptr<GameObject> m_spotLightGameObject;
	shared_ptr<SpotLight> m_spotLight;
	shared_ptr<LookAtComponent> m_lookAtComponent;
};


#endif //TESTSCENE_H

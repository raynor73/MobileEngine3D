#ifndef TESTSCENE_H
#define TESTSCENE_H


#include <engine/core/scenewithrootobject.h>
#include <engine/components/meshrenderer.h>
#include <engine/core/coreengine.h>
#include <engine/components/spotlight.h>
#include "JoystickInput.h"
#include "TestController.h"

using namespace std;

class TestScene : public SceneWithRootObject
{
public:
	TestScene(const string &bricksImagePath, JoystickInput &leftJoystickInput, JoystickInput &rightJoystickInput);

	void makeOpenGLDependentSetup() override;
	void onOpenGLResized(int width, int height) override;
	void update(float dt) override;
	void setEngine(CoreEngine *) override;

private:
	static const string TAG;

	CoreEngine *m_coreEngine;

	string m_bricksImagePath;

	shared_ptr<TestController> m_controller;
	shared_ptr<Camera> m_camera;
	shared_ptr<GameObject> m_cameraGameObject;
	shared_ptr<GameObject> m_landGameObject;
	shared_ptr<Material> m_landMaterial;
	shared_ptr<Texture> m_bricksTexture;
	shared_ptr<Mesh> m_landMesh;
	shared_ptr<MeshRenderer> m_landMeshRenderer;
	shared_ptr<GameObject> m_directionalLightGameObject;
	shared_ptr<DirectionalLight> m_directionalLight;
	shared_ptr<GameObject> m_pointLightGameObject;
	shared_ptr<PointLight> m_pointLight;
	shared_ptr<GameObject> m_spotLightGameObject;
	shared_ptr<SpotLight> m_spotLight;
};


#endif //TESTSCENE_H

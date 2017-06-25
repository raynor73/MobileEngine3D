#ifndef TESTSCENE_H
#define TESTSCENE_H


#include <engine/core/scenewithrootobject.h>
#include <engine/components/meshrenderer.h>

using namespace std;

class TestScene : public SceneWithRootObject
{
public:
	virtual void makeOpenGLDependentSetup() override;
	virtual void onOpenGLResized(int width, int height) override;

private:
	shared_ptr<GameObject> m_landGameObject;
	shared_ptr<Mesh> m_landMesh;
	shared_ptr<MeshRenderer> m_landMeshRenderer;
};


#endif //TESTSCENE_H

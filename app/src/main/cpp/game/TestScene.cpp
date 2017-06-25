#include <engine/core/matrix4f.h>
#include <utils.h>
#include <engine/rendering/vertex.h>
#include <vector>
#include <engine/core/gameobject.h>
#include <engine/components/camera.h>
#include <logwrapper.h>
#include "TestScene.h"

using namespace std;

const string TestScene::TAG = "TestScene";

static vector<Vertex> g_vertices;
vector<unsigned int> g_indices;
void TestScene::makeOpenGLDependentSetup()
{
	float fieldDepth = 10;
	float fieldWidth = 10;

	g_vertices.push_back(Vertex(Vector3f(-fieldWidth, -1, -fieldDepth), Vector2f(0, 0)));
	g_vertices.push_back(Vertex(Vector3f(-fieldWidth, -1, fieldDepth * 3), Vector2f(0, 1)));
	g_vertices.push_back(Vertex(Vector3f(fieldWidth * 3, -1, -fieldDepth), Vector2f(1, 0)));
	g_vertices.push_back(Vertex(Vector3f(fieldWidth * 3, -1, fieldDepth * 3), Vector2f(1, 1)));

	g_indices.push_back(0);
	g_indices.push_back(1);
	g_indices.push_back(2);
	g_indices.push_back(2);
	g_indices.push_back(1);
	g_indices.push_back(3);

	m_landMesh = make_shared<Mesh>();
	m_landMesh->setVertices(g_vertices, g_indices, true);
	m_landMaterial = make_shared<Material>();
	m_landMeshRenderer = make_shared<MeshRenderer>(m_landMesh.get(), m_landMaterial.get());

	m_landGameObject = make_shared<GameObject>();
	m_landGameObject->addComponent(m_landMeshRenderer.get());
	m_rootGameObject->addChild(m_landGameObject.get());
}

void TestScene::onOpenGLResized(int width, int height)
{
	if (m_camera.use_count() > 0) {
		Log::e(TAG, "Camera resizing not supported");
		throw new runtime_error("Camera resizing not supported");
	}

	m_camera = make_shared<Camera>(Utils::toRadians(70), float(width) / float(height), 0.01, 1000);
	m_cameraGameObject = make_shared<GameObject>();
	m_cameraGameObject->addComponent(m_camera.get());
	m_rootGameObject->addChild(m_cameraGameObject.get());
	//g_mvp.initPerspective(Utils::toRadians(70), float(width) / float(height), 0.01, 1000);
}

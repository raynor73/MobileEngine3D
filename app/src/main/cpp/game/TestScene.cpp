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
	m_landMeshRenderer = make_shared<MeshRenderer>(m_landMesh.get(), m_landMaterial.get());

	m_landGameObject = make_shared<GameObject>();
	m_landGameObject->addComponent(m_landMeshRenderer.get());
	m_rootGameObject->addChild(m_landGameObject.get());
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

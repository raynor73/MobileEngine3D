//
// Created by Игорь Лапин on 23.06.17.
//

#include <engine/core/matrix4f.h>
#include <utils.h>
#include <glwrapper.h>
#include <engine/rendering/vertex.h>
#include <vector>
#include "TestScene.h"

using namespace std;

static vector<Vertex> g_vertices;
vector<unsigned int> g_indices;
GLuint g_vertexBufferObjectName;
GLuint g_indexBufferObjectName;
void TestScene::makeOpenGLDependentSetup()
{
	// Buffers
	glGenBuffers(1, &g_vertexBufferObjectName);
	glGenBuffers(1, &g_indexBufferObjectName);

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

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObjectName);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (g_vertices.size() * Vertex::SIZE * sizeof(float)), g_vertices.data(),
				 GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferObjectName);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (g_indices.size() * sizeof(unsigned int)), g_indices.data(),
				 GL_STATIC_DRAW);
}

Matrix4f g_mvp;
void TestScene::onOpenGLResized(int width, int height)
{
	g_mvp.initPerspective(Utils::toRadians(70), float(width) / float(height), 0.01, 1000);
}

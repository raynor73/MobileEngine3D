#ifndef SHADER_H
#define SHADER_H

#include <glwrapper.h>
#include <string>
#include <vector>
#include <memory>
#include <engine/core/vector3f.h>
#include <engine/core/matrix4f.h>
#include "resourcemanagement/shaderresource.h"
#include "material.h"

using namespace std;

class RenderingEngine;
class Camera;
class Transform;
/*class BaseLight;
class DirectionalLight;
class PointLight;
class SpotLight;*/

class Shader
{
public:
	static void clearLoadedShaders() { s_loadedShaders.clear(); }

#ifdef __ANDROID__
	Shader(const string &path, const string &name);
#else
	Shader(const string &path, const string &name, GLuint vertexArrayName);
#endif

	void setUniformi(const string &, int);
	void setUniformf(const string &, float);
	void setUniform(const string &, const Vector3f &);
	void setUniform(const string &, const Matrix4f &);
	void linkProgram();
	void bind();
	virtual void updateUniforms(Transform &, Material &, RenderingEngine &);

private:
	static const string TAG;

	static unordered_map<string, weak_ptr<ShaderResource>> s_loadedShaders;

	static string loadShader(const string &path, const string &name);

	struct StructField {
		StructField(string type, string name) :
				type(type),
				name(name)
		{}

		string type;
		string name;
	};

	struct Uniform : StructField {
		Uniform(string type, string name) : StructField(type, name) {}
	};

	shared_ptr<ShaderResource> m_shaderResource;

	unordered_map<string, GLint> m_uniformLocations;
	vector<Uniform> m_uniforms;

	void compileShader(const string &shaderText, GLenum type);
	unordered_map<string, vector<StructField>> findUniformStructs(const string &shaderText);
	void addUniform(string uniformType, string uniformName,
					unordered_map<string, vector<StructField>> structsWithFields);
	void addAllUniforms(const string &shaderText);
	void addAllAttributes(const string &shaderText);
	void setVertexShader(const string &shaderText);
#ifndef __ANDROID__
	void setGeometryShader(const string &shaderText);
#endif
	void setFragmentShader(const string &shaderText);
	/*void setUniform(const string &name, const BaseLight &value);
	void setUniform(const string &name, DirectionalLight &value);
	void setUniform(const string &name, PointLight &value);
	void setUniform(const string &name, SpotLight &value);*/
	void loadShaderAndPutToCache(const string &path, const string &shaderName);
	void setAttributeLocation(const string &attributeName, GLuint location);
};

#endif // SHADER_H

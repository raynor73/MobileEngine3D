#include "shader.h"
#include <fstream>
#include <regex>
#include <engine/rendering/renderingengine.h>
/*#include <engine/components/baselight.h>
#include <engine/components/directionallight.h>
#include <engine/components/pointlight.h>
#include <engine/components/spotlight.h>*/
#include <engine/rendering/renderutils.h>
#include <logwrapper.h>

unordered_map<string, weak_ptr<ShaderResource>> Shader::s_loadedShaders;

#ifdef __ANDROID__
Shader::Shader(const string &path, const string &name)
{
#else
Shader::Shader(const string &path, const string &name, GLuint vertexArrayName);
{
	glBindVertexArray(vertexArrayName);
#endif
	if (s_loadedShaders.count(name) > 0) {
		weak_ptr<ShaderResource> shaderResource = s_loadedShaders[name];
		if (shaderResource.expired()) {
			s_loadedShaders.erase(name);
			loadShaderAndPutToCache(path, name);
		} else {
			Log::i("Shader", "Shader with name:" + name + "already loaded, reusing program");
			m_shaderResource = shaderResource.lock();
		}
	} else {
		loadShaderAndPutToCache(path, name);
	}
}

void Shader::loadShaderAndPutToCache(const string &path, const string &name)
{
	m_shaderResource = make_shared<ShaderResource>();



	/*string vertexShaderText = loadShader(path, name + ".vsh");
	string fragmentShaderText = loadShader(path, name + ".fsh");

	setVertexShader(vertexShaderText);
	setFragmentShader(fragmentShaderText);

	// TODO Make automatic attributes binding
	setAttributeLocation("position", 0);
	setAttributeLocation("textureCoordinate", 1);

	linkProgram();

	addAllUniforms(vertexShaderText);
	addAllUniforms(fragmentShaderText);*/

	s_loadedShaders[name] = m_shaderResource;
}

void Shader::setVertexShader(const string &text)
{
	compileShader(text, GL_VERTEX_SHADER);
}

#ifndef __ANDROID__
void Shader::setGeometryShader(const string &text)
{
	compileShader(text, GL_GEOMETRY_SHADER);
}
#endif

void Shader::setFragmentShader(const string &text)
{
	compileShader(text, GL_FRAGMENT_SHADER);
}

void Shader::linkProgram()
{
	glLinkProgram(m_shaderResource->programReference());
	if (RenderUtils::glGetProgram(m_shaderResource->programReference(), GL_LINK_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetProgramInfoLog(m_shaderResource->programReference()));
		throw new runtime_error(RenderUtils::_glGetProgramInfoLog(m_shaderResource->programReference()));
	}

	glValidateProgram(m_shaderResource->programReference());
	if (RenderUtils::glGetProgram(m_shaderResource->programReference(), GL_VALIDATE_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetProgramInfoLog(m_shaderResource->programReference()));
		throw new runtime_error(RenderUtils::_glGetProgramInfoLog(m_shaderResource->programReference()));
	}
}

void Shader::bind()
{
	glUseProgram(m_shaderResource->programReference());
}

void Shader::compileShader(const string &text, GLenum type)
{
	GLuint shaderReference = glCreateShader(type);
	if (shaderReference == 0) {
		Log::e("Shader", "Error creating shader");
		throw new runtime_error("Error creating shader");
	}

	GLint length = (GLint) text.length();
	const GLchar *rawText = (GLchar *) text.c_str();
	glShaderSource(shaderReference, 1, &rawText, &length);
	glCompileShader(shaderReference);
	if (RenderUtils::glGetShader(shaderReference, GL_COMPILE_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetShaderInfoLog(shaderReference));
		throw new runtime_error(RenderUtils::_glGetShaderInfoLog(shaderReference));
	}

	glAttachShader(m_shaderResource->programReference(), shaderReference);
}

unordered_map<string, vector<Shader::StructField>> Shader::findUniformStructs(const string &shaderText)
{
	unordered_map<string, vector<StructField>> structsWithFields;

	regex structRe("struct (\\w+)\\n\\{((.|\\n)*?)\\};");
	smatch structMatch;
	regex fieldRe("(\\w+) (\\w+);");
	smatch fieldMatch;

	string shaderTextToSearch = shaderText;
	while (regex_search(shaderTextToSearch, structMatch, structRe)) {
		vector<StructField> fields;
		string fieldsTextToSearch = structMatch[2].str();
		while (regex_search(fieldsTextToSearch, fieldMatch, fieldRe)) {
			fields.push_back(StructField(fieldMatch[1], fieldMatch[2]));
			fieldsTextToSearch = fieldMatch.suffix();
		}
		structsWithFields[structMatch[1]] = fields;

		shaderTextToSearch = structMatch.suffix();
	}

	return structsWithFields;
}

void Shader::addUniform(string uniformType, string uniformName,
						unordered_map<string, vector<StructField>> structsWithFields)
{
	bool shouldAddThis = true;

	if (structsWithFields.count(uniformType) > 0) {
		shouldAddThis = false;
		auto structFields = structsWithFields[uniformType];

		for (int i = 0; i < structFields.size(); i++)
			addUniform(structFields[i].type, uniformName + "." + structFields[i].name, structsWithFields);
	}

	if (!shouldAddThis)
		return;

	GLint uniformLocation = glGetUniformLocation(m_shaderResource->programReference(), uniformName.c_str());
	if (uniformLocation < 0) {
		Log::e("Shader", "Error retrieving uniform location: " + uniformName);
		throw new runtime_error("Error retrieving uniform location: " + uniformName);
	}

	m_uniformLocations[uniformName] = uniformLocation;
}

void Shader::updateUniforms(Transform &transform, Material &material, RenderingEngine &renderingEngine)
{
	glUniformMatrix4fv(m_uniformLocations["T_modelViewProjection"], 1, GL_TRUE, renderingEngine.mainCamera().calculateViewProjection().getM().data());
	glUniform3f(m_uniformLocations["R_ambient"], 0, 0.5, 0);

	/*Matrix4f worldMatrix = transform.transformation();
	Matrix4f projectedMatrix = renderingEngine.mainCamera().calculateViewProjection() * worldMatrix;

	for (int i = 0; i < m_uniforms.size(); i++) {
		auto uniformType = m_uniforms[i].type;
		auto uniformName = m_uniforms[i].name;

		string renderingEnginePrefix = "R_";
		string transformPrefix = "T_";
		string cameraPrefix = "C_";

		if (uniformType == "sampler2D") {
			GLuint samplerSlot = renderingEngine.samplerSlot(uniformName);
			material.findTexture(uniformName)->bind(samplerSlot);
			setUniformi(uniformName, samplerSlot);
		} else if (uniformName.find(transformPrefix) == 0) {
			if (uniformName == "T_modelViewProjection") {
				setUniform(uniformName, projectedMatrix);
			} else if (uniformName == "T_model") {
				setUniform(uniformName, worldMatrix);
			} else {
				Log::e("Shader", "Unknown Transform-related uniform name: " + uniformName + " of type " + uniformType);
				throw new runtime_error("Unknown Transform-related uniform name: " + uniformName + " of type " + uniformType);
			}
		} else if (uniformName.find(renderingEnginePrefix) == 0) {
			string unprefixedUniformName = uniformName.substr(renderingEnginePrefix.size());

			if (uniformType == "vec3")
				setUniform(uniformName, renderingEngine.findVector3f(unprefixedUniformName));
			else if (uniformType == "float")
				setUniformf(uniformName, renderingEngine.findFloat(unprefixedUniformName));
			else if (uniformType == "DirectionalLight")
				setUniform(uniformName, static_cast<DirectionalLight &>(renderingEngine.activeLight()));
			else if (uniformType == "PointLight")
				setUniform(uniformName, static_cast<PointLight &>(renderingEngine.activeLight()));
			else if (uniformType == "SpotLight")
				setUniform(uniformName, static_cast<SpotLight &>(renderingEngine.activeLight()));
			else
				renderingEngine.updateUniformStruct(transform, material, *this, uniformType, uniformName);
		} else if (uniformName.find(cameraPrefix) == 0) {
			if (uniformName == "C_eyePosition") {
				setUniform(uniformName, renderingEngine.mainCamera().transform().calculateTransformedTranslation());
			} else {
				Log::e("Shader", "Unknown Camera-related uniform name: " + uniformName + " of type " + uniformType);
				throw new runtime_error("Unknown Camera-related uniform name: " + uniformName + " of type " + uniformType);
			}
		} else {
			if (uniformType == "vec3") {
				setUniform(uniformName, material.findVector3f(uniformName));
			} else if (uniformType == "float") {
				setUniformf(uniformName, material.findFloat(uniformName));
			}else {
				Log::e("Shader", "Unknown uniform name: " + uniformName + " of type " + uniformType);
				throw new runtime_error("Unknown uniform name: " + uniformName + " of type " + uniformType);
			}
		}
	}*/
}

void Shader::addAllUniforms(const string &shaderText)
{
	unordered_map<string, vector<StructField>> structsWithFields = findUniformStructs(shaderText);

	regex re("uniform (\\w*?) ([\\w]+)");
	smatch match;

	string shaderTextToSearch = shaderText;
	while (regex_search(shaderTextToSearch, match, re)) {
		m_uniforms.push_back(Uniform(match[1], match[2]));
		addUniform(match[1], match[2], structsWithFields);
		shaderTextToSearch = match.suffix();
	}
}

void Shader::setUniformi(const string &uniformName, int value)
{
	glUniform1i(m_uniformLocations[uniformName], value);
}

void Shader::setUniformf(const string &uniformName, float value)
{
	glUniform1f(m_uniformLocations[uniformName], value);
}

void Shader::setUniform(const string &uniformName, const Vector3f &value)
{
	glUniform3f(m_uniformLocations[uniformName], value.x(), value.y(), value.z());
}

void Shader::setUniform(const string &uniformName, const Matrix4f &value)
{
	glUniformMatrix4fv(m_uniformLocations[uniformName], 1, GL_TRUE, value.getM().data());
}

string Shader::loadShader(const string &path, const string &name)
{
	string shaderText;
	ifstream inputStream(path + name);
	string line;

	while (getline(inputStream, line)) {
		regex re("#include \"([a-z\\.]+)\"");
		smatch match;
		if (regex_search(line, match, re)) {
			shaderText.append(loadShader(path, match[1]));
		} else {
			shaderText.append(line).append("\n");
		}
	}

	inputStream.close();

	return shaderText;
}

/*void Shader::setUniform(const string &uniformName, const BaseLight &baseLight)
{
	setUniform(uniformName + ".color", baseLight.color());
	setUniformf(uniformName + ".intensity", baseLight.intensity());
}

void Shader::setUniform(const string &uniformName, DirectionalLight &directionalLight)
{
	setUniform(uniformName + ".base", static_cast<const BaseLight &>(directionalLight));
	setUniform(uniformName + ".direction", directionalLight.direction());
}

void Shader::setUniform(const string &uniformName, PointLight &pointLight)
{
	setUniform(uniformName + ".base", static_cast<const BaseLight &>(pointLight));
	setUniformf(uniformName + ".attenuation.constant", pointLight.attenuation().constant());
	setUniformf(uniformName + ".attenuation.linear", pointLight.attenuation().linear());
	setUniformf(uniformName + ".attenuation.exponent", pointLight.attenuation().exponent());
	setUniform(uniformName + ".position", pointLight.transform().calculateTransformedTranslation());
	setUniformf(uniformName + ".range", pointLight.range());
}

void Shader::setUniform(const string &uniformName, SpotLight &spotLight)
{
	setUniform(uniformName + ".pointLight", static_cast<PointLight &>(spotLight));
	setUniform(uniformName + ".direction", spotLight.direction());
	setUniformf(uniformName + ".cutoff", spotLight.cutoff());
}*/

void Shader::setAttributeLocation(const string &attributeName, GLuint location)
{
	glBindAttribLocation(m_shaderResource.get()->programReference(), location, attributeName.c_str());
}

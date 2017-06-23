#include <jni.h>
#include <string>
#include <glwrapper.h>
#include <game/TestUserInput.h>
#include <fstream>
#include <logwrapper.h>
#include <engine/rendering/renderutils.h>
#include <engine/core/matrix4f.h>
#include <engine/rendering/vertex.h>
#include <utils.h>
#include <vector>

using namespace std;

string loadShader(const string &path, const string &name)
{
	string shaderText;
	ifstream inputStream(path + name);
	string line;

	while (getline(inputStream, line)) {
		/*regex re("#include \"([a-z\\.]+)\"");
		smatch match;
		if (regex_search(line, match, re)) {
			shaderText.append(loadShader(path, match[1]));
		} else {
			shaderText.append(line).append("\n");
		}*/
		shaderText.append(line).append("\n");
	}

	inputStream.close();

	return shaderText;
}

extern "C"
{

static bool g_isEngineInitialized = false;

static TestUserInput *g_userInput;

static string g_shadersDirPath;

static GLuint g_programReference;
static GLint g_ambientUniformLocation;
static GLint g_mvpUniformLocation;

static Matrix4f g_mvp;

static GLuint g_vertexBufferObjectName;
static GLuint g_indexBufferObjectName;

static vector<Vertex> g_vertices;
static vector<unsigned int> g_indices;

GLint getUniformLocation(const string &uniformName)
{
	GLint uniformLocation = glGetUniformLocation(g_programReference, uniformName.c_str());
	if (uniformLocation < 0) {
		Log::e("Shader", "Error retrieving uniform location: " + uniformName);
		throw new runtime_error("Error retrieving uniform location: " + uniformName);
	}

	return uniformLocation;
}

void linkProgram()
{
	glLinkProgram(g_programReference);
	if (RenderUtils::glGetProgram(g_programReference, GL_LINK_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetProgramInfoLog(g_programReference));
		throw new runtime_error(RenderUtils::_glGetProgramInfoLog(g_programReference));
	}

	glValidateProgram(g_programReference);
	if (RenderUtils::glGetProgram(g_programReference, GL_VALIDATE_STATUS) != GL_TRUE) {
		Log::e("Shader", RenderUtils::_glGetProgramInfoLog(g_programReference));
		throw new runtime_error(RenderUtils::_glGetProgramInfoLog(g_programReference));
	}
}

void bindShader()
{
	glUseProgram(g_programReference);
}

void compileShader(const string &text, GLenum type)
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

	glAttachShader(g_programReference, shaderReference);
}


JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_initEngine(JNIEnv *env, jobject,
																			 jstring shadersDirPath_) {
	if (g_isEngineInitialized)
		return;

	g_userInput = new TestUserInput();

	const char *shadersDirPath = env->GetStringUTFChars(shadersDirPath_, 0);
	//const char *brickTexturePath = env->GetStringUTFChars(brickTexturePath_, 0);

	g_shadersDirPath = shadersDirPath;
	/*coreEngine = new CoreEngine(string(shaderDirPath), *g_userInput);
	scene = new TestScene(*coreEngine, string(brickTexturePath));
	coreEngine->setScene(scene);*/

	env->ReleaseStringUTFChars(shadersDirPath_, shadersDirPath);
	//env->ReleaseStringUTFChars(brickTexturePath_, brickTexturePath);*/
}

JNIEXPORT void JNICALL
Java_ru_ilapin_mobileengine3d_MainActivity_onJoystickPositionChanged(JNIEnv *env, jobject, jobject position)
{
	if (!g_isEngineInitialized)
		return;

	jclass positionClass = env->GetObjectClass(position);

	jmethodID getXMethodId = env->GetMethodID(positionClass, "getX", "()F");
	jmethodID getYMethodId = env->GetMethodID(positionClass, "getY", "()F");

	float x = env->CallFloatMethod(position, getXMethodId);
	float y = env->CallFloatMethod(position, getYMethodId);

	g_userInput->onJoystickPositionChanged(TestUserInput::JoystickPosition(x, y));
}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onSurfaceCreated(JNIEnv *, jclass)
{
	glClearColor(0, 0, 0.5, 0);

	// Shaders
	g_programReference = glCreateProgram();

	if (g_programReference == 0) {
		Log::e("ShaderResource", "Error creating shader program");
		throw new runtime_error("Error creating shader program");
	}

	string vertexShaderText = loadShader(g_shadersDirPath, "forwardambient.vsh");
	string fragmentShaderText = loadShader(g_shadersDirPath, "forwardambient.fsh");

	compileShader(vertexShaderText, GL_VERTEX_SHADER);
	compileShader(fragmentShaderText, GL_FRAGMENT_SHADER);

	glBindAttribLocation(g_programReference, 0, "position");

	linkProgram();

	g_ambientUniformLocation = getUniformLocation("R_ambient");
	g_mvpUniformLocation = getUniformLocation("T_modelViewProjection");

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

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onSurfaceChanged(JNIEnv *, jclass, jint width,
																				   jint height)
{
	g_mvp.initPerspective(Utils::toRadians(70), float(width) / float(height), 0.01, 1000);
}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onDrawFrame(JNIEnv *, jclass)
{
	glClear(GL_COLOR_BUFFER_BIT);

	bindShader();

	glUniformMatrix4fv(g_mvpUniformLocation, 1, GL_TRUE, g_mvp.getM().data());
	glUniform3f(g_ambientUniformLocation, 0, 0.5, 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObjectName);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vertex::SIZE * sizeof(float), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indexBufferObjectName);

	glDrawElements(GL_TRIANGLES, g_indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
}

}

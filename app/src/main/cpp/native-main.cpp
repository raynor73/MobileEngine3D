#include <jni.h>
#include <string>
#include <game/TestUserInput.h>
#include <engine/core/coreengine.h>
#include <game/TestScene.h>
#include "logwrapper.h"

using namespace std;

extern "C"
{

static bool g_isEngineInitialized = false;

static TestUserInput *g_userInput;
static CoreEngine *g_coreEngine;
static TestScene *g_scene;

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_initEngine(JNIEnv *env, jobject,
																			 jstring shadersDirPath_,
																			 jstring bricksImagePath_) {
	if (g_isEngineInitialized)
		return;
	g_isEngineInitialized = true;

	const char *shadersDirPath = env->GetStringUTFChars(shadersDirPath_, 0);
	const char *bricksImagePath = env->GetStringUTFChars(bricksImagePath_, 0);

	g_userInput = new TestUserInput();
	g_coreEngine = new CoreEngine(shadersDirPath, *g_userInput);
	g_scene = new TestScene(bricksImagePath);

	g_coreEngine->setScene(g_scene);

	env->ReleaseStringUTFChars(shadersDirPath_, shadersDirPath);
	env->ReleaseStringUTFChars(bricksImagePath_, bricksImagePath);
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
	g_coreEngine->onOpenGLReady();
}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onSurfaceChanged(JNIEnv *, jclass, jint width,
																				   jint height)
{
	g_coreEngine->onOpenGLResized(width, height);
}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onDrawFrame(JNIEnv *, jclass)
{
	g_coreEngine->onRender();
}

}

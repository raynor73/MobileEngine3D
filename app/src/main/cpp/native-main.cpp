#include <jni.h>
#include <string>
#include <game/JoystickInput.h>
#include <engine/core/coreengine.h>
#include <game/TestScene.h>
#include "logwrapper.h"

using namespace std;

extern "C"
{

static bool g_isEngineInitialized = false;

static JoystickInput *g_rightJoystickInput;
static JoystickInput *g_leftJoystickInput;
static CoreEngine *g_coreEngine;
static TestScene *g_scene;

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_initEngine(JNIEnv *env, jobject,
																			 jobjectArray paths_) {
	if (g_isEngineInitialized)
		return;
	g_isEngineInitialized = true;

	jstring shadersDirPath_ = (jstring) (env->GetObjectArrayElement(paths_, 0));
	jstring bricksImagePath_ = (jstring) (env->GetObjectArrayElement(paths_, 1));
	jstring testImagePath_ = (jstring) (env->GetObjectArrayElement(paths_, 2));
	jstring monkeyModelPath_ = (jstring) (env->GetObjectArrayElement(paths_, 3));
	jstring bricksNormalMapImagePath_ = (jstring) (env->GetObjectArrayElement(paths_, 4));
	jstring landModelPath_ = (jstring) (env->GetObjectArrayElement(paths_, 5));
	jstring defaultNormalMapImagePath_ = (jstring) (env->GetObjectArrayElement(paths_, 6));

	const char *shadersDirPath = env->GetStringUTFChars(shadersDirPath_, 0);
	const char *bricksImagePath = env->GetStringUTFChars(bricksImagePath_, 0);
	const char *bricksNormalMapImagePath = env->GetStringUTFChars(bricksNormalMapImagePath_, 0);
	const char *testImagePath = env->GetStringUTFChars(testImagePath_, 0);
	const char *monkeyModelPath = env->GetStringUTFChars(monkeyModelPath_, 0);
	const char *landModelPath = env->GetStringUTFChars(landModelPath_, 0);
	const char *defaultNormalMapImagePath = env->GetStringUTFChars(defaultNormalMapImagePath_, 0);

	unordered_map<string, string> paths;
	paths.insert({TestScene::BRICKS_IMAGE_KEY, bricksImagePath});
	paths.insert({TestScene::BRICKS_NORMAL_MAP_IMAGE_KEY, bricksNormalMapImagePath});
	paths.insert({TestScene::TEST_IMAGE_KEY, testImagePath});
	paths.insert({TestScene::MONKEY_MODEL_KEY, monkeyModelPath});
	paths.insert({TestScene::LAND_MODEL_KEY, landModelPath});
	paths.insert({TestScene::DEFAULT_NORMAL_MAP_IMAGE_KEY, defaultNormalMapImagePath});

	g_leftJoystickInput = new JoystickInput();
	g_rightJoystickInput = new JoystickInput();
	g_coreEngine = new CoreEngine(shadersDirPath);
	g_scene = new TestScene(paths, *g_leftJoystickInput, *g_rightJoystickInput);

	g_coreEngine->setScene(g_scene);

	env->ReleaseStringUTFChars(shadersDirPath_, shadersDirPath);
	env->ReleaseStringUTFChars(bricksImagePath_, bricksImagePath);
	env->ReleaseStringUTFChars(bricksNormalMapImagePath_, bricksNormalMapImagePath);
	env->ReleaseStringUTFChars(testImagePath_, testImagePath);
	env->ReleaseStringUTFChars(monkeyModelPath_, monkeyModelPath);
	env->ReleaseStringUTFChars(landModelPath_, landModelPath);
	env->ReleaseStringUTFChars(defaultNormalMapImagePath_, defaultNormalMapImagePath);
}

JNIEXPORT void JNICALL
Java_ru_ilapin_mobileengine3d_MainActivity_onRightJoystickPositionChanged(JNIEnv *env, jobject, jobject position)
{
	if (!g_isEngineInitialized)
		return;

	jclass positionClass = env->GetObjectClass(position);

	jmethodID getXMethodId = env->GetMethodID(positionClass, "getX", "()F");
	jmethodID getYMethodId = env->GetMethodID(positionClass, "getY", "()F");

	float x = env->CallFloatMethod(position, getXMethodId);
	float y = env->CallFloatMethod(position, getYMethodId);

	g_rightJoystickInput->onJoystickPositionChanged(JoystickInput::JoystickPosition(x, y));
}

JNIEXPORT void JNICALL
Java_ru_ilapin_mobileengine3d_MainActivity_onLeftJoystickPositionChanged(JNIEnv *env, jobject, jobject position)
{
	if (!g_isEngineInitialized)
		return;

	jclass positionClass = env->GetObjectClass(position);

	jmethodID getXMethodId = env->GetMethodID(positionClass, "getX", "()F");
	jmethodID getYMethodId = env->GetMethodID(positionClass, "getY", "()F");

	float x = env->CallFloatMethod(position, getXMethodId);
	float y = env->CallFloatMethod(position, getYMethodId);

	g_leftJoystickInput->onJoystickPositionChanged(JoystickInput::JoystickPosition(x, y));
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

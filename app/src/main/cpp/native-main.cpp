#include <jni.h>
#include <string>
#include <glwrapper.h>
#include <game/TestUserInput.h>

using namespace std;

extern "C"
{

static bool isEngineInitialized = false;
static TestUserInput *userInput;

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_initEngine(JNIEnv *, jobject) {
	if (isEngineInitialized)
		return;

	userInput = new TestUserInput();

	/*const char *shaderDirPath = env->GetStringUTFChars(shaderDirPath_, 0);
	const char *brickTexturePath = env->GetStringUTFChars(brickTexturePath_, 0);

	coreEngine = new CoreEngine(string(shaderDirPath), *userInput);
	scene = new TestScene(*coreEngine, string(brickTexturePath));
	coreEngine->setScene(scene);

	env->ReleaseStringUTFChars(shaderDirPath_, shaderDirPath);
	env->ReleaseStringUTFChars(brickTexturePath_, brickTexturePath);*/
}

JNIEXPORT void JNICALL
Java_ru_ilapin_mobileengine3d_MainActivity_onJoystickPositionChanged(JNIEnv *env, jobject, jobject position)
{
	if (!isEngineInitialized)
		return;

	jclass positionClass = env->GetObjectClass(position);

	jmethodID getXMethodId = env->GetMethodID(positionClass, "getX", "()F");
	jmethodID getYMethodId = env->GetMethodID(positionClass, "getY", "()F");

	float x = env->CallFloatMethod(position, getXMethodId);
	float y = env->CallFloatMethod(position, getYMethodId);

	userInput->onJoystickPositionChanged(TestUserInput::JoystickPosition(x, y));
}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onSurfaceCreated(JNIEnv *, jclass)
{
	glClearColor(0, 0, 0.5, 0);
}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onSurfaceChanged(JNIEnv *, jclass, jint, jint)
{}

JNIEXPORT void JNICALL Java_ru_ilapin_mobileengine3d_MainActivity_onDrawFrame(JNIEnv *, jclass)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

}

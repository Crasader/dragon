#include "native_jni.h"

#include "cocos2d.h"
#include "CCEventType.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "ThirdPlatform.h"

USING_NS_CC;

#define LOG_TAG    "jni"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define getStaticMethodInfo(a, b, c, d) if (!JniHelper::getStaticMethodInfo(a, b, c, d)) LOGD("%s %s does not exist in java", b, c)
#define XY_JAVA_PACKAGE "com/wonpee/battle/JniMethod"

using namespace cocos2d;

extern "C" {

JNIEXPORT jint JNICALL Java_com_wonpee_battle_JniMethod_nativeTest(JNIEnv* env, jobject thiz) {

    int uniqueId = 0;

    return uniqueId;
}

JNIEXPORT void JNICALL Java_com_wonpee_battle_JniMethod_commandToNative(JNIEnv*  env, jobject thiz, int command, jstring data) {
    const char* chars = env->GetStringUTFChars(data, NULL);

    if (chars == NULL)
    {
        ThirdPlatform::getInstance()->handleJavaMsg(command, NULL);
    }
    else
    {
        ThirdPlatform::getInstance()->handleJavaMsg(command, CCString::create(chars));
    }

    env->ReleaseStringUTFChars(data, chars);
}


int notifyToJava(int command, const char *data) {

    JniMethodInfo info;
    getStaticMethodInfo(info, XY_JAVA_PACKAGE, "onNativeCommand", "(ILjava/lang/String;)I");

    JNIEnv *env = info.env;
    int ret = 0;
    if (data && strcmp(data, "") != 0) {
        ret = env->CallStaticIntMethod(info.classID, info.methodID, command, env->NewStringUTF(data));
    } else {
        ret = env->CallStaticIntMethod(info.classID, info.methodID, command, NULL);
    }
    return ret;
}

void writeToClipboard(const char* data) {

    JniMethodInfo info;
    getStaticMethodInfo(info, XY_JAVA_PACKAGE, "onWriteToClipboard", "(Ljava/lang/String;)V");

    JNIEnv* env = info.env;
    if (data && strcmp(data, "") != 0)
    {
        env->CallStaticIntMethod(info.classID, info.methodID, env->NewStringUTF(data));
    }
}

const char* readFromClipboard() {

    JniMethodInfo info;
    getStaticMethodInfo(info, XY_JAVA_PACKAGE, "onReadFromClipboard", "()Ljava/lang/String;");

    JNIEnv* env = info.env;

    jstring data = (jstring)env->CallStaticObjectMethod(info.classID, info.methodID);
    return env->GetStringUTFChars(data, NULL);
}

const char* getUserInfoForPayment() {
    LOGD("apple...native call getUserInfoForPayment");
    JniMethodInfo info;
    getStaticMethodInfo(info, XY_JAVA_PACKAGE, "getUserInfoForPayment", "()Ljava/lang/String;");

    LOGD("apple...native call getUserInfoForPayment ok");

    JNIEnv* env = info.env;

    jstring data = (jstring)env->CallStaticObjectMethod(info.classID, info.methodID);

    LOGD("apple...native call getUserInfoForPayment %s", env->GetStringUTFChars(data, NULL));

    return env->GetStringUTFChars(data, NULL);
}

}



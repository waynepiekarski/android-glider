#include <android/log.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>


JavaVM* jni_java_vm = NULL;
JNIEnv* jni_env = NULL;
jobject jni_object = NULL;
jbyteArray jni_array;
int jni_array_len = 0;

#define MAX_BYTE_ARRAY_LEN 1024

#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "AnsiTerminal-JNI", fmt, ## __VA_ARGS__)
#define FATAL(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "AnsiTerminal-JNI", "FATAL: " fmt, ## __VA_ARGS__), exit(1)


void java_submitAnsiBuffer(const char *buffer) {
  LOGD("submitAnsiBuffer making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to lookup GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "submitAnsiBuffer", "(I[B)V" );
  if (methodID == NULL) FATAL("Failed to lookup GetMethodID");
  int len = strlen(buffer);
  (*jni_env)->SetByteArrayRegion(jni_env, jni_array, 0, len, buffer);
  (*jni_env)->CallVoidMethod (jni_env, jni_object, methodID, len, jni_array);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);
}


char java_waitForKeypress(void) {
  LOGD("waitForKeypress making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "waitForKeypress", "()B" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  jbyte result = (*jni_env)->CallByteMethod (jni_env, jni_object, methodID);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);

  return (result);
}

char java_checkForKeypress(void) {
  LOGD("checkForKeypress making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "checkForKeypress", "()B" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  jbyte result = (*jni_env)->CallByteMethod (jni_env, jni_object, methodID);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);

  return (result);
}


// Method that Java will call to get things started
void
Java_net_waynepiekarski_ansiterminal_AnsiTerminalView_nativeAnsiCode(
                                                  JNIEnv* env,
                                                  jobject thiz )
{
  LOGD("nativeAnsiCode start");
  jni_env = env;
  // Cannot store pointer directly, need to make a global reference to the object
  jni_object = (*env)->NewGlobalRef(jni_env, thiz);
  LOGD("nativeAnsiCode function");
  
  // Allocate byte array
  jni_array = (*jni_env)->NewByteArray( jni_env, MAX_BYTE_ARRAY_LEN );
  jni_array_len = 0;

  // Spin in a loop dispatching buffers
  int c = 0;
  while (1) {
    char buffer [MAX_BYTE_ARRAY_LEN];

    LOGD ("sleep(2)");
    sleep(2);

    sprintf (buffer, "\033[%d;%dH" "Hello world %d to Java", c, c, c);
    LOGD ("Submitting string [%s]", buffer);
    java_submitAnsiBuffer(buffer);

    char ch = java_checkForKeypress();
    LOGD ("received char [%c]=0x%x,d%d", ch, ch, ch);

    c++;
  }
}

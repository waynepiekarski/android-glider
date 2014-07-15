#include <android/log.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>


JavaVM* jni_java_vm = NULL;
JNIEnv* jni_env = NULL;
jobject jni_object = NULL;
jbyteArray jni_array;
int jni_array_len = 0;

// Large default buffer to use for storing each stdio session before flushing
#define MAX_BYTE_ARRAY_LEN 1024*128
char char_buffer[MAX_BYTE_ARRAY_LEN];
char *char_buffer_ptr = char_buffer;
size_t char_buffer_ofs = 0;

#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "AnsiTerminal-JNI", fmt, ## __VA_ARGS__)
#define FATAL(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "AnsiTerminal-JNI", "FATAL: " fmt, ## __VA_ARGS__), exit(1)


void java_submitAnsiBuffer(const char *buffer, size_t len) {
  LOGD("submitAnsiBuffer making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to lookup GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "submitAnsiBuffer", "(I[B)V" );
  if (methodID == NULL) FATAL("Failed to lookup GetMethodID");
  (*jni_env)->SetByteArrayRegion(jni_env, jni_array, 0, len, buffer);
  (*jni_env)->CallVoidMethod (jni_env, jni_object, methodID, len, jni_array);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);
}


void ansi_fflush (void)
{
  // Write all the chars in the buffer to the Java processing function
  if (char_buffer_ofs == 0)
    LOGD("Not performing ansi_fflush() since there is nothing in the buffer");
  else {
    java_submitAnsiBuffer(char_buffer, char_buffer_ofs);
    char_buffer_ptr = char_buffer;
    char_buffer_ofs = 0;
  }
}


void ansi_printf (const char *format, ...)
{
  va_list args;
  va_start (args, format);
  int result = vsnprintf (char_buffer_ptr, MAX_BYTE_ARRAY_LEN-char_buffer_ofs, format, args);
  va_end (args);
  if (result < 0) FATAL("Failed to call vsnprintf()");
  char_buffer_ofs += result;
  char_buffer_ptr += result;
}


void ansi_putchar (char ch)
{
  if (char_buffer_ofs == MAX_BYTE_ARRAY_LEN) FATAL("Internal JNI buffer exceeded");
  *char_buffer_ptr = ch;
  char_buffer_ptr++;
  char_buffer_ofs++;
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


int glider_main (void);


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

  // Call into the Glider main() function to get started
  int result = glider_main();
  LOGD ("glider_main() ended with exit code %d", result);
}

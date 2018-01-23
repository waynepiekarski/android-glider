// ---------------------------------------------------------------------
//
// Glider
//
// Copyright (C) 1996-2018 Wayne Piekarski
// wayne@tinmith.net http://tinmith.net/wayne
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------

#include <android/log.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <unistd.h>
#include "ansi.h"


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

#undef LOGD
// #define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "Glider-JNI", "%s: %d " fmt, __FILE__, __LINE__, ## __VA_ARGS__)
#define LOGD(fmt, ...) { /* Do nothing */ }
#define FATAL(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, "Glider-JNI", "FATAL: %s:%d " fmt, __FILE__, __LINE__, ## __VA_ARGS__), exit(1)


void java_submitAnsiBuffer(const char *buffer, size_t len) {
  LOGD("submitAnsiBuffer making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to lookup GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "submitAnsiBuffer", "(I[B)V" );
  if (methodID == NULL) FATAL("Failed to lookup GetMethodID");
  (*jni_env)->SetByteArrayRegion(jni_env, jni_array, 0, len, (const jbyte *)buffer);
  (*jni_env)->CallVoidMethod (jni_env, jni_object, methodID, len, jni_array);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);
}


void ansi_fflush (void)
{
  // Write all the chars in the buffer to the Java processing function
  if (char_buffer_ofs == 0) {
    LOGD("Not performing ansi_fflush() since there is nothing in the buffer");
  } else {
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

char java_waitForArrowpress(void) {
  LOGD("waitForArrowpress making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "waitForArrowpress", "()B" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  jbyte result = (*jni_env)->CallByteMethod (jni_env, jni_object, methodID);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);

  return (result);
}

void java_blockUntilKeypress(void) {
  LOGD("blockUntilKeypress making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "blockUntilKeypress", "()V" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  (*jni_env)->CallVoidMethod (jni_env, jni_object, methodID);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);
}

void java_clearForKeypress(void) {
  LOGD("clearForKeypress making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "clearForKeypress", "()V" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  (*jni_env)->CallVoidMethod (jni_env, jni_object, methodID);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);
}

int java_timeUntilKeypress(int microseconds) {
  LOGD("timeUntilKeypress making call to Java with %d usec", microseconds);

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "timeUntilKeypress", "(I)Z" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  int result = (*jni_env)->CallBooleanMethod (jni_env, jni_object, methodID, microseconds);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);

  return result;
}

int java_getPackageVersion(void) {
  LOGD("getPackageVersion making call to Java");

  jclass clazz = (*jni_env)->GetObjectClass( jni_env, jni_object );
  if (clazz == NULL) FATAL("Failed to GetObjectClass");
  jmethodID methodID = (*jni_env)->GetMethodID( jni_env, clazz, "getPackageVersion", "()I" );
  if (methodID == NULL) FATAL("Failed to GetMethodID");
  int result = (*jni_env)->CallIntMethod (jni_env, jni_object, methodID);
  (*jni_env)->DeleteLocalRef(jni_env, clazz);

  return result;
}


int glider_main (void);


// Method that Java will call to get things started
void
Java_net_waynepiekarski_glider_AnsiTerminalView_nativeAnsiCode(
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



// Implementation of terminal.c functions
void flush_stdin (void)
{
  // Clear all input characters
  java_clearForKeypress();
}

int wait_for_key (void)
{
  // Update terminal with any output before we go to sleep
  ansi_fflush();
  // Sleep until a keypress arrives, then return it back
  return java_waitForKeypress();
}

void wait_for_keypress (void)
{
  // Update terminal with any output before we go to sleep
  ansi_fflush();
  // Sleep until a keypress arrives, then leave it ready for reading
  java_blockUntilKeypress();
}

struct get_arrow_keys;

int get_arrow (int enable_arrows, struct get_arrow_keys *set1, struct get_arrow_keys *set2)
{
  // Update terminal with any output before we go to sleep
  ansi_fflush();
  char pressed = java_waitForArrowpress();
  switch (pressed) {
  case '4': return DIR_left;
  case '8': return DIR_up;
  case '6': return DIR_right;
  case '2': return DIR_down;
  default: return DIR_unknown;
  }
}

void terminal_sleep (int seconds, int microseconds)
{
  ansi_fflush (); /* Make sure all output gets sent to the screen before we sleep */
  usleep (seconds * 1000000 + microseconds);
}

int delay_for_key (int seconds, int microseconds)
{
  // Update terminal with any output before we go to sleep
  ansi_fflush();
  // Flushes the input queue, waits the specified time. Then, we check the input buffer
  // and return true if there are any keys available. But don't let the key shorten the time
  // of the sleep that we do!
  terminal_sleep (seconds, microseconds);

  // Are there any keys available?
  return java_timeUntilKeypress(0);
}

char terminal_getchar (void)
{
  return wait_for_key();
}

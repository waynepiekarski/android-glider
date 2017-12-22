LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := native-jni
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES := \
	glider_gfx.c \
	glider_stats.c \
	glider.c \
	glider_anim.c \
	native-jni.c \
	ansi.c \
	terminal.c

include $(BUILD_SHARED_LIBRARY)

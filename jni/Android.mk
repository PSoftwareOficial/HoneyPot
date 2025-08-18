LOCAL_PATH := $(call my-dir)

# -------------------------------
# Build native_app_glue as a static library
# -------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := native_app_glue
LOCAL_SRC_FILES := $(NDK_ROOT)/sources/android/native_app_glue/android_native_app_glue.c
LOCAL_EXPORT_C_INCLUDES := $(NDK_ROOT)/sources/android/native_app_glue

include $(BUILD_STATIC_LIBRARY)

# -------------------------------
# Build FreeType as a static library
# -------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := freetype

# List the necessary source files for FreeType
LOCAL_SRC_FILES := \
    ../src/main/cpp/rendering/freetype/src/autofit/autofit.c \
    ../src/main/cpp/rendering/freetype/src/base/ftbase.c \
    ../src/main/cpp/rendering/freetype/src/base/ftbitmap.c \
    ../src/main/cpp/rendering/freetype/src/base/ftglyph.c \
    ../src/main/cpp/rendering/freetype/src/base/ftinit.c \
    ../src/main/cpp/rendering/freetype/src/base/ftsystem.c \
    ../src/main/cpp/rendering/freetype/src/smooth/ftsmooth.c \
    ../src/main/cpp/rendering/freetype/src/truetype/truetype.c \
    ../src/main/cpp/rendering/freetype/src/raster/raster.c \
    ../src/main/cpp/rendering/freetype/src/cache/ftcache.c

# Include paths for headers
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../src/main/cpp/rendering/freetype/include \
    $(LOCAL_PATH)/../src/main/cpp/rendering/freetype/src

# Compiler flags (optional debugging)
LOCAL_CFLAGS := -DDEBUG -Wall -Wno-unused-function -Wno-unused-variable

include $(BUILD_STATIC_LIBRARY)

# -------------------------------
# Build main shared library
# -------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE := baseNativeLib
LOCAL_SRC_FILES := \
    ../app/src/main/cpp/Engine/Engine.cpp \
    ../app/src/main/cpp/custom/UI/UIManager.cpp\
    ../app/src/main/cpp/rendering/text/text.cpp

LOCAL_CFLAGS := -Wall -Wno-unused-function -Wno-unused-variable
LOCAL_CPPFLAGS := -std=gnu++17
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3
LOCAL_STATIC_LIBRARIES := native_app_glue freetype

# Workaround to export ANativeActivity_onCreate
LOCAL_LDFLAGS := -u ANativeActivity_onCreate

LOCAL_C_INCLUDES := \
    $(NDK_ROOT)/sources/android/native_app_glue \
    $(LOCAL_PATH)/../app/src/main/cpp/rendering/freetype/include \
    $(LOCAL_PATH)/../app/src/main/cpp/rendering/freetype/src

include $(BUILD_SHARED_LIBRARY)
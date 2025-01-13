# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("learnopengl")

set(libs "${CMAKE_SOURCE_DIR}/../jniLibs")

include_directories(
        include

        jni

        render

        sample

        utils
        utils/log
        utils/time
        utils/stb
)

#自定义搜索规则
file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp

        ${CMAKE_SOURCE_DIR}/*/*.cpp

        ${CMAKE_SOURCE_DIR}/*/*/*.cpp

        ${CMAKE_SOURCE_DIR}/*/*/*/*.cpp

        ${CMAKE_SOURCE_DIR}/*/*/*/*/*.cpp

        ${CMAKE_SOURCE_DIR}/*/*/*/*/*/*.cpp

        ${CMAKE_SOURCE_DIR}/*/*/*/*/*/*/*.cpp
        )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(learn-opengles-lib SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${src-files})

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# =====  ASSIMP  =====
add_library(libassimp SHARED IMPORTED )
set_target_properties(libassimp PROPERTIES IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libassimp.so")

# =====  FreeType  =====
add_library(libfreetype SHARED IMPORTED )
set_target_properties(libfreetype PROPERTIES IMPORTED_LOCATION "${libs}/${ANDROID_ABI}/libfreetype.so")

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(learn-opengles-lib
        # List libraries link to the target library
        EGL
        GLESv3
        android
        libfreetype
        libassimp
        ${log-lib}
        aaudio)
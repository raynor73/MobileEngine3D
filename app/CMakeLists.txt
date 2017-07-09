# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

### Prebuild libjpeg
add_library( jpeg-lib
             STATIC
             IMPORTED )

set_target_properties( # Specifies the target library.
                       jpeg-lib

                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION

                       # Provides the path to the library you want to import.
                       ${CMAKE_SOURCE_DIR}/native-libs/${ANDROID_ABI}/libjpeg-local.a )

include_directories(native-libs/includes/libjpeg)
### /Prebuild libjpeg

### jpegutil
add_library ( jpegutil-lib

              STATIC

              src/main/cpp/libs/jpegutil/JpegDec.cpp
              src/main/cpp/libs/jpegutil/JpegDecPlanarYCbCr.cpp
              src/main/cpp/libs/jpegutil/JpegEnc.cpp
              src/main/cpp/libs/jpegutil/jpegutil.cpp )

target_link_libraries( jpegutil-lib

                       jpeg-lib )

include_directories(src/main/cpp/libs/jpegutil)
### /jpegutil

add_library( # Sets the name of the library.
             native-main

             # Sets the library as a shared library.
             SHARED

             src/main/cpp/native-main.cpp

             src/main/cpp/research.cpp

             src/main/cpp/libs/json-parser/json.c

             src/main/cpp/utils.cpp
             src/main/cpp/logwrapper.cpp

             src/main/cpp/engine/core/userinput.cpp
             src/main/cpp/engine/core/vector2f.cpp
             src/main/cpp/engine/core/vector3f.cpp
             src/main/cpp/engine/core/matrix4f.cpp
             src/main/cpp/engine/core/quaternion.cpp
             src/main/cpp/engine/core/transform.cpp
             src/main/cpp/engine/core/coreengine.cpp
             src/main/cpp/engine/core/gameobject.cpp
             src/main/cpp/engine/core/scenewithrootobject.cpp
             src/main/cpp/engine/core/scenewithtimemeasurement.cpp

             src/main/cpp/engine/rendering/renderingengine.cpp
             src/main/cpp/engine/rendering/vertex.cpp
             src/main/cpp/engine/rendering/mesh.cpp
             src/main/cpp/engine/rendering/renderutils.cpp
             src/main/cpp/engine/rendering/material.cpp
             src/main/cpp/engine/rendering/shader.cpp
             src/main/cpp/engine/rendering/texture.cpp
             src/main/cpp/engine/rendering/resourcemanagement/meshresource.cpp
             src/main/cpp/engine/rendering/resourcemanagement/shaderresource.cpp
             src/main/cpp/engine/rendering/resourcemanagement/textureresource.cpp
             src/main/cpp/engine/rendering/meshloading/indexedmodel.cpp
             src/main/cpp/engine/rendering/meshloading/objmodel.cpp

             src/main/cpp/engine/components/camera.cpp
             src/main/cpp/engine/components/gamecomponent.cpp
             src/main/cpp/engine/components/meshrenderer.cpp
             src/main/cpp/engine/components/baselight.cpp
             src/main/cpp/engine/components/directionallight.cpp
             src/main/cpp/engine/components/attenuation.cpp
             src/main/cpp/engine/components/pointlight.cpp
             src/main/cpp/engine/components/spotlight.cpp
             src/main/cpp/engine/components/lightsfactory.cpp

             src/main/cpp/game/TestScene.cpp
             src/main/cpp/game/MainMenuScene.cpp
             src/main/cpp/game/lookatcomponent.cpp
             src/main/cpp/game/TestController.cpp
             src/main/cpp/game/JoystickInput.cpp )

include_directories(src/main/cpp/)
include_directories(src/main/cpp/libs/)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

find_library( # Sets the name of the path variable.
              GLESv2-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              GLESv2 )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-main

                       jpeg-lib
                       jpegutil-lib
                       ${GLESv2-lib}
                       ${log-lib} )
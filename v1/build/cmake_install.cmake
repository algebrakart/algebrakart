# Install script for directory: /code/dev/algebrakart/v1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/code/dev/algebrakart/v1/bin/Editor.sh"
    "/code/dev/algebrakart/v1/bin/NinjaSnowWar.sh"
    "/code/dev/algebrakart/v1/bin/PBRDemo.sh"
    "/code/dev/algebrakart/v1/bin/startServer.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Urho3D/cmake" TYPE DIRECTORY FILES "/code/dev/algebrakart/v1/cmake/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Urho3D/scripts" TYPE PROGRAM FILES
    "/code/dev/algebrakart/v1/script/.bash_helpers.sh"
    "/code/dev/algebrakart/v1/script/cmake_arm.sh"
    "/code/dev/algebrakart/v1/script/cmake_clean.sh"
    "/code/dev/algebrakart/v1/script/cmake_cling_xcode_dylib.sh"
    "/code/dev/algebrakart/v1/script/cmake_codeblocks.sh"
    "/code/dev/algebrakart/v1/script/cmake_codelite.sh"
    "/code/dev/algebrakart/v1/script/cmake_emscripten.sh"
    "/code/dev/algebrakart/v1/script/cmake_generic.sh"
    "/code/dev/algebrakart/v1/script/cmake_ios.sh"
    "/code/dev/algebrakart/v1/script/cmake_ios_metal.sh"
    "/code/dev/algebrakart/v1/script/cmake_mingw.sh"
    "/code/dev/algebrakart/v1/script/cmake_ninja.sh"
    "/code/dev/algebrakart/v1/script/cmake_rpi.sh"
    "/code/dev/algebrakart/v1/script/cmake_tvos.sh"
    "/code/dev/algebrakart/v1/script/cmake_xcode.sh"
    "/code/dev/algebrakart/v1/script/cmake_xcode_dylib.sh"
    "/code/dev/algebrakart/v1/script/cmake_xcode_metal.sh"
    "/code/dev/algebrakart/v1/script/dockerized.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Urho3D" TYPE FILE FILES "/code/dev/algebrakart/v1/rakefile")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/code/dev/algebrakart/v1/build/Source/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Docs/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/code/dev/algebrakart/v1/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")

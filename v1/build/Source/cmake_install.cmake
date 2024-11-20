# Install script for directory: /code/dev/algebrakart/v1/Source

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/ETCPACK/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/FreeType/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/LZ4/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/PugiXml/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/rapidjson/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/SDL/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/StanHull/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/STB/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/muFFT/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/AngelScript/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/LuaJIT/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/toluapp/src/lib/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/Civetweb/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/SLikeNet/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/nanodbc/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/ik/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/Detour/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/DetourCrowd/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/DetourTileCache/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/Recast/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/Box2D/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/WebP/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/Bullet/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/Tracy/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/ThirdParty/GLEW/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/Urho3D/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/Tools/cmake_install.cmake")
  include("/code/dev/algebrakart/v1/build/Source/Game/cmake_install.cmake")

endif()


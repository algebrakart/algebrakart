#
# Copyright (c) 2008-2022 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Define target name
set (TARGET_NAME muFFT)
message("muFFT")
cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_STANDARD 99)
project(muFFT LANGUAGES C)

#if (CMAKE_COMPILER_IS_GNUCC OR (${CMAKE_C_COMPILER_ID} MATCHES "Clang"))
#    # Cannot safely use -ffast-math, breaks on GCC due to reliance on -0.0f behavior.
#    set(MUFFT_C_FLAGS -Wall -Wextra)
#elseif (MSVC)
#    set(MUFFT_C_FLAGS /fp:precise /D_CRT_SECURE_NO_WARNINGS /wd4267 /wd4244 /wd4309 /wd4005 /MP /DNOMINMAX)
#endif()

message("Processor: ${CMAKE_SYSTEM_PROCESSOR}")

option(MUFFT_SIMD_SSE "Enable SSE support if present" ON)
option(MUFFT_SIMD_SSE3 "Enable SSE3 support if present" ON)
option(MUFFT_SIMD_AVX "Enable AVX support if present" ON)
option(MUFFT_ENABLE_FFTW "Enable FFTW support" ON)

if (ANDROID)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Define source
define_source_files (GLOB_CPP_PATTERNS src/*.c GLOB_H_PATTERNS src/*.h src/x86/*.h)

# Define dependency libs
set (INCLUDE_DIRS src)

setup_library(NODEPS)

# Install headers for building the Urho3D library
install_header_files (DIRECTORY src/ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/muFFT FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)  # Note: the trailing slash is significant

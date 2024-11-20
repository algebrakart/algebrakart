#!/bin/bash
export ANDROID_NDK=~/Android/Sdk/ndk/23.1.7779620
export ANDROID_SDK_ROOT=~/Android/Sdk
export PATH=$ANDROID_NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin:$ANDROID_NDK/build:$ANDROID_NDK/prebuilt/linux-x86_64/bin:$ANDROID_SDK/tools:$ANDROID_SDK/tools/bin:$ANDROID_SDK/platform-tools:$PATH

export URHO3D_LIB_TYPE=SHARED 

script/dockerized.sh android ./gradlew :android:launcher-app:clean
script/dockerized.sh android ./gradlew :android:launcher-app:build --info --stacktrace "$@"

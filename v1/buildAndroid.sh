#!/bin/bash
#export ANDROID_NDK=~/Android/Sdk/ndk/21.0.6113669
export ANDROID_NDK=~/Android/Sdk/ndk/23.1.7779620
export ANDROID_SDK_ROOT=~/Android/Sdk
export PATH=$ANDROID_NDK/toolchains/x86_64-4.9/prebuilt/linux-x86_64/bin:$ANDROID_NDK/build:$ANDROID_NDK/prebuilt/linux-x86_64/bin:$ANDROID_SDK/tools:$ANDROID_SDK/tools/bin:$ANDROID_SDK/platform-tools:$PATH

#export URHO3D_HOME=~/workspace/Urho3D/android/urho3d-lib/build/outputs/aar
export URHO3D_LIB_TYPE=SHARED 

script/dockerized.sh android ./gradlew :android:urho3d-lib:clean

script/dockerized.sh android ./gradlew :android:urho3d-lib:build
cd ./android
gradle wrapper --gradle-version 6.2 --distribution-type all
../script/dockerized.sh android ./gradlew Build


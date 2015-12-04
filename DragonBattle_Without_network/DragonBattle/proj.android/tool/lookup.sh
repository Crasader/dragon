#!/bin/sh

if [ ! -d "$NDK_ROOT" ];then
	NDK_ROOT=~/bin/android-ndk-r9
fi

platform=Android
toolchain=$NDK_ROOT/toolchains/arm-linux-androideabi-4.4.3/prebuilt/darwin-x86_64/bin

lib=libgame.so
echo "toolchain : $toolchain"
echo "target    : $lib"

$toolchain/arm-linux-androideabi-addr2line -C -f -e ${PWD}/../obj/local/armeabi/$lib $1 


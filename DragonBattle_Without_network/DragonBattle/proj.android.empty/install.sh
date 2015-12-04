#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

function build() {
    echo "build natvie code."
    $DIR/build_native.sh
}

function build_with_res() {
    echo "build natvie code."
    echo "run ./build_native.sh"
    $DIR/build_native.sh
    
    echo "copy res files."
    echo "./build_res.sh"
    $DIR/build_res.sh
}

function installSo() {
	echo "build natvie code."
	$DIR/build_native.sh

    echo "install so......"
    adb push libs/armeabi/libgame.so /data/data/com.wonpee.battle/lib/libgame.so
}

function installApk() {

    echo "build natvie code."
    $DIR/build_native.sh

    echo "copy res files."
    $DIR/build_res.sh

    echo "ant debug"
    ant clean
    ant debug

    echo "install apk......"
    echo "adb install ./bin/DragonBattle-debug.apk "
    adb install ./bin/DragonBattle-debug.apk 
}

echo "$DIR"

if [[ $1 == "so" ]]; then
	installSo;
fi

if [[ $1 == "apk" ]]; then
	installApk;
fi

if [[ $1 == "adt" ]]; then
	build_with_res;
fi

if [[ $1 == "" ]]; then
	build;
fi

if [[ $1 == "?" ]]; then
	#statements
	echo "Usage:"
	echo "so       -install library."
	echo "apk      -install apk"
fi


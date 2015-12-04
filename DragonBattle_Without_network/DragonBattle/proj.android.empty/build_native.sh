#!/bin/sh

APPNAME="DragonBattleGame"

# options

NDK_ROOT=~/bin/android-ndk-r9

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s	Build externals from source
-h	this help
EOF
}

while getopts "sh" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# ... use paths relative to current directory
COCOS2DX_ROOT="$DIR/../libs"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"

DEBUG=1

echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_ANDROID_ROOT = $APP_ANDROID_ROOT"
echo "NDK_DEBUG = $DEBUG"


# make sure assets is exist

if [[ "$buildexternalsfromsource" ]]; then
    echo "Building external dependencies from source"
    NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source
else
    echo "Using prebuilt externals"
    NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt
fi

"$NDK_ROOT"/ndk-build -C "$APP_ANDROID_ROOT" -j4 $* NDK_MODULE_PATH=$NDK_MODULE_PATH NDK_DEBUG=$DEBUG

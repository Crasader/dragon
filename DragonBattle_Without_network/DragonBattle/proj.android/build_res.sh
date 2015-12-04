#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"

echo "start to copy res files."

#
# default: cp resources to assets
#
function cp_res() {
	
	if [[ $2 == "" ]]; then
		echo "1.cp -rf $1 $APP_ANDROID_ROOT/assets"
		cp -rf $1 "$APP_ANDROID_ROOT"/assets
	fi

	if [[ $2 != "" ]]; then
		echo "2.cp -rf $1 $2"
		cp -rf $1 $2
	fi
}



if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets
fi

mkdir "$APP_ANDROID_ROOT"/assets

cp_res "$APP_ROOT"/Resources/bust
cp_res "$APP_ROOT"/Resources/config
cp_res "$APP_ROOT"/Resources/Community
cp_res "$APP_ROOT"/Resources/lang
cp_res "$APP_ROOT"/Resources/standby
cp_res "$APP_ROOT"/Resources/rolepic
cp_res "$APP_ROOT"/Resources/effect
cp_res "$APP_ROOT"/Resources/music
cp_res "$APP_ROOT"/Resources/headicon
cp_res "$APP_ROOT"/Resources/skillicon
cp_res "$APP_ROOT"/Resources/npc
cp_res "$APP_ROOT"/Resources/item
cp_res "$APP_ROOT"/Resources/start
cp_res "$APP_ROOT"/Resources/action


# copy Resources
echo "copy BattleScene Resources............."
for file in "$APP_ROOT"/Resources/BattleScene/*
do
if [ -d "$file" ]; then
    cp_res "$file" "$APP_ANDROID_ROOT"/assets
fi
done

echo "copy CCBFile Resources............."
for file in "$APP_ROOT"/Resources/ccb/CCBFile/*
do
if [ -d "$file" ]; then
    cp_res "$file" "$APP_ANDROID_ROOT"/assets
fi
done

echo "copy res files done!"

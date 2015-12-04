#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
APP_ROOT="$DIR/.."
APP_ANDROID_ROOT="$DIR"

echo "start to copy res files."

if [ -d "$APP_ANDROID_ROOT"/assets ]; then
    rm -rf "$APP_ANDROID_ROOT"/assets
fi

mkdir "$APP_ANDROID_ROOT"/assets

cp -rf "$APP_ROOT"/Resources/config "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/Community "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/lang "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/standby "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/rolepic "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/effect "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/music "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/headicon "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/skillicon "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/npc "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/item "$APP_ANDROID_ROOT"/assets
cp -rf "$APP_ROOT"/Resources/start "$APP_ANDROID_ROOT"/assets



# copy Resources
for file in "$APP_ROOT"/Resources/action/*
do
if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

for file in "$APP_ROOT"/Resources/BattleScene/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi
done

for file in "$APP_ROOT"/Resources/ccb/CCBFile/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_ANDROID_ROOT"/assets
fi
done

for file in "$APP_ROOT"/Resources/action/*
do
if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

for file in "$APP_ROOT"/Resources/action/monster_1/*
do
if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

for file in "$APP_ROOT"/Resources/action/player_1/*
do
if [ -f "$file" ]; then
    cp "$file" "$APP_ANDROID_ROOT"/assets
fi
done

echo "copy res files done!"
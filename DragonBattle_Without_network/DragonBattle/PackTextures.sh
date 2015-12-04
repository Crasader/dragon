#!/bin/sh

TP="/usr/local/bin/TexturePacker"



echo "${ACTION}"
if [ "${ACTION}" = "clean" ]
then
echo "cleaning..."

rm DragonBattle/Resources/item/items.pvr.ccz
rm DragonBattle/Resources/item/items.plist

# ....
# add all files to be removed in clean phase
# ....
else
echo "buliding..."
# create assets
${TP} --smart-update \
--format cocos2d \
--data DragonBattle/Resources/item/items.plist \
--sheet DragonBattle/Resources/item/items.pvr.ccz \
--dither-fs-alpha \
--opt RGBA8888 \
--premultiply-alpha \
DragonBattle/Resources/TexturePacker/item/*.png

# ....
# add other sheets to create here
# ....
fi
exit 0
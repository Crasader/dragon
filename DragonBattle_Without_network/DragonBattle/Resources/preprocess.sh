#!/bin/sh

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $DIR/action
find * -type d -name .svn|xargs rm -rf
find * -type f -name .DS_Store|xargs rm -rf
find * -type f -name *\!Localizable.strings|xargs rm -rf

cd -

zip -r action.dat action
#zip -r $DIR/config.dat $DIR/config
#zip -r $DIR/action.dat $DIR/effect
#zip -r $DIR/action.dat $DIR/headicon
#zip -r $DIR/action.dat $DIR/lang

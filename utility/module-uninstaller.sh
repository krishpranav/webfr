#!/bin/sh

MODULE_NAME="webfr"
MODULE_VERSION="1.0.0"

echo "remove include folder: '/usr/local/include/webfr-$MODULE_VERSION/$MODULE_NAME'"
rm -rf "/usr/local/include/webfr-$MODULE_VERSION/$MODULE_NAME"

echo "remove cmake package: '/usr/local/lib/cmake/$MODULE_NAME-$MODULE_VERSION'"
rm -rf "/usr/local/lib/cmake/$MODULE_NAME-$MODULE_VERSION"

MODULE_LIB_PATH="/usr/local/lib/webfr-$MODULE_VERSION"

echo "remove '$MODULE_LIB_PATH/lib$MODULE_NAME.dylib'"
rm "$MODULE_LIB_PATH/lib$MODULE_NAME.dylib"

echo "remove '$MODULE_LIB_PATH/lib$MODULE_NAME.so'"
rm "$MODULE_LIB_PATH/lib$MODULE_NAME.so"

echo "remove '$MODULE_LIB_PATH/lib$MODULE_NAME.a'"
rm "$MODULE_LIB_PATH/lib$MODULE_NAME.a"
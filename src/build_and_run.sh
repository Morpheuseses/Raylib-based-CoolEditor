#!/bin/sh
./build.sh
LD_LIBRARY_PATH=../deps/raygui-4.0/lib ../build/editor

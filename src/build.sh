#!/bin/sh

BUILDDIR='../build'

INCLUDE='-I../deps/raylib-5.5_linux_amd64/include -I../deps/raygui-4.0/src'
CFLAGS='-c -g'

LFLAGS='-L../deps/raylib-5.5_linux_amd64/lib -L../deps/raygui-4.0/lib'
LIBS='-lm -l:libraylib.a -l:libraygui.so'

OBJS='Editor.o Selector.o TransformOperation.o main.o'

echo "Compiling Editor.cpp..."
g++ ${CFLAGS} ${INCLUDE} Editor.cpp

echo "Compiling Selector.cpp..."
g++ ${CFLAGS} ${INCLUDE} Selector.cpp

echo "Compiling TransformOperation.cpp..."
g++ ${CFLAGS} ${INCLUDE} TransformOperation.cpp

echo "Compiling main.cpp..."
g++ ${CFLAGS} ${INCLUDE} main.cpp

echo "Linking..."
mkdir -p ${BUILDDIR}
g++ -o ${BUILDDIR}/editor ${LFLAGS} ${OBJS} ${LIBS}

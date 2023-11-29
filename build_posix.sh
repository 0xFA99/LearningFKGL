set -xe

CC=cc
CFLAGS="-Wall -Wextra -std=c11 -pedantic -g -I/usr/include/freetype2"
LIBS="-lglfw -lGLEW -lGL -lm -lfreetype"

$CC $CFLAGS -o main main.c mymath.c transform.c shader.c texture.c $LIBS

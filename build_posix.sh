set -xe

CC=cc
CFLAGS="-Wall -Wextra -std=c11 -pedantic -ggdb -I./include/"
LIBS="-lglfw -lGLEW -lGL -lm"

$CC $CFLAGS -o main main.c include/glowMath.c include/shader.c $LIBS

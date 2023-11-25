set -xe

CC=cc
CFLAGS="-Wall -Wextra -std=c11 -pedantic -ggdb"
LIBS="-lglfw -lGLEW -lGL -lm"

$CC $CFLAGS -o main main.c mymath.c transform.c shader.c $LIBS

#!/usr/bin/env bash
# Builds and runs a Graphics1300 program on macOS, Linux, or Git Bash on Windows.
#   Usage:  bash build.sh             (builds main.cpp)
#           bash build.sh demo.cpp    (builds demo.cpp)
#           bash build.sh swarm       (builds every .cpp file in the swarm folder)

SRC="${1:-main.cpp}"
SRC="${SRC%/}"
NAME="$(basename "$SRC")"
NAME="${NAME%.*}"

case "$(uname -s)" in
    Darwin)
        PLATFORM=mac
        LIBS="-framework OpenGL -framework Cocoa" ;;
    Linux)
        PLATFORM=linux
        LIBS="-lGLU -lGL -lX11" ;;
    MINGW*|MSYS*|CYGWIN*)
        PLATFORM=windows
        LIBS="-static -lopengl32 -lgdi32"
        NAME="$NAME.exe" ;;
    *)
        echo "Unknown system: $(uname -s)"; exit 1 ;;
esac

fail() {
    echo
    echo "Build FAILED - see the errors above."
    exit 1
}

# tigr.c is C code and only has to be compiled once; after that, builds are fast.
TIGR_OBJ="tigr-$PLATFORM.o"
if [ ! -f "$TIGR_OBJ" ]; then
    echo "Compiling tigr.c - only needed the first time..."
    g++ -x c -c tigr.c -o "$TIGR_OBJ" -Wno-deprecated-declarations || fail
fi

# Your program: one .cpp file, or every .cpp file in a folder.
if [ -d "$SRC" ]; then
    FILES=("$SRC"/*.cpp)
    RUNDIR="$SRC"
else
    FILES=("$SRC")
    RUNDIR=.
fi

# The Graphics1300 library: every .cpp file in the graphics folder.
g++ -std=c++17 -I. "${FILES[@]}" graphics/*.cpp "$TIGR_OBJ" -o "$RUNDIR/$NAME" $LIBS || fail

echo "Built $RUNDIR/$NAME - running it..."
cd "$RUNDIR" && ./"$NAME"

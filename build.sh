#!/bin/sh

# ARGUEMENTS; will clean up build types, platforms, etc, later
compileMode=$1

# OPTIMIZATION
opti="-O2"

# PATHS
buildDir="build/ChibiApplets.exe"
src="src/*.c"
includes="src/include"
libraries="src/lib"

# WARNINGS
warnings="-Wall -Wextra -pedantic"
ignores="-Wunused-variable"

# LINKING
linking="-lraylib -lgdi32 -lwinmm"




gcc $opti $ignores $warnings $src -o $buildDir -I $includes -L $libraries $linking 
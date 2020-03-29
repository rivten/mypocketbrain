
curDir=$(pwd)
buildDir="$curDir/../build"
dataDir="$curDir/../data"

CommonCompilerFlags="-g -ggdb -std=c99 -msse4.1 -ffast-math -Wno-braced-scalar-init -Wno-format -Wno-writable-strings -Wno-switch -Wno-unused-value"
CommonDefines="-DCOMPILE_INTERNAL=1 -DCOMPILE_SLOW=1 -DCOMPILE_LINUX=1"
CommonLinkerFlags="-pthread -lX11 -ldl -lGL -lraylib"

pushd $buildDir > /dev/null

echo "Building Unix..."
~/dev/zig/build/zig cc $CommonCompilerFlags $CommonDefines $curDir/brain.c -o brain $CommonLinkerFlags

popd > /dev/null

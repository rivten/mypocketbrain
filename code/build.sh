
curDir=$(pwd)
compileDir="$curDir/.."

ZigPath=/home/hugo/Téléchargements/zig-linux-x86_64-0.5.0+c70471fae/zig

pushd $compileDir > /dev/null

echo "Building Unix..."
$ZigPath build

popd > /dev/null

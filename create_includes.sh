rm -rf include/
rm -rf lib/
mkdir -p include
mkdir -p lib
cp src/*.h include
cp build/Release/*.lib lib

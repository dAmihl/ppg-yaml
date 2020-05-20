rm -rf include/
mkdir -p include
cp src/*.h include
cp build/Release/*.lib include

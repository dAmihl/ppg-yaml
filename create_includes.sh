rm -rf include/
mkdir -p include
mkdir -p lib
cp src/*.h include
cp build/Release/*.lib lib
cp includes/ppg-core/include/ppg-core.lib lib

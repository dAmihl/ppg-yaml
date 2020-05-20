rm -rf include/
rm -rf lib/
mkdir -p include
mkdir -p lib
cp src/*.h include
cp build/Release/*.lib lib
cp includes/ppg-core/lib/ppg-core.lib lib

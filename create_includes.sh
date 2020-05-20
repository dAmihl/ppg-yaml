rm -rf include/
rm -rf lib/
mkdir -p include
mkdir -p lib
cp src/*.h include
cp build/Release/*.lib lib
cp build/includes/thirdparty/yaml-cpp/Release/*.lib lib
cp build/includes/ppg-core/build_results/bin/Release/*.lib lib
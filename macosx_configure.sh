mkdir -p build/macosx
cd build/macosx
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DNESTK_USE_FREENECT=ON -DNESTK_USE_OPENNI=ON ../..

mkdir bin/
cmake -S . -B bin/ -Wno-dev
cmake --build bin --target ModelViewer --config Release
read
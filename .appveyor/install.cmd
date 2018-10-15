git clone https://github.com/SFML/SFML.git
cd SFML

mkdir install
mkdir build
cd build

cmake .. -G "Visual Studio 15 Win64" -DCMAKE_INSTALL_PREFIX=../install -DSFML_DEPENDENCIES_INSTALL_PREFIX=../install -DSFML_BUILD_FRAMEWORKS=FALSE -DBUILD_SHARED_LIBS=TRUE

msbuild INSTALL.vcxproj

cd ../../
cd TIMLE

mkdir build

mkdir "build/TIMLE"
echo "mkdir build/TIMLE"

echo "Process TIMLE by CMake started"
cmake . -G "Visual Studio 15 Win64" -DCMAKE_INSTALL_PREFIX=./build/TIMLE -DCMAKE_PREFIX_PATH=./SFML/install
echo "Process TIMLE by CMake ended"

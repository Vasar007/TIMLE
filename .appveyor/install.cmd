git clone https://github.com/Vasar007/TIMLE.git
cd SFML

mkdir install
mkdir build
cd build

cmake .. -G "Visual Studio 15 Win64" -DCMAKE_INSTALL_PREFIX=../install -DSFML_DEPENDENCIES_INSTALL_PREFIX=../install -DSFML_BUILD_FRAMEWORKS=FALSE -DBUILD_SHARED_LIBS=TRUE

msbuild INSTALL.vcxproj

cd ../../
git clone https://github.com/Vasar007/TIMLE.git
cd TIMLE

mkdir build
mkdir build/TIMLE

cmake . -G "Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION -DTARGET_CPU=$TARGET_CPU -DCMAKE_INSTALL_PREFIX=./build/TIMLE -DCMAKE_PREFIX_PATH=./SFML/install
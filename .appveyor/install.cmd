echo "Start build"
git clone https://github.com/SFML/SFML.git
cd SFML

mkdir install
mkdir build
cd build

cmake .. -G "Visual Studio 15 Win64" -DCMAKE_INSTALL_PREFIX=../install -DSFML_DEPENDENCIES_INSTALL_PREFIX=../install -DSFML_BUILD_FRAMEWORKS=FALSE -DBUILD_SHARED_LIBS=TRUE

msbuild INSTALL.vcxproj
echo "After SFML build"

cd ../../
git clone https://github.com/Vasar007/TIMLE.git
echo "After TIMPLE clonning"
cd TIMLE

mkdir build
mkdir build/TIMLE

cmake . -G "Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=$CONFIGURATION -DTARGET_CPU=$PLATFORM -DCMAKE_INSTALL_PREFIX=./build/TIMLE -DCMAKE_PREFIX_PATH=./SFML/install
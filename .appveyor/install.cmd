git clone https://github.com/SFML/SFML.git
cd SFML

mkdir install
mkdir build
cd build

cmake .. -G "Visual Studio 15 Win64" -DCMAKE_INSTALL_PREFIX=../install -DSFML_DEPENDENCIES_INSTALL_PREFIX=../install -DSFML_BUILD_FRAMEWORKS=FALSE -DBUILD_SHARED_LIBS=TRUE

msbuild INSTALL.vcxproj

cd ../../

mkdir build
cd build
mkdir TIMLE

cmake .. -G "Visual Studio 15 Win64" -DCMAKE_INSTALL_PREFIX=TIMLE -DCMAKE_PREFIX_PATH=../SFML/install

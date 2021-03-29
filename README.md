EXAMPLE-LIBRARY
================
An example C/C++ library. Uses CMake as build system and
[googletest](https://github.com/google/googletest) as testing framework.

## Build:
```
cd example-library
rm -r build
mkdir build && cd build
cmake ..
make
```

Installation command:
```
sudo make install
```
Success results:
```
[100%] Built target example-library
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/lib/libexample-library.so.0.1.0
-- Installing: /usr/local/lib/libexample-library.so
-- Installing: /usr/local/include/example-library
-- Installing: /usr/local/include/example-library/examplepublicclass.h
-- Installing: /usr/local/share/example-library/cmake/example-libraryConfig.cmake
-- Installing: /usr/local/share/example-library/cmake/example-libraryConfig-noconfig.cmake
-- Installing: /usr/local/lib/pkgconfig/example-library.pc
```

EXAMPLE-LIBRARY
================
An example C/C++ library. Uses CMake as build system and
[googletest](https://github.com/google/googletest) as testing framework.

## Build:
```
./script_compile.sh
```

Installation command:
```
cd build
sudo make install
```
Success output of installation:
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

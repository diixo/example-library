cmake_minimum_required(VERSION 3.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules)

# Project name and version
project(example-library-Prj VERSION 0.1.0 LANGUAGES CXX)


# Define GNU standard installation directories
include(GNUInstallDirs)

# C++ compiler flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project targets
set(TARGET ${PROJECT_NAME})

# Target source files
file(GLOB SOURCE_FILES      "src/*.cpp" "src/*.hpp")
file(GLOB PUBLIC_HEADERS    "include/*.hpp")

# Declare library
add_library(${TARGET} SHARED ${SOURCE_FILES} ${PUBLIC_HEADERS})
set_target_properties (${TARGET} PROPERTIES SOVERSION 0.1.0)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths.
target_include_directories(${TARGET} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE src)
#target_compile_features(${TARGET}
	#PUBLIC cxx_auto_type
	#PRIVATE cxx_variadic_templates)

install(TARGETS ${TARGET} EXPORT "${TARGET}Config"
	ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${TARGET})

install(EXPORT "${TARGET}Config" DESTINATION share/${TARGET}/cmake)

export(TARGETS ${TARGET} FILE "${TARGET}Config.cmake")

# The following pkg config file is for backwards compatibility
# Usually imported targets, like above should be used

# Use a pkg-config module for CMake
find_package(PkgConfig)
#find_package(Threads)

SET(CMAKE_EXE_LINKER_FLAGS "-pthread")
#target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

# Create pkgconfig file
configure_file(example-library.pc.in example-library.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/example-library.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

enable_testing()
add_subdirectory(tests)
add_subdirectory(itc)

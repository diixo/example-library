#!/bin/bash

for i in "$@"
do
    case $i in
        -j2|--set-2-threads) WITH_2_THREADS=TRUE ;;
        -j4|--set-4-threads) WITH_4_THREADS=TRUE ;;
        -j8|--set-8-threads) WITH_8_THREADS=TRUE ;;
        -jx|--set-max-threads) WITH_MAX_THREADS=TRUE ;;
        -ct|--with-component-tests) WITH_COMPONENT_TESTS=TRUE ;;
        -ut|--with-unit-tests) WITH_UNIT_TESTS=TRUE ;;
        -h|--help|-?) echo "Use keys:
                -j2|--set-2-threads - Compile binaries with 2 threads
                -j4|--set-4-threads - Compile binaries with 4 threads
                -j8|--set-8-threads - Compile binaries with 8 threads
                -jx|--set-max-threads - Compile binaries with max threads
                -ct|--with-component-tests - Compile binaries with component tests
                -ut|--with-unit-tests - Compile binaries with unit tests
            " && exit ;;
    esac
done

rm -rf build
mkdir build && cd build

CMAKE_PARAMS=""
nproc=4
if [[ -n ${WITH_2_THREADS} ]]; then
    nproc=2
elif [[ -n ${WITH_4_THREADS} ]]; then
    nproc=4
elif [[ -n ${WITH_8_THREADS} ]]; then
    nproc=8
elif [[ -n ${WITH_MAX_THREADS} ]]; then
    nproc=$(grep -c ^processor /proc/cpuinfo)
fi

if [[ -n ${WITH_COMPONENT_TESTS} ]]; then
    CMAKE_PARAMS+=" -DTEST_COMPONENT=TRUE"
fi
if [[ -n ${WITH_UNIT_TESTS} ]]; then
    CMAKE_PARAMS+=" -DTEST_UNIT=TRUE"
fi

echo "CMAKE_PARAMS: ${CMAKE_PARAMS}"
echo "nproc=${nproc}"
#echo "cmake=${BASH_ALIASES[cmake]}"
cmake ${CMAKE_PARAMS} ..
make -j${nproc}

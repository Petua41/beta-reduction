#!/bin/bash

function print_help() {
cat << EOF
Usage: ./build.sh [options]

Possible options:
  -h,         --help                  Display help
  -n,         --no-tests              Don't build tests
  -j[N],      --jobs[=N]              Allow N jobs at once (default [=1])
  -d,         --debug                 Set debug build type

EOF
}

for i in "$@"
    do
    case $i in
        -n|--no-tests) # Don't build tests
            NO_TESTS=true
            ;;
        -j*|--jobs=*) # Allow N jobs at once
            JOBS_OPTION=$i
            ;;
        -d|--debug) # Set debug build type
            DEBUG_MODE=true
            ;;
        -h|--help|*) # Display help
            print_help
            exit 0
            ;;
    esac
done

mkdir -p lib
cd lib

if [[ ! -d "easyloggingpp" ]] ; then
  git clone https://github.com/amrayn/easyloggingpp/ --branch v9.97.0 --depth 1
fi

if [[ $NO_TESTS == true ]]; then
  PREFIX="$PREFIX -D COMPILE_TESTS=OFF"
else
  if [[ ! -d "googletest" ]] ; then
    git clone https://github.com/google/googletest/ --branch v1.13.0 --depth 1
  fi
fi

if [[ $DEBUG_MODE != true ]]; then
  PREFIX="$PREFIX -D CMAKE_BUILD_TYPE=Release"
fi

cd ..
mkdir -p build
cd build
rm CMakeCache.txt
cmake $PREFIX .. && make $JOBS_OPTION

cd ..
ln -svf build/target/Beta-reduction_cli ./Beta-reduction_cli

if [[ $NO_TESTS != true ]]; then
  ln -svf build/target/Beta-reduction_test ./Beta-reduction_test
fi

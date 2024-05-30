#!/bin/bash

set -e

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


if [[ ! -d "vendor/easyloggingpp/src" ]] ; then
  git submodule update --init --recursive
fi

if [[ $DEBUG_MODE != true ]]; then
  PREFIX="$PREFIX -D CMAKE_BUILD_TYPE=Release"
fi

mkdir -p build
cd build
cmake $PREFIX .. && make $JOBS_OPTION

cd ..
ln -svf build/target/beta-red ./beta-red

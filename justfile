set shell := ["bash", "-cu"]

build_dir := "build"
build_type := "Debug"

configure:
    cmake -S . -B {{build_dir}} -DCMAKE_BUILD_TYPE={{build_type}}

build: configure
    cmake --build {{build_dir}}

run *args: build
    ./{{build_dir}}/ck {{args}}
set shell := ["bash", "-cu"]

build_dir := "build"
build_type := "Debug"

configure:
    cmake -S . -B {{build_dir}} -DCK_ENABLE_SANITIZERS=OFF -DCMAKE_BUILD_TYPE={{build_type}}

build: configure
    cmake --build {{build_dir}}

run *args: build
    ./{{build_dir}}/ck {{args}}

test filter="": build
    cd {{build_dir}} && ctest --output-on-failure {{ if filter != "" { "-R " + filter } else { "" }}} 
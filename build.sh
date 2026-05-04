#!/bin/bash

SOURCE_FILE="main.c"
OUTPUT_EXE="3d-renderer"

build(){
    echo "Comipling..."
    gcc -Wall -Wextra "$SOURCE_FILE" -o "$OUTPUT_EXE" -lgdi32 -luser32 -ld3d11 -ldxgi -luuid
}

clean(){
    echo "Cleaning up build artifacts ..."
    rm -rf "$OUTPUT_EXE" *.o
}

if [[ $# -gt 0 ]]; then
    case "$1" in
        clean)
            clean
            exit 0
            ;;
        *)
            echo "Usage: $0 [clean]"
            exit 1
            ;;
    esac
fi

build
echo "Build successful: ./$OUTPUT_EXE"

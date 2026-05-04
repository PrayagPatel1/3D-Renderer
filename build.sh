#!/bin/bash

SOURCE_FILE="main.c"
OUTPUT_EXE="3d-renderer"

gcc -Wall -Wextra "$SOURCE_FILE" -o "$OUTPUT_EXE" -lgdi32 -luser32 -ld3d11 -ldxgi -luuid

if [ $? -eq 0 ]; then
    echo "Build Successful. Running $OUTPUT_EXE..."
    ./"$OUTPUT_EXE"
else
    echo "Build Failed."
    exit 1
fi 
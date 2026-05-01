#!/bin/bash

SOURCE_FILE="main.c"
OUTPUT_EXE="3d-renderer"

gcc -Wall -Wextra "$SOURCE_FILE" -o "$OUTPUT_EXE"

if [ $? -eq 0 ]; then
    echo "Build Successful. Running $OUTPUT_EXE..."
    ./"$OUTPUT_EXE"
else
    echo "Build Failed."
    exit 1
fi 
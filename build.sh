#!/bin/bash

# Handle clean option
if [ "$1" == "clean" ]; then
    echo "Cleaning build directories..."
    rm -rf build
    rm -rf install
    echo "Clean complete."
    exit 0
fi

# Create directories if they don't exist
mkdir -p build
mkdir -p install

# Configure with install prefix
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=./install

# Build the project
cmake --build build

# Install to the install directory
cmake --install build

echo "Build complete! Executable is in the install/bin directory."

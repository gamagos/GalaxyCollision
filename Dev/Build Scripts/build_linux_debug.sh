#!/bin/bash

# (C) Sebastian Fiault

echo "[WARNING] This Script needs to be run from GalaxyCollision/Dev/Build Scripts/ to function properly!"
echo "Running from different locations will result in improper behavior" #TODO find way to make script automatically cd to it's location
echo " "

cd ../..
cmake --preset Linux-Debug --fresh
echo "==============================================================="
echo "CMake Configured. Build starts here"
echo "==============================================================="
cmake --build --preset Linux-Debug
cd "Dev/Build Scripts"

#TODO document this so that people can build it on GitHub

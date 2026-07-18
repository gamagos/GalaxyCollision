#!/bin/bash

# (C) Sebastian Fiault

echo "[WARNING] This Script needs to be run from GalaxyCollision/Dev/Build Scripts/ to funtion properly!\n Running from different locations will result in improper behaviour" #TODO find way to make script automatically cd to it's location

cd ../..
cmake --preset Linux-Debug --fresh
cmake --build --preset Linux-Debug
cd "Dev/Build Scripts"

#TODO document this so that people can build it on GitHub

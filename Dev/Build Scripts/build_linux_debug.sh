#! bin/bash

# (C) Sebastian Fiault

cmake -S "../../" --preset Linux-Debug --fresh

cmake --build --preset Linux-Debug

#TODO document this so that people can build it on GitHub

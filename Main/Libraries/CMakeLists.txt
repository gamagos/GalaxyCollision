# (C) Sebastian Fiault 2026

add_subdirectory("glfw-3.4") # glfw needs to be built first, because the other libraries depend on it
add_subdirectory("glad")
add_subdirectory("cglm-0.9.6")

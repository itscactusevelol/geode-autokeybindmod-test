cmake_minimum_required(VERSION 3.20)
project(auto_keybind VERSION 1.0.0)

# Find the Geode SDK
find_package(Geode REQUIRED)

# Add our source files
add_geode_mod(
    ${PROJECT_NAME}
    SHARED
    src/main.cpp
)

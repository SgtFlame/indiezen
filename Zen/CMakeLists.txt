cmake_minimum_required (VERSION 2.6)

if (NOT INDIEZEN_PARENT)
    find_path (INDIEZEN_PARENT Zen .. ../.. ../../.. ENV)
endif (NOT INDIEZEN_PARENT)

add_subdirectory ("${INDIEZEN_PARENT}/Zen/build/CMake")


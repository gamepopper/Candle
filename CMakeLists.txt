cmake_minimum_required(VERSION 3.5)

project(Candle)

# set C++17 standard
set(CMAKE_CXX_STANDARD 17)

# set output files directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)


# might need to set manually SFML_ROOT to the install location of SFML for Windows users
# set(SFML_ROOT "")
if(SFML_ROOT STREQUAL "")
	find_package(SFML 3.0 REQUIRED COMPONENTS Graphics)
endif()

# if the user wants to use static SFML libs
# set(SFML_STATIC_LIBRARIES TRUE)

find_path(SFML_INCLUDE_DIR SFML/Config.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_SFML_PATHS})

set(CANDLE_HEADERS
	include/Candle/LightingArea.hpp
	include/Candle/LightSource.hpp
	include/Candle/RadialLight.hpp
	include/Candle/DirectedLight.hpp
	include/Candle/geometry/Line.hpp
	include/Candle/geometry/Polygon.hpp
    include/Candle/geometry/Vector2.hpp
	include/Candle/graphics/Color.hpp
	include/Candle/graphics/VertexArray.hpp
	include/Candle/Constants.hpp
)

set(CANDLE_SRC
	src/LightingArea.cpp
	src/LightSource.cpp
	src/RadialLight.cpp
	src/DirectedLight.cpp
	src/Line.cpp
	src/Polygon.cpp
	src/Color.cpp
	src/VertexArray.cpp
	src/Constants.cpp
)

# Static library target
add_library(Candle-s STATIC ${CANDLE_SRC} ${CANDLE_HEADERS})
target_include_directories(Candle-s PUBLIC include)
target_include_directories(Candle-s PUBLIC ${SFML_INCLUDE_DIR})
target_link_libraries(Candle-s PUBLIC SFML::Graphics)
target_compile_features(Candle-s PUBLIC cxx_std_17)

option(RADIAL_LIGHT_FIX "Use RadialLight fix for errors with textures" OFF)

if(RADIAL_LIGHT_FIX)
	target_compile_definitions(Candle-s PUBLIC -DRADIAL_LIGHT_FIX)
endif()


# Demo target
option(BUILD_DEMO "Build demo application" OFF)

if (BUILD_DEMO)
	set(DEMO_SRC demo.cpp)
	add_executable(demo ${DEMO_SRC})
	target_include_directories(demo PRIVATE include)
	target_include_directories(demo PRIVATE ${SFML_INCLUDE_DIR})
	target_link_libraries(demo PRIVATE SFML::Graphics Candle-s)

endif()

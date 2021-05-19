cmake_minimum_required(VERSION 3.15)
project(lab_unicode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FIND_DEBUG_MODE 1)

# Project source compilation
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/inc HEADERS)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCES)

add_executable(lab_unicode main.cpp ${SOURCES} ${HEADERS})
add_executable(utf8_ops utf8_ops.cpp)

# Add external packages
find_package(ICU REQUIRED COMPONENTS uc i18n)

set (Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set (Boost_USE_MULTITHREAD ON)  # enable multithreading
find_package(Boost REQUIRED COMPONENTS
        iostreams filesystem locale system program_options)

include_directories(${ICU_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_link_libraries(lab_unicode ${ICU_LIBRARIES} ${Boost_LIBRARIES})
cmake_minimum_required(VERSION 3.10)

project(lab07)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)


add_library(${CMAKE_PROJECT_NAME}_lib 
    src/npc.cpp
    src/elf.cpp
    src/knight_errant.cpp  
    src/rogue.cpp
    src/tech_impl.cpp
    src/fightManager.cpp
)


target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC include)


add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_PROJECT_NAME}_lib)


enable_testing()

add_executable(tests tests/test_npcs.cpp)
target_link_libraries(tests PRIVATE ${CMAKE_PROJECT_NAME}_lib gtest_main)
target_include_directories(tests PRIVATE include)
add_test(NAME NPCtesting COMMAND tests)
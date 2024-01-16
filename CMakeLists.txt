cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") 
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES 
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif() 

project(project)


# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package (glog 0.6.0 REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_compile_options(-g -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG)
add_link_options(-g -Wall -Wextra -Wshadow -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG)
add_executable(main src/main.cpp ${SRC_FILES})
target_link_libraries(main glog::glog)
target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


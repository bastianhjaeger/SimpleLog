cmake_minimum_required (VERSION 3.10.2)
project (SimpleLogExample)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
  main.cpp
)
target_link_libraries(${PROJECT_NAME}
  pthread)
target_include_directories(${PROJECT_NAME}
  PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}/../..)


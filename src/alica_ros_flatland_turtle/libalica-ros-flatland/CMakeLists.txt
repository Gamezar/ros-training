cmake_minimum_required(VERSION 3.5.1)
project(libalica-ros-flatland)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED
  src/WaitForTrigger.cpp
  src/WaitForMsg.cpp
)
target_link_libraries(${PROJECT_NAME} alica_engine)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

install(TARGETS ${PROJECT_NAME}
  EXPORT ${CMAKE_PROJECT_NAME}Targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}/
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

cmake_minimum_required(VERSION 2.8.8)

project(math_test C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Threads REQUIRED)

add_subdirectory(gtest)

enable_testing()

set(RMCD_DIRECTORY ${PROJECT_SOURCE_DIR}/../rmcd)
set(RMCD_TESTED_FILES
  ${RMCD_DIRECTORY}/primes.cpp
  ${RMCD_DIRECTORY}/primes.h
  )

file(GLOB TEST_SRC_FILES *.cpp)

include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES} ${RMCD_DIRECTORY})

add_executable(${PROJECT_NAME}
  ${TEST_SRC_FILES}
  ${RMCD_TESTED_FILES}
  )

add_dependencies(${PROJECT_NAME} googletest)
add_definitions(-DGTEST_INVOKED)

if(NOT WIN32 OR MINGW)
  target_link_libraries(${PROJECT_NAME}
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    )
else()
  target_link_libraries(${PROJECT_TEST_NAME}
    debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
    )
  target_link_libraries(${PROJECT_TEST_NAME}
    debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
    optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
    )
endif()

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_test(${PROJECT_NAME} ${PROJECT_NAME})

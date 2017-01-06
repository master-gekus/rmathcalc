project(rmcd)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(../thrift/rmathcalc.thrift.cmake)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${RMC_THRIFT_GENERATED})
LING_AGAINST_THRITH(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} boost_system boost_thread boost_chrono)


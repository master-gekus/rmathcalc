project(rmc)
cmake_minimum_required(VERSION 3.0)

# get git dscription
execute_process(COMMAND git describe --long --always
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                RESULT_VARIABLE res
                OUTPUT_VARIABLE GIT_DESCRIBE
                ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
               )
if(NOT res EQUAL 0)
    message(FATAL_ERROR "Get GIT_DESCRIBE failed, error: ${res}")
endif()
add_definitions("-DGIT_DESCRIBE=\"${GIT_DESCRIBE}\"")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include(../thrift/rmathcalc.thrift.cmake)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${RMC_THRIFT_GENERATED})
LING_AGAINST_THRITH(${PROJECT_NAME})

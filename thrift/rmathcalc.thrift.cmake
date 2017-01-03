set(RMC_THRIFT_SOURCE ${CMAKE_CURRENT_LIST_DIR}/rmathcalc.thrift)

set(RMC_THRIFT_GENERATED
  gen-cpp/MathCalc.cpp
  gen-cpp/MathCalc.h
  gen-cpp/rmathcalc_constants.cpp
  gen-cpp/rmathcalc_constants.h
  gen-cpp/rmathcalc_types.cpp
  gen-cpp/rmathcalc_types.h
  )

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
  OUTPUT ${RMC_THRIFT_GENERATED}
  COMMAND thrift -gen cpp ${RMC_THRIFT_SOURCE}
  COMMENT "Generating CPP files from ${RMC_THRIFT_SOURCE}..."
  DEPENDS ${RMC_THRIFT_SOURCE}
  )

macro(LING_AGAINST_THRITH name)
  target_link_libraries(${name} thrift crypto)
endmacro(LING_AGAINST_THRITH)

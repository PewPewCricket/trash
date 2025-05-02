file(
  GLOB_RECURSE 
  PREGEN_HEADERS 
  CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.in"
)

foreach(PREGEN_HEADER ${PREGEN_HEADERS})
  string(
    REPLACE
    ".in"
    ""
    GEN_HEADER
    ${PREGEN_HEADER}
  )

  string(
    REPLACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    GEN_HEADER
    ${GEN_HEADER}
  )

  configure_file(
    ${PREGEN_HEADER}
    ${GEN_HEADER}
    @ONLY
  )

endforeach()
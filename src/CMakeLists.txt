add_library( annieobj OBJECT
  # Append each .cc file
  Annie.cc
  GeoANNIEFactory.cc
)

target_include_directories( annieobj PUBLIC
  $<BUILD_INTERFACE:${ANNIE_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)

file(COPY include/ DESTINATION ${ANNIE_INCLUDE_DIR})

add_executable(annie annie.cpp)
target_link_libraries(annie PUBLIC 
  annieobj ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${Ratpac_LIBRARIES})

set_target_properties(annieobj annie
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${ANNIE_LIBRARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${ANNIE_LIBRARY_DIR})

install(TARGETS annieobj annie
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)

install(DIRECTORY ${ANNIE_INCLUDE_DIR}
    DESTINATION ${CMAKE_INSTALL_PREFIX})

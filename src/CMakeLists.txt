# ANNRI-Gd gamma generator (from ANNRIGdFeb2019), used by NeutronCapture.cc
file(GLOB ANNRIGD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ANNRIGd/*.cc)

add_library( annieobj OBJECT
  # Append each .cc file
  Annie.cc
  GeoANNIEFactory.cc
  OutANNIEClusterProc.cc
  ANNIEInnerStructureMembers.cc
  NeutronCapture.cc
  ${ANNRIGD_SOURCES}
)

target_include_directories( annieobj PUBLIC
  $<BUILD_INTERFACE:${ANNIE_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>)
target_include_directories( annieobj PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ANNRIGd)

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

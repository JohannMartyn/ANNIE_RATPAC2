#add_library( annieobj OBJECT
  #GeoEosFactory.cc
#)

#target_include_directories( annieobj PUBLIC
#  $<BUILD_INTERFACE:${ANNIE_INCLUDE_DIR}>
#  $<INSTALL_INTERFACE:include>)

file(COPY include/ DESTINATION ${ANNIE_INCLUDE_DIR})

add_executable(annie annie.cpp)
target_link_libraries(annie PUBLIC 
  #annieobj
  ${ROOT_LIBRARIES} ${Geant4_LIBRARIES} ${Ratpac_LIBRARIES})

install(TARGETS annie RUNTIME DESTINATION bin)


cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

project(AnalysisPayload)

find_package(ROOT)
find_package(AnalysisBase)

message(${ROOT_CXX_FLAGS})

add_executable(AnalysisPayload AnalysisPayload.cxx)

target_compile_definitions(AnalysisPayload
  PRIVATE ${ROOT_DEFINITIONS}
)

include_directories(
  ${ROOT_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/JetSelectionHelper/JetSelectionHelper
)

add_library( JetSelectionHelper 
  SHARED JetSelectionHelper/src/JetSelectionHelper.cxx JetSelectionHelper/JetSelectionHelper/JetSelectionHelper.h
)

target_link_libraries(JetSelectionHelper
  PRIVATE ${ROOT_LIBRARIES}
  AnalysisBase::xAODEventInfo
  AnalysisBase::xAODRootAccess
  AnalysisBase::xAODJet
)


target_link_libraries(AnalysisPayload
  PRIVATE ${ROOT_LIBRARIES}
  AnalysisBase::xAODEventInfo
  AnalysisBase::xAODRootAccess
  AnalysisBase::xAODJet
  JetSelectionHelper
)

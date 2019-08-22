# The name of the package
ATLAS_SUBDIR(AnalysisPayload)

# Add binary
ATLAS_ADD_EXECUTABLE ( AnalysisPayload util/AnalysisPayload.cxx
                     LINK_LIBRARIES  xAODEventInfo
                                     xAODRootAccess
                                     xAODJet
                                     JetSelectionHelperLib
                                     AsgTools
                                     JetCalibToolsLib)


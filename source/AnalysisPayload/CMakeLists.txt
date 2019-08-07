# The name of the package
ATLAS_SUBDIR(AnalysisPayload)

# External Dependencies
FIND_PACKAGE( ROOT )

# Add binary
ATLAS_ADD_EXECUTABLE ( AnalysisPayload utils/AnalysisPayload.cxx
		     INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
		     LINK_LIBRARIES ${ROOT_LIBRARIES}
		     		     xAODEventInfo
				     xAODRootAccess
				     xAODJet
				     AsgTools
				     JetCalibToolsLib
				     JetSelectionHelperLib)


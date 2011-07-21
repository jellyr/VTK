INCLUDE(vtkDependentOption)

SET(KIT Chemistry)
SET(UKIT CHEMISTRY)

SET(KIT_LIBS vtkRendering vtkFiltering vtkVolumeRendering)

SET(Kit_SRCS
  vtkAbstractElectronicData.cxx
  vtkBlueObeliskData.cxx
  vtkBlueObeliskDataParser.cxx
  vtkCMLMoleculeReader.cxx
  vtkMoleculeAlgorithm.cxx
  vtkMoleculeMapper.cxx
  vtkMoleculeToAtomBallFilter.cxx
  vtkMoleculeToBondStickFilter.cxx
  vtkMoleculeToPolyDataFilter.cxx
  vtkPeriodicTable.cxx
  vtkProgrammableElectronicData.cxx
  vtkSimpleBondPerceiver.cxx
  )

SET_SOURCE_FILES_PROPERTIES(
  vtkAbstractElectronicData.cxx
  vtkMoleculeToPolyDataFilter.cxx
  ABSTRACT
  )

# OpenQube setup
FIND_PACKAGE(OpenQube QUIET)
IF(OPENQUBE_FOUND)
  FIND_PACKAGE(Eigen2 QUIET)
ENDIF(OPENQUBE_FOUND)
VTK_DEPENDENT_OPTION(VTK_USE_OPENQUBE
                     "Compile OpenQube support into vtkChemistry."
                     ON "OPENQUBE_FOUND;EIGEN2_FOUND" OFF)
IF(VTK_USE_OPENQUBE)
  INCLUDE_DIRECTORIES(${OPENCUBE_INCLUDE_DIR} ${EIGEN2_INCLUDE_DIR})
  SET(KIT_LIBS ${KIT_LIBS} ${OPENQUBE_LIBRARIES})
  SET(Kit_SRCS
    ${Kit_SRCS}
    vtkOpenQubeElectronicData.cxx
    vtkOpenQubeMoleculeSource.cxx
  )
ENDIF(VTK_USE_OPENQUBE)

#-----------------------------------------------------------------------------
# Include CMake code common to all kits.
INCLUDE(${VTK_CMAKE_DIR}/KitCommonBlock.cmake)
#-----------------------------------------------------------------------------

# Install Blue Obelisk Data Repository files
INSTALL(DIRECTORY vtkBODRData
  DESTINATION ${VTK_INSTALL_SHARE_DIR_CM24}/vtkChemistry
  )

# Generate configured header file
CONFIGURE_FILE(${VTK_SOURCE_DIR}/Chemistry/vtkChemistryConfigure.h.in
               ${VTK_BINARY_DIR}/Chemistry/vtkChemistryConfigure.h IMMEDIATE)

# TODO We don't really want to add an eigen dep, do we? We should be
# able to modify openqube to make Eigen optional. For now I've
# hardcoded the include dir in. This will obviously need to change ;-)
INCLUDE_DIRECTORIES( "/usr/include/eigen2/" )

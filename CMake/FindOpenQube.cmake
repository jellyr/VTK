#
# Find the native OpenQube includes and library
#
# This module defines
# OPENQUBE_INCLUDE_DIR, where to find openqube/*.h
# OPENQUBE_LIBRARIES, the libraries to link against to use OpenQube.
# OPENQUBE_FOUND, If false, do not try to use OpenQube.

FIND_PATH(OPENQUBE_INCLUDE_DIR openqube/openqubeabi.h)

FIND_LIBRARY(OPENQUBE_LIBRARY OpenQube)

IF(OPENQUBE_INCLUDE_DIR)
  IF(OPENQUBE_LIBRARY)
    SET( OPENQUBE_FOUND "YES" )
    SET( OPENQUBE_LIBRARIES ${OPENQUBE_LIBRARY} )
  ENDIF(OPENQUBE_LIBRARY)
ENDIF(OPENQUBE_INCLUDE_DIR)

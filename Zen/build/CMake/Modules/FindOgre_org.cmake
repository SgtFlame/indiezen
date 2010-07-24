# Find the OGRE headers and library
#
#  OGRE_INCLUDE_DIR - location of Ogre.h
#  OGRE_LIBRARIES   - List of libraries to link to when using Ogre
#  OGRE_FOUND       - True if Ogre was found.


IF (OGRE_INCLUDE_DIR)
  	SET(OGRE_FIND_QUIETLY TRUE)
ENDIF (OGRE_INCLUDE_DIR)

IF (OIS_INCLUDE_DIR)
    SET(OIS_FIND_QUIETLY TRUE)
ENDIF(OIS_INCLUDE_DIR)

FIND_PATH(OGRE_INCLUDE_DIR Ogre.h
	HINTS 
	${DEPENDENCIES_HOME}/ogre/include/OGRE
	${INDIEZEN_PARENT}/ogre/include/OGRE
	${DEPENDENCIES_HOME}/ogre/OgreMain/include
	/usr/local/include/OGRE
	/usr/include/OGRE
)

FIND_PATH(OIS_INCLUDE_DIR OIS/OIS.h
    HINTS
	${DEPENDENCIES_HOME}/ogre/include
	${INDIEZEN_PARENT}/ogre/include
    ${DEPENDENCIES_HOME}/ogre/Dependencies/include
)

FIND_LIBRARY(OGRE_LIBRARY OgreMain 
	HINTS
	${DEV_LIB}
	${INDIEZEN_PARENT}/ogre/include/OGRE
	/usr/local/lib
	/usr/lib
)

FIND_LIBRARY(OIS_LIBRARY OIS
    HINTS
    ${DEV_LIB}
    /usr/lib
)

# handle the QUIETLY and REQUIRED arguments and set OGRE_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OGRE DEFAULT_MSG OGRE_LIBRARY OGRE_INCLUDE_DIR)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OIS DEFAULT_MSG OIS_LIBRARY OIS_INCLUDE_DIR)

IF(OGRE_FOUND)
  SET( OGRE_LIBRARIES ${OGRE_LIBRARY} )
ELSE(OGRE_FOUND)
  SET( OGRE_LIBRARIES )
ENDIF(OGRE_FOUND)

IF(OIS_FOUND)
    SET( OIS_LIBRARIES ${OIS_LIBRARY} )
ELSE(OIS_FOUND)
    SET( OIS_LIBRARIES )
ENDIF(OIS_FOUND)

MARK_AS_ADVANCED( OGRE_LIBRARY OGRE_INCLUDE_DIR )

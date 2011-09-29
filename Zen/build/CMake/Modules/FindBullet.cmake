# Find the Open Dynamics Engine headers and library
#
#  BULLET_INCLUDE_DIR - location of Ogre.h
#  BULLET_LIBRARIES   - List of libraries to link to when using Ogre
#  BULLET_FOUND       - True if Ogre was found.
# BULLET_DEFINITIONS - OpenDE definitions

include(FindPkgMacros)
include(PreprocessorUtils)

IF (BULLET_INCLUDE_DIR)
  	SET(BULLET_FIND_QUIETLY TRUE)
ENDIF (BULLET_INCLUDE_DIR)

#try using PkgConfig first
use_pkgconfig(BULLET bullet)

# If that didn't work, do it manually
if (NOT BULLET_FOUND)
	FIND_PATH(BULLET_INCLUDE_DIR ode
		HINTS 
		${INDIEZEN_PARENT}/bullet/src
		${DEPENDENCIES_HOME}/bullet/src
		/usr/include
	)

	FIND_LIBRARY(BULLET_RELEASE_LIBRARY bullet
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/bullet/lib
		${DEPENDENCIES_HOME}/bullet/lib
		/usr/lib
	)

	FIND_LIBRARY(BULLET_DEBUG_LIBRARY ode_singled
		HINTS
		${DEV_LIB}
		${INDIEZEN_PARENT}/opende/lib/debugsingledll
		${DEPENDENCIES_HOME}/opende/lib/debugsingledll
		/usr/lib
	)

	if (BULLET_INCLUDE_DIR AND BULLET_RELEASE_LIBRARY)
		set(BULLET_FOUND "True")
		set(BULLET_LIBRARIES debug ${BULLET_DEBUG_LIBRARY} optimized ${BULLET_RELEASE_LIBRARY} )
		set(BULLET_DEFINITIONS "-DdSINGLE")
	else(BULLET_INCLUDE_DIR AND BULLET_RELEASE_LIBRARY)
		set(BULLET_LIBRARIES "BULLET-NOTFOUND")
		set(BULLET_FOUND "False")
	endif (BULLET_INCLUDE_DIR AND BULLET_RELEASE_LIBRARY)

endif(NOT BULLET_FOUND)




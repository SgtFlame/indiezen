# - Find VLD
# Find the native VLD includes and library
# This module defines
#  VLD_INCLUDE_DIR, where to find VLDlib.h, etc.
#  VLD_LIBRARIES, the libraries needed to use VLD.
#  VLD_FOUND, If false, do not try to use VLD.
# also defined, but not for general use are
#  VLD_LIBRARY, where to find the VLD library.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)
IF(WIN32)
	FIND_PATH(VLD_ROOT_DIR vld.ini
		PATHS
		"C:/Program Files/Visual Leak Detector"
	)
	#message("VLD Root Dir = ${VLD_ROOT_DIR}")

	FIND_PATH(VLD_INCLUDE_DIR vld.h
		PATHS
		"${VLD_ROOT_DIR}/include"
	)
	#message("VLD Include Dir = ${VLD_INCLUDE_DIR}")

	FIND_PATH(VLD_LIB_DIR vld.lib
		PATHS
		"${VLD_ROOT_DIR}/lib/Win32"
	)
	#message("VLD Library Dir = ${VLD_LIB_DIR}")
	
	FIND_PATH(VLD_BIN_DIR vld_x86.dll
		PATHS
		"${VLD_ROOT_DIR}/bin/Win32"
	)
	#message("VLD Binary Dir = ${VLD_BIN_DIR}")
		
# handle the QUIETLY and REQUIRED arguments and set VLD_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VLD DEFAULT_MSG VLD_INCLUDE_DIR)

IF(VLD_FOUND)
		find_library(VLD_LIBRARY vld
			  PATHS
			  ${VLD_LIB_DIR}
		)
	SET(VLD_LIBRARIES ${VLD_LIBRARIES} ${VLD_LIBRARY})
	#MESSAGE("VLD Libraries = ${VLD_LIBRARIES}")
ENDIF(VLD_FOUND)

# Deprecated declarations.
SET (NATIVE_VLD_INCLUDE_PATH ${VLD_INCLUDE_DIR} )
IF(VLD_LIBRARY)
  GET_FILENAME_COMPONENT (NATIVE_VLD_LIB_PATH ${VLD_LIBRARY} PATH)
ENDIF(VLD_LIBRARY)

MARK_AS_ADVANCED(VLD_LIBRARY VLD_INCLUDE_DIR )

ENDIF(WIN32)

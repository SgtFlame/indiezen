# - Find PostgreSQL
# Find the PostgreSQL includes and client library
# This module defines
#  POSTGRESQL_INCLUDE_DIR, where to find POSTGRESQL.h
#  POSTGRESQL_LIBRARIES, the libraries needed to use POSTGRESQL.
#  POSTGRESQL_FOUND, If false, do not try to use PostgreSQL.
#
# Add the postgresql and mysql include paths here

#  find_path(POSTGRESQL_INCLUDE_DIR libpq-fe.h

 FIND_PATH(POSTGRESQL_INCLUDE_DIR libpq-fe.h
      /usr/include
      /usr/include/postgresql
      /usr/include/pgsql
      /usr/local/include/pgsql
      /usr/include/postgresql
      /usr/include/postgresql/*
      /usr/local/include/postgresql
      /usr/local/include/postgresql/*
      $ENV{ProgramFiles}/PostgreSQL/*/include
      $ENV{SystemDrive}Program Files/PostgreSQL/*/include
      )
#message("POSTGRESQL_INCLUDE_DIR: ${POSTGRESQL_INCLUDE_DIR}")
      
  find_library(POSTGRESQL_LIBRARIES NAMES libpq pq
      PATHS
      /usr/lib
      /usr/local/lib
      /usr/lib/postgresql
      /usr/lib64
      /usr/local/lib64
      /usr/lib64/postgresql
      $ENV{ProgramFiles}/PostgreSQL/*/lib
      $ENV{SystemDrive}/PostgreSQL/*/lib
      )
#message("POSTGRESQL_LIBRARIES: ${POSTGRESQL_LIBRARIES}")
      
  if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND TRUE)
    #message(STATUS "Found PostgreSQL: ${POSTGRESQL_INCLUDE_DIR}, ${POSTGRESQL_LIBRARIES}")
    INCLUDE_DIRECTORIES(${POSTGRESQL_INCLUDE_DIR})
  else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND FALSE)
    message(STATUS "PostgreSQL not found.")
  endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

  mark_as_advanced(POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES)

# ENDMACRO(FIND_PostgreSQL)

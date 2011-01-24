cmake_minimum_required (VERSION 2.6) 

project (ZPostgres)
set ( PROJECT_PREFIX "plugins/ZPostgres" )

if ( NOT INDIEZEN_PARENT)
	find_path ( INDIEZEN_PARENT Zen .. ../.. ../../.. ENV )
endif (NOT INDIEZEN_PARENT)

set(CMAKE_MODULE_PATH "${INDIEZEN_PARENT}/Zen/build/CMake/Modules" ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
list (REMOVE_DUPLICATES CMAKE_MODULE_PATH)

Find_Package(IndieZen REQUIRED)
Find_Package(Boost REQUIRED )
Find_Package(PostgreSql REQUIRED)

SET (LIB_TYPE SHARED)

#-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
# Include ZPostgres source file

source_group(
    src\\
	REGULAR_EXPRESSION 
	src/*.*
	)


	
set ( ZPostgres_SRC
    CommandQueue.hpp
    I_Command.hpp
    src/DatabaseColumn.cpp
    src/DatabaseColumn.hpp
    src/DatabaseCommand.cpp
    src/DatabaseCommand.hpp
    src/DatabaseConnection.cpp
    src/DatabaseConnection.hpp
    src/DatabaseDynamicQuery.cpp
    src/DatabaseDynamicQuery.hpp
    src/DatabaseQuery.cpp
    src/DatabaseQuery.hpp
    src/DatabaseResult.cpp
    src/DatabaseResult.hpp
    src/DatabaseRow.cpp
    src/DatabaseRow.hpp
    src/DatabaseService.cpp
    src/DatabaseService.hpp
    src/DatabaseServiceFactory.cpp
    src/DatabaseServiceFactory.hpp
    src/DatabaseStaticQuery.cpp
    src/DatabaseStaticQuery.hpp
    src/DatabaseTransaction.cpp
    src/DatabaseTransaction.hpp
    src/PostgresTypes.cpp
    src/PostgresTypes.hpp
    src/ZPostgresPlugin.cpp

    )

add_definitions( -DZPOSTGRES_EXPORTS ${Zen_DEFINITIONS})

include_directories ( ${Boost_INCLUDE_DIR} ${INDIEZEN_PARENT} ${POSTGRESQL_INCLUDE_DIR} )

link_directories ( ${DEV_LIB} )

add_library ( ZPostgres ${LIB_TYPE} ${ZPostgres_SRC} )

add_dependencies(ZPostgres ZenCore ZenEnterprise )

target_link_libraries( ZPostgres ZenEnterprise ${POSTGRESQL_LIBRARIES})

set_target_properties ( ZPostgres 
						PROPERTIES
						DEBUG_POSTFIX "_d"
					)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    set_target_properties ( ZPostgres PROPERTIES PREFIX "../../bin/" )
	set_target_properties ( ZPostgres PROPERTIES IMPORT_PREFIX "../../lib/" )
endif(MSVC_IDE)


install ( FILES 
    CommandQueue.hpp
    I_Command.hpp
    plugin.xml
    DESTINATION
    Zen/${PROJECT_PREFIX}/
)


install ( TARGETS
	ZPostgres
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
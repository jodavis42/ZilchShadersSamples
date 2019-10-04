set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_executable(SimpleDemo1 "")

include(${CurrentDirectory}/CMakeFiles.cmake)

target_include_directories(SimpleDemo1
    PUBLIC 
        ${CurrentDirectory}
)

Set_Common_TargetCompileOptions(SimpleDemo1)

target_link_libraries(SimpleDemo1
                      PUBLIC
                      Math
                      Common
                      ZilchShaders
                      Engine
                      DemoUtilities
)

set_target_properties(SimpleDemo1 PROPERTIES FOLDER "Applications")

get_target_property(ShaderCoreDir ShaderCorePackage IMPORTED_LOCATION)
get_target_property(ProjectsDir ProjectsPackage IMPORTED_LOCATION)
configure_file(BuildConfig.data BuildConfig.data ESCAPE_QUOTES) 

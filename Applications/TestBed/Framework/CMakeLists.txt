set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_executable(Framework "")

include(${CurrentDirectory}/CMakeFiles.cmake)

target_include_directories(Framework
    PUBLIC 
        ${CurrentDirectory}
)

#set_target_properties(Framework PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CurrentDirectory}/")
Set_Common_TargetCompileOptions(Framework)

get_target_property(SDL2_DLL_Path SDL2_DLL IMPORTED_LOCATION)

target_link_libraries(Framework
                      PUBLIC
                      SDL2
                      ImGui
                      Math
                      Common
                      ZilchShaders
                      OpenGL
                      GLU
                      Glew32
                      Graphics
                      OpenGLApi
)

#find_library(SDL2_DLL SDL2.dll PATHS ../../../)
add_custom_command(TARGET Framework POST_BUILD   # Adds a post-build event to MyTest
            COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
            ${SDL2_DLL_Path}#${CurrentDirectory}/lib/x86/SDL2.dll                                         # <--this is in-file
            $<TARGET_FILE_DIR:Framework>/SDL2.dll           # <--this is out-file path
        )     
        
set_target_properties(Framework PROPERTIES FOLDER "Applications")

get_target_property(ShaderCoreDir ShaderCorePackage IMPORTED_LOCATION)
get_target_property(ProjectsDir ProjectsPackage IMPORTED_LOCATION)

configure_file(BuildConfig.data BuildConfig.data ESCAPE_QUOTES) 

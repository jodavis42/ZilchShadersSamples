set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(OpenGLApi "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)
target_sources(OpenGLApi
    PRIVATE
    ${manualAdditions}
)

target_include_directories(OpenGLApi
    PUBLIC
    ${CurrentDirectory}
    ${LibrariesDir}
)

ADD_MSVC_PRECOMPILED_HEADER(OpenGLApi "Precompiled.hpp" "Precompiled.cpp")
Set_Common_TargetCompileOptions(OpenGLApi)

target_link_libraries(OpenGLApi
                      PUBLIC
                      Graphics
                      Glew32
)

set_target_properties(OpenGLApi PROPERTIES FOLDER "Libraries")

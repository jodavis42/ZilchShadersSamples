set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(ImGui "")

include(${CurrentDirectory}/CMakeFiles.cmake)

target_include_directories(ImGui
    PUBLIC 
        ${CurrentDirectory}
)

set_target_properties(ImGui PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CurrentDirectory}/")
Set_Common_TargetCompileOptions(ImGui)
add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH)
target_link_libraries(ImGui
                      PUBLIC
                      SDL2
                      Glew32
)

set_target_properties(ImGui PROPERTIES FOLDER "Libraries")
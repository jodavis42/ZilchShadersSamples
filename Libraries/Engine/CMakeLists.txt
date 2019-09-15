set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(Engine "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)
target_sources(Engine
    PRIVATE
    ${manualAdditions}
)

target_include_directories(Engine
    PUBLIC
    ${CurrentDirectory}
    ${LibrariesDir}
)

ADD_MSVC_PRECOMPILED_HEADER(Engine "Precompiled.hpp" "Precompiled.cpp")
Set_Common_TargetCompileOptions(Engine)

target_link_libraries(Engine
                      PUBLIC
                      Zilch
)

set_target_properties(Engine PROPERTIES FOLDER "Libraries")

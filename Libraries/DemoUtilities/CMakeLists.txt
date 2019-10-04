set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(DemoUtilities "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)
target_sources(DemoUtilities
    PRIVATE
    ${manualAdditions}
)

target_include_directories(DemoUtilities
    PUBLIC
    ${CurrentDirectory}
    ${LibrariesDir}
)

ADD_MSVC_PRECOMPILED_HEADER(DemoUtilities "Precompiled.hpp" "Precompiled.cpp")
Set_Common_TargetCompileOptions(DemoUtilities)

target_link_libraries(DemoUtilities
                      PUBLIC
                      Zilch
                      ZilchShaders
                      Engine
)

set_target_properties(DemoUtilities PROPERTIES FOLDER "Libraries")

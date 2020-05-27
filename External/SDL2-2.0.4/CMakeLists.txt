set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

set(platform "x64")
if(${CMAKE_VS_PLATFORM_NAME} STREQUAL "Win32")
  set(platform "x86")
endif()

add_library(SDL2 STATIC IMPORTED GLOBAL)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${CurrentDirectory}/lib/${platform}/SDL2.lib)
target_include_directories(SDL2 INTERFACE ${CurrentDirectory} ${CurrentDirectory}/include)

add_library(SDL2_DLL SHARED IMPORTED GLOBAL)
set_target_properties(SDL2_DLL PROPERTIES IMPORTED_LOCATION ${CurrentDirectory}/lib/${platform}/SDL2.dll)
#target_include_directories(SDL2_DLL INTERFACE ${CurrentDirectory} ${CurrentDirectory}/include)

#find_file (Local_SDL2_DLL SDL2.dll PATHS  ${CurrentDirectory}/lib/x86)
#set_property(GLOBAL PROPERTY SDL2_DLL  ${CurrentDirectory}/lib/x86/SDL2.dll)
#message("Local" ${Local_SDL2_DLL})
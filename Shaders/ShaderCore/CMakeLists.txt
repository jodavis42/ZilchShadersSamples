set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(ShaderCorePackage SHARED IMPORTED GLOBAL)
set_target_properties(ShaderCorePackage PROPERTIES IMPORTED_LOCATION ${CurrentDirectory})


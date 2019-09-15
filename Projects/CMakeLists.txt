set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(ProjectsPackage SHARED IMPORTED GLOBAL)
set_target_properties(ProjectsPackage PROPERTIES IMPORTED_LOCATION ${CurrentDirectory})


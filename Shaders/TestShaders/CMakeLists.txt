set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(TestShadersPackage SHARED IMPORTED GLOBAL)
set_target_properties(TestShadersPackage PROPERTIES IMPORTED_LOCATION ${CurrentDirectory})


target_sources(Framework
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/FrameworkStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SDL.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SDL.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiHelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiHelper.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Application.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Mouse.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Mouse.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Keyboard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Keyboard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Camera.hpp

    ${CMAKE_CURRENT_LIST_DIR}/ObjReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ObjReader.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Properties.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Properties.hpp

    ${CMAKE_CURRENT_LIST_DIR}/Serializer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serializer.hpp
    ${CMAKE_CURRENT_LIST_DIR}/JsonSerializer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JsonSerializer.hpp
)

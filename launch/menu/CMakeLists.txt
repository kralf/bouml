remake_project_get(EXECUTABLE_DESTINATION DESTINATION OUTPUT EXECUTABLE_PATH)
remake_project_get(ICON_DESTINATION DESTINATION OUTPUT ICON_PATH)

remake_add_configurations(*.remake INSTALL ${CMAKE_INSTALL_PREFIX}/share/menu)

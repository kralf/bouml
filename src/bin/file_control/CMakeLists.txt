remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(file_control)
remake_add_executable(
  file_control *.cpp
  PREFIX OFF
  LINK ${QT_LIBRARIES}
  INSTALL ${PLUGIN_DESTINATION}
)

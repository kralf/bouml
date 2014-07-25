remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(file_control)
remake_add_executable(
  file_control *.cpp
  PREFIX OFF
  LINK ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY}
  INSTALL ${PLUGIN_DESTINATION}
)

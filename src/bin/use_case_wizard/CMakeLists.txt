remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(usecasewizard)
remake_add_executable(
  usecasewizard *.cpp
  PREFIX OFF
  LINK ${QT_LIBRARIES}
  INSTALL ${PLUGIN_DESTINATION}
)

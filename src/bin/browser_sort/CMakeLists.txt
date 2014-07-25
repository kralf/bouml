remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(browsersort)
remake_add_executable(
  browsersort *.cpp
  PREFIX OFF
  LINK ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY}
  INSTALL ${PLUGIN_DESTINATION}
)

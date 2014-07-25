remake_define(WITHCPP 1)
remake_define(WITHJAVA 1)
remake_define(WITHIDL 1)
remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(irose)
remake_add_executable(
  irose *.cpp
  PREFIX OFF
  LINK ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY}
  INSTALL ${PLUGIN_DESTINATION}
)

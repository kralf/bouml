remake_define(WITHCPP 1)
remake_define(WITHJAVA 1)
remake_define(WITHIDL 1)
remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(gxmi)
remake_add_executable(
  gxmi *.cpp
  PREFIX OFF
  LINK ${QT_LIBRARIES}
  INSTALL ${PLUGIN_DESTINATION}
)

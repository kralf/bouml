remake_define(WITHCPP 1)
remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()

remake_qt4_moc(cpp_util)
remake_add_executable(
  cpp_util *.cpp
  PREFIX OFF
  LINK ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY}
  INSTALL ${PLUGIN_DESTINATION}
)

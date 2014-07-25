remake_include()

remake_qt4_moc(project_control)
remake_add_executable(
  project_control *.cpp *.xpm
  LINK ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY}
)

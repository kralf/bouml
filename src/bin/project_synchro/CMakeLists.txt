remake_include()

remake_qt4_moc(project_synchro)
remake_add_executable(
  project_synchro *.cpp *.xpm
  LINK ${QT_LIBRARIES}
)

remake_qt4(REQUIRED QtCore QtGui Qt3Support QtNetwork)
remake_qt4_include()
remake_set(QT_LIBRARIES ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY})


remake_define(BooL bool)
remake_define(QT3_SUPPORT 1)

remake_add_directories(lib)
remake_add_directories(bin)
remake_add_directories(
  plugouts
  COMPONENT plugouts-src
)

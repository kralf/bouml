remake_find_package(Qt4 REQUIRED Qt3Support QtNetwork)
remake_qt4_include()

remake_define(BooL bool)
remake_define(QT3_SUPPORT 1)

remake_add_directories(lib)
remake_add_directories(bin)
remake_add_directories(
  plugouts
  COMPONENT plugouts-src
)

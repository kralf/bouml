remake_include(../../lib/*)

remake_add_executable(bouml PREFIX OFF
  LINK browser data diagram dialog misc tool xpm
    ${QT_QT3SUPPORT_LIBRARY} ${QT_QTNETWORK_LIBRARY}
)

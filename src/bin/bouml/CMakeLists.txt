remake_include(../../lib/*)

remake_add_executable(
  bouml
  PREFIX OFF
  LINK browser data diagram dialog misc tool xpm ${QT_LIBRARIES}
)

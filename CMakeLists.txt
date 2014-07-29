cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  BOUML
  VERSION 4.21
  RELEASE free
  SUMMARY "UML2 tool box to specify and generate code."
  AUTHOR "Bruno Pages"
  AUTHOR "Ralf Kaestner"
  CONTACT "bouml@free.fr"
  HOME "http://bouml.sourceforge.net"
  LICENSE "GNU General Public License (GPL)"
  INSTALL /usr
)

remake_add_directories(
  lang
  icons
  launch
)
remake_doc(
  man INSTALL share
  html
)

remake_pack_deb(
  DEPENDS libqt4-qt3support libqt4-network
  EXTRA_COMPONENTS doc
)
remake_pack_deb(
  COMPONENT plugouts-src
  DESCRIPTION "plugouts sources"
  DEPENDS bouml
)

remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "development headers"
  DEPENDS bouml libqt4-dev
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION devel
  UPLOAD ppa:kralf/asl
  DEPENDS remake libqt4-dev pkg-config docbook-utils
  PASS CMAKE_BUILD_TYPE BOUML_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION precise
  SECTION devel
  UPLOAD ppa:kralf/asl
  DEPENDS remake libqt4-dev pkg-config docbook-utils
  PASS CMAKE_BUILD_TYPE BOUML_GIT_REVISION
)
remake_distribute_deb(
  DISTRIBUTION trusty
  SECTION devel
  UPLOAD ppa:kralf/asl
  DEPENDS remake libqt4-dev pkg-config docbook-utils
  PASS CMAKE_BUILD_TYPE BOUML_GIT_REVISION
)

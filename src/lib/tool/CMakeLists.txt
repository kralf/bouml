remake_project_get(PLUGIN_DESTINATION DESTINATION OUTPUT PLUGIN_PATH)
remake_project_header(Config.h.remake)

remake_qt4_moc(tool)
remake_add_library(tool LINK browser)
remake_add_headers()

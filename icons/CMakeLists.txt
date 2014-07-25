remake_project_set(ICON_DESTINATION "share/icons"
  CACHE STRING "Install destination of project icons.")
remake_set(bouml_icon_sizes 16 32 48 64)

remake_project_get(ICON_DESTINATION DESTINATION)

foreach(bouml_icon_size ${bouml_icon_sizes})
  remake_set(bouml_icon_install 
    "${ICON_DESTINATION}/hicolor/${bouml_icon_size}x${bouml_icon_size}/apps")
  
  remake_component_install(
    FILES bouml.${bouml_icon_size}.png
    DESTINATION ${bouml_icon_install}
    RENAME bouml.png)
  remake_component_install(
    FILES project_control.${bouml_icon_size}.png
    DESTINATION ${bouml_icon_install}
    RENAME bouml-project_control.png)
  remake_component_install(
    FILES project_synchro.${bouml_icon_size}.png
    DESTINATION ${bouml_icon_install}
    RENAME bouml-project_synchro.png)
endforeach(bouml_icon_size)

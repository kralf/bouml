remake_define(WITHCPP 1)
remake_define(WITHJAVA 1)
remake_define(WITHPHP 1)
remake_define(WITHIDL 1)
remake_define(WITHPYTHON 1)
remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()
remake_include(../../lib/uml)

remake_qt4_moc(plug_out_upgrade)
remake_add_executable(
  plug_out_upgrade *.cpp
    ../../lib/uml/UmlBaseClassItem.cpp
    ../../lib/uml/CppSettings.cpp
    ../../lib/uml/IdlSettings.cpp
    ../../lib/uml/JavaSettings.cpp
    ../../lib/uml/PhpSettings.cpp
    ../../lib/uml/PythonSettings.cpp
    ../../lib/uml/UmlBaseArtifact.cpp
    ../../lib/uml/UmlBaseAttribute.cpp
    ../../lib/uml/UmlBaseClass.cpp
    ../../lib/uml/UmlBaseClassDiagram.cpp
    ../../lib/uml/UmlBaseClassMember.cpp
    ../../lib/uml/UmlBaseExtraClassMember.cpp
    ../../lib/uml/UmlBaseClassView.cpp
    ../../lib/uml/UmlBaseCollaborationDiagram.cpp
    ../../lib/uml/UmlBaseComponent.cpp
    ../../lib/uml/UmlBaseComponentDiagram.cpp
    ../../lib/uml/UmlBaseComponentView.cpp
    ../../lib/uml/UmlBaseDeploymentDiagram.cpp
    ../../lib/uml/UmlBaseDeploymentView.cpp
    ../../lib/uml/UmlBaseDiagram.cpp
    ../../lib/uml/UmlBaseItem.cpp
    ../../lib/uml/UmlBaseNode.cpp
    ../../lib/uml/UmlBaseOperation.cpp
    ../../lib/uml/UmlBasePackage.cpp
    ../../lib/uml/UmlBaseRelation.cpp
    ../../lib/uml/UmlBaseNcRelation.cpp
    ../../lib/uml/UmlBaseSequenceDiagram.cpp
    ../../lib/uml/UmlBaseUseCase.cpp
    ../../lib/uml/UmlBaseUseCaseDiagram.cpp
    ../../lib/uml/UmlBaseUseCaseView.cpp
    ../../lib/uml/UmlCom.cpp
    ../../lib/uml/UmlSettings.cpp
    ../../lib/uml/UmlBaseChoicePseudoState.cpp
    ../../lib/uml/UmlBaseDeepHistoryPseudoState.cpp
    ../../lib/uml/UmlBaseEntryPointPseudoState.cpp
    ../../lib/uml/UmlBaseExitPointPseudoState.cpp
    ../../lib/uml/UmlBaseFinalState.cpp
    ../../lib/uml/UmlBaseForkPseudoState.cpp
    ../../lib/uml/UmlBaseInitialPseudoState.cpp
    ../../lib/uml/UmlBaseJoinPseudoState.cpp
    ../../lib/uml/UmlBaseJunctionPseudoState.cpp
    ../../lib/uml/UmlBaseShallowHistoryPseudoState.cpp
    ../../lib/uml/UmlBaseStateAction.cpp
    ../../lib/uml/UmlBaseState.cpp
    ../../lib/uml/UmlBaseStateDiagram.cpp
    ../../lib/uml/UmlBaseTerminatePseudoState.cpp
    ../../lib/uml/UmlBaseRegion.cpp
    ../../lib/uml/StateBehavior.cpp
    ../../lib/uml/UmlBaseTransition.cpp
    ../../lib/uml/TransitionBehavior.cpp
    ../../lib/uml/UmlBaseAccessVariableValueAction.cpp
    ../../lib/uml/UmlBaseOnSignalAction.cpp
    ../../lib/uml/UmlBasePinParameter.cpp
    ../../lib/uml/UmlBaseActivityObject.cpp
    ../../lib/uml/UmlBaseActivityAction.cpp
    ../../lib/uml/UmlBaseActivityPin.cpp
    ../../lib/uml/UmlBaseActivityRegion.cpp
    ../../lib/uml/UmlBaseActivityPartition.cpp
    ../../lib/uml/UmlBaseActivityControlNodeClasses.cpp
    ../../lib/uml/UmlBaseActivityActionClasses.cpp
    ../../lib/uml/UmlBaseExpansionNode.cpp
    ../../lib/uml/UmlBaseActivityDiagram.cpp
    ../../lib/uml/UmlBaseFlow.cpp
    ../../lib/uml/UmlBaseParameterSet.cpp
    ../../lib/uml/UmlBaseActivityParameter.cpp
    ../../lib/uml/UmlBaseObjectDiagram.cpp
    ../../lib/uml/UmlBaseActualParameter.cpp
    ../../lib/uml/UmlBaseFormalParameter.cpp
    ../../lib/uml/UmlBaseExpansionRegion.cpp
    ../../lib/uml/UmlBaseActivity.cpp
    ../../lib/uml/UmlBaseInterruptibleActivityRegion.cpp
    ../../lib/uml/FlowBehavior.cpp
    ../../lib/uml/UmlBaseTypeSpec.cpp
    ../../lib/uml/UmlBaseParameter.cpp
    ../../lib/uml/UmlBaseView.cpp
    ../../lib/uml/UmlBaseClassInstance.cpp
  PREFIX OFF
  LINK ${QT_LIBRARIES}
  INSTALL ${PLUGIN_DESTINATION}
)

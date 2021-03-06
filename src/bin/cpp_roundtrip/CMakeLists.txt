remake_define(WITHCPP 1)
remake_define(REVERSE 1)
remake_define(ROUNDTRIP 1)
remake_project_get(PLUGIN_DESTINATION DESTINATION)

remake_include()
remake_include(../../lib/uml ../cpp_reverse)

remake_qt4_moc(cpp_roundtrip)
remake_add_executable(
  cpp_roundtrip *.cpp
    ../cpp_reverse/UmlClassItem.cpp
    ../cpp_reverse/UmlAttribute.cpp
    ../cpp_reverse/UmlArtifact.cpp
    ../cpp_reverse/UmlClass.cpp
    ../cpp_reverse/UmlClassDiagram.cpp
    ../cpp_reverse/UmlClassMember.cpp
    ../cpp_reverse/UmlExtraClassMember.cpp
    ../cpp_reverse/UmlClassView.cpp
    ../cpp_reverse/UmlCollaborationDiagram.cpp
    ../cpp_reverse/UmlComponent.cpp
    ../cpp_reverse/UmlComponentDiagram.cpp
    ../cpp_reverse/UmlComponentView.cpp
    ../cpp_reverse/UmlDeploymentDiagram.cpp
    ../cpp_reverse/UmlDeploymentView.cpp
    ../cpp_reverse/UmlDiagram.cpp
    ../cpp_reverse/UmlItem.cpp
    ../cpp_reverse/UmlNode.cpp
    ../cpp_reverse/UmlOperation.cpp
    ../cpp_reverse/UmlPackage.cpp
    ../cpp_reverse/UmlRelation.cpp
    ../cpp_reverse/UmlNcRelation.cpp
    ../cpp_reverse/UmlSequenceDiagram.cpp
    ../cpp_reverse/UmlUseCase.cpp
    ../cpp_reverse/UmlUseCaseDiagram.cpp
    ../cpp_reverse/UmlUseCaseView.cpp
    ../cpp_reverse/UmlChoicePseudoState.cpp
    ../cpp_reverse/UmlDeepHistoryPseudoState.cpp
    ../cpp_reverse/UmlEntryPointPseudoState.cpp
    ../cpp_reverse/UmlExitPointPseudoState.cpp
    ../cpp_reverse/UmlFinalState.cpp
    ../cpp_reverse/UmlForkPseudoState.cpp
    ../cpp_reverse/UmlInitialPseudoState.cpp
    ../cpp_reverse/UmlJoinPseudoState.cpp
    ../cpp_reverse/UmlJunctionPseudoState.cpp
    ../cpp_reverse/UmlShallowHistoryPseudoState.cpp
    ../cpp_reverse/UmlStateAction.cpp
    ../cpp_reverse/UmlState.cpp
    ../cpp_reverse/UmlStateDiagram.cpp
    ../cpp_reverse/UmlTerminatePseudoState.cpp
    ../cpp_reverse/UmlRegion.cpp
    ../cpp_reverse/UmlAccessVariableValueAction.cpp
    ../cpp_reverse/UmlOnSignalAction.cpp
    ../cpp_reverse/UmlPinParameter.cpp
    ../cpp_reverse/UmlActivityObject.cpp
    ../cpp_reverse/UmlActivityAction.cpp
    ../cpp_reverse/UmlActivityPin.cpp
    ../cpp_reverse/UmlActivityRegion.cpp
    ../cpp_reverse/UmlActivityControlNodeClasses.cpp
    ../cpp_reverse/UmlActivityActionClasses.cpp
    ../cpp_reverse/UmlExpansionNode.cpp
    ../cpp_reverse/UmlActivityDiagram.cpp
    ../cpp_reverse/UmlFlow.cpp
    ../cpp_reverse/UmlParameterSet.cpp
    ../cpp_reverse/UmlActivityParameter.cpp
    ../cpp_reverse/UmlObjectDiagram.cpp
    ../cpp_reverse/UmlActualParameter.cpp
    ../cpp_reverse/UmlFormalParameter.cpp
    ../cpp_reverse/UmlExpansionRegion.cpp
    ../cpp_reverse/UmlActivity.cpp
    ../cpp_reverse/UmlInterruptibleActivityRegion.cpp
    ../cpp_reverse/UmlTypeSpec.cpp
    ../cpp_reverse/UmlClassInstance.cpp
    ../cpp_reverse/Class.cpp
    ../cpp_reverse/Package.cpp
    ../cpp_reverse/BrowserNode.cpp
    ../cpp_reverse/Namespace.cpp
    ../cpp_reverse/Lex.cpp
    ../cpp_reverse/ClassContainer.cpp
    ../cpp_reverse/Progress.cpp
    ../../lib/uml/UmlBaseClassItem.cpp
    ../../lib/uml/CppSettings.cpp
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

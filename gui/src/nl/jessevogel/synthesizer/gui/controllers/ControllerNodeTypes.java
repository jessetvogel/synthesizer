package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.ChoiceBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.data.NodeType;
import nl.jessevogel.synthesizer.structure.controllers.NodeTypes;

public class ControllerNodeTypes {

    @FXML public ChoiceBox<String> groups;
    @FXML public VBox types;

    @FXML public void initialize() {
        // Store pointer to this controller
        GUI.controllerNodeTypes = this;

        // Selection element for node type groups
        NodeTypes nodeTypes = GUI.controller.getNodeTypes();
        groups.getItems().addAll(nodeTypes.getGroups());
        groups.setOnAction(event -> {
            // Set node types
            String group = groups.getValue();
            types.getChildren().clear();
            for(NodeType nodeType : nodeTypes.getTypes()) {
                if(!nodeType.group.equals(group)) continue;
                Pane pane = ListItems.createDraggable(nodeType.name, "create\n" + nodeType.name);
                types.getChildren().add(pane);
            }
        });
    }
}

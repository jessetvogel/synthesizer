package nl.jessevogel.synthesizer.gui.controllers;

import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.ChoiceBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.info.NodeType;
import nl.jessevogel.synthesizer.structure.info.NodeTypes;

public class ControllerNodeTypes {

    @FXML public ChoiceBox<String> groups;
    @FXML public VBox nodes;

    @FXML public void initialize() {
        // Store pointer to this controller
        GUI.controllerNodeTypes = this;

        // Selection element for node type groups
        NodeTypes nodeTypes = GUI.controller.getNodeTypes();
        groups.getItems().addAll(nodeTypes.groups);
        groups.setOnAction(event -> {

            // Set node types
            String group = groups.getValue();
            for(NodeType nodeType : nodeTypes.types) {
                if(!nodeType.group.equals(group)) continue;
                Pane pane = ListItems.createDraggable(nodeType.name, "create " + nodeType.name);
                nodes.getChildren().add(pane);
            }
        });
    }
}

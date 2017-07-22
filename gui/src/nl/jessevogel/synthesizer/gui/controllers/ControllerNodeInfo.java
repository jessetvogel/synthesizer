package nl.jessevogel.synthesizer.gui.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.gui.util.Confirm;
import nl.jessevogel.synthesizer.structure.data.Node;
import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.data.NodeOutput;
import nl.jessevogel.synthesizer.structure.response.NodeInfo;

import java.util.List;

public class ControllerNodeInfo {

    @FXML public VBox info;

    private String currentNode;

    @FXML public void initialize() {
        GUI.controllerNodeInfo = this;
    }

    public void setInfo(NodeInfo nodeInfo) {
        try {
            info.getChildren().clear();
            if(nodeInfo == null) {
                currentNode = null;
                return;
            }

            // Node id
            Pane id = ListItems.createTextField("id", nodeInfo.id, event -> {
                TextField textField = (TextField) event.getSource();
                if(GUI.controller.getNodes().rename(nodeInfo.id, textField.getText())) {
                    textField.getStyleClass().remove("invalid");
                    nodeInfo.id = textField.getText();
                }
                else
                    textField.getStyleClass().add("invalid");
            });
            info.getChildren().add(id);

            // Node inputs
            info.getChildren().add(ListItems.createHeader("Inputs"));
            for(NodeInput nodeInput : nodeInfo.inputs) {
                Pane pane = ListItems.createTextField(nodeInput.label, nodeInput.value, event -> {
                    TextField textField = (TextField) event.getSource();
                    if(GUI.controller.getNodes().set(nodeInfo.id, nodeInput.label, textField.getText()))
                        textField.getStyleClass().remove("invalid");
                    else
                        textField.getStyleClass().add("invalid");
                });
                info.getChildren().add(pane);
            }

            // Node outputs
            info.getChildren().add(ListItems.createHeader("Outputs"));
            for(NodeOutput nodeOutput : nodeInfo.outputs) {
                Pane pane = ListItems.createLabel(nodeOutput.label);
                info.getChildren().add(pane);
            }

            // Delete button
            Pane delete = ListItems.createButton("Delete", event -> {
                if(Confirm.show("Are you sure you want to delete node '" + currentNode + "'?", "This action cannot be undone.")) {
                    Node node = GUI.controller.getNodes().getNode(currentNode);
                    GUI.controller.getNodes().delete(node);
                }
            });
            info.getChildren().add(delete);

            // Set current node id
            currentNode = nodeInfo.id;
        }
        catch(Exception e) { e.printStackTrace(); /* TODO */ }
    }
}

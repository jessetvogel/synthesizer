package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.data.NodeOutput;
import nl.jessevogel.synthesizer.structure.response.NodeInfo;

public class ControllerNodeInfo {

    @FXML public VBox info;

    @FXML public void initialize() {
        GUI.controllerNodeInfo = this;
    }

    public void setInfo(NodeInfo nodeInfo) {
        try {
            info.getChildren().clear();

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
        }
        catch(Exception e) { e.printStackTrace(); /* TODO */ }
    }
}

package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.response.NodeInfo;

public class ControllerNodeInfo {

    @FXML public VBox nodeInfoContainer;
    @FXML public VBox nodeInputContainer;
    @FXML public Label labelId;

    @FXML public void initialize() {
        GUI.controllerNodeInfo = this;
    }

    public void setInfo(NodeInfo nodeInfo) {
        try {
            labelId.setText(nodeInfo.id);

            nodeInputContainer.getChildren().clear();
            for (NodeInput nodeInput : nodeInfo.inputs) {
                HBox hbox = FXMLLoader.load(getClass().getResource("/nodes_input.fxml"));

                ((Label) hbox.getChildren().get(0)).setText(nodeInput.label);

                TextField field = (TextField) hbox.getChildren().get(1);
                field.setText(nodeInput.value);
                field.setOnAction(event -> {
                    boolean success = GUI.controller.getNodes().set(nodeInfo.id, nodeInput.label, field.getText());
                    if(success)
                        field.getStyleClass().remove("invalid");
                    else
                        field.getStyleClass().add("invalid");
                });

                nodeInputContainer.getChildren().add(hbox);
            }
        }
        catch(Exception e) { e.printStackTrace(); /* TODO */ }
    }
}

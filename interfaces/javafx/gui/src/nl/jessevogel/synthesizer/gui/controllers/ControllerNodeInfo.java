package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.gui.util.Confirm;
import nl.jessevogel.synthesizer.structure.data.Node;
import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.data.NodeOutput;
import nl.jessevogel.synthesizer.structure.data.Option;

public class ControllerNodeInfo {

    @FXML public VBox info;

    private String currentNode;

    @FXML public void initialize() {
        GUI.controllerNodeInfo = this;
    }

    public void setInfo(Node node) {
        try {
            info.getChildren().clear();
            if(node == null) {
                currentNode = null;
                return;
            }

            // Type
            Pane type = ListItems.createHeader(node.type.name);
            info.getChildren().add(type);

            // Options
            for(Option option : node.options) {
                Pane item = ListItems.createLabelValue(option.description, option.value);
                info.getChildren().add(item);
            }

            info.getChildren().add(ListItems.createHr());

            // Node id
            Pane id = ListItems.createTextField("id", node.id, event -> {
                TextField textField = (TextField) event.getSource();
                if(GUI.controller.getNodes().rename(node.id, textField.getText()))
                    textField.getStyleClass().remove("invalid");
                else
                    textField.getStyleClass().add("invalid");
            });
            info.getChildren().add(id);

            // Node inputs
            info.getChildren().add(ListItems.createHeader("Inputs"));
            for(NodeInput nodeInput : node.inputs) {
                Pane pane = createListItem(node, nodeInput);
                info.getChildren().add(pane);
            }

            // Node outputs
            info.getChildren().add(ListItems.createHeader("Outputs"));
            for(NodeOutput nodeOutput : node.outputs) {
                Pane pane = ListItems.createLabel(nodeOutput.label);
                info.getChildren().add(pane);
            }

            info.getChildren().add(ListItems.createHr());

            // Delete button
            Pane delete = ListItems.createButton("Delete", event -> {
                if(Confirm.show("Are you sure you want to delete node '" + currentNode + "'?", "This action cannot be undone."))
                    GUI.controller.getNodes().delete(GUI.controller.getNodes().getNode(currentNode));
            });
            info.getChildren().add(delete);

            // Set current node id
            currentNode = node.id;
        }
        catch(Exception e) { e.printStackTrace(); /* TODO */ }
    }

    private Pane createListItem(Node node, NodeInput nodeInput) {
        Pane pane = ListItems.createTextField(nodeInput.label, nodeInput.value, event -> {
            TextField textField = (TextField) event.getSource();
            if(GUI.controller.getNodes().set(node.id, nodeInput.label, textField.getText()))
                textField.getStyleClass().remove("invalid");
            else
                textField.getStyleClass().add("invalid");
        });
        return pane;
    }
}

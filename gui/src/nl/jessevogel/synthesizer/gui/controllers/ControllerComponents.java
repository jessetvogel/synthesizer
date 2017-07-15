package nl.jessevogel.synthesizer.gui.controllers;

import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.input.*;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.structure.info.NodeType;
import nl.jessevogel.synthesizer.structure.info.NodeTypes;
import nl.jessevogel.synthesizer.structure.data.Parameter;

public class ControllerComponents {
    @FXML public ChoiceBox<String> choiceBoxComponentGroup;
    @FXML public ListView<String> listViewComponents;
    @FXML public VBox parametersContainer;

    @FXML public void initialize() {
        GUI.controllerComponents = this;
        NodeTypes components = GUI.controller.getNodeTypes();
        choiceBoxComponentGroup.getItems().addAll(components.groups);
        choiceBoxComponentGroup.setOnAction(event -> {
            String group = choiceBoxComponentGroup.getValue();
            ObservableList<String> list = listViewComponents.getItems();
            list.clear();
            for(NodeType nodeType : components.types) {
                if(!nodeType.group.equals(group)) continue;
                list.add(nodeType.name);
            }
        });
    }

    @FXML public void listDragDetected(MouseEvent event) {
        Dragboard db = listViewComponents.startDragAndDrop(TransferMode.ANY);
        ClipboardContent content = new ClipboardContent();
        content.putString(listViewComponents.getSelectionModel().getSelectedItem());
        db.setContent(content);
        db.setDragView(new Image("/img/components/template.png"));
        event.consume();
    }

    public void setParameters(Parameter[] parameters) {
        try {
            parametersContainer.getChildren().clear();
            for (Parameter parameter : parameters) {
                HBox hbox = FXMLLoader.load(getClass().getResource("/fxml/nodes_parameters.fxml"));
                ((Label) hbox.getChildren().get(0)).setText(parameter.label);
                ((TextField) hbox.getChildren().get(2)).setText(parameter.value);
                parametersContainer.getChildren().add(hbox);
            }
        }
        catch(Exception e) { e.printStackTrace(); /* TODO */ }
    }
}

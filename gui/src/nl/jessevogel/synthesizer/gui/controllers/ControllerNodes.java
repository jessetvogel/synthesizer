package nl.jessevogel.synthesizer.gui.controllers;

import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ListView;
import javafx.scene.image.Image;
import javafx.scene.input.*;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.structure.info.NodeType;
import nl.jessevogel.synthesizer.structure.info.NodeTypes;

public class ControllerNodes {

    @FXML public ChoiceBox<String> choiceBoxComponentGroup;
    @FXML public ListView<String> listViewComponents;

    @FXML public void initialize() {
        GUI.controllerNodes = this;
        NodeTypes nodeTypes = GUI.controller.getNodeTypes();
        choiceBoxComponentGroup.getItems().addAll(nodeTypes.groups);
        choiceBoxComponentGroup.setOnAction(event -> {
            String group = choiceBoxComponentGroup.getValue();
            ObservableList<String> list = listViewComponents.getItems();
            list.clear();
            for(NodeType nodeType : nodeTypes.types) {
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
}

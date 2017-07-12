package nl.jessevogel.synthesizer.gui.controllers;

import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.*;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.shape.Rectangle;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.synth.Components;
import nl.jessevogel.synthesizer.synth.Parameter;

public class ControllerComponents {
    @FXML public ChoiceBox<String> choiceBoxComponentGroup;
    @FXML public ListView<String> listViewComponents;
    @FXML public Pane viewComponents;
    @FXML public VBox parametersContainer;

    @FXML public void initialize() {
        GUI.controllerComponents = this;
        Components components = GUI.controller.getComponents();
        choiceBoxComponentGroup.getItems().addAll(components.groups);
        choiceBoxComponentGroup.setOnAction(event -> {
            String group = choiceBoxComponentGroup.getValue();
            ObservableList<String> list = listViewComponents.getItems();
            list.clear();
            for(Components.Component component : components.items) {
                if(!component.group.equals(group)) continue;
                list.add(component.name);
            }
        });

        Rectangle clipRect = new Rectangle(viewComponents.getWidth(), viewComponents.getHeight());
        clipRect.widthProperty().bind(viewComponents.widthProperty());
        clipRect.heightProperty().bind(viewComponents.heightProperty());
        viewComponents.setClip(clipRect);
    }

    @FXML public void viewDragOver(DragEvent event) {
        if(event.getDragboard().hasString())
            event.acceptTransferModes(TransferMode.ANY);
    }

    @FXML public void viewDragDropped(DragEvent event) {
        try {
            String type = event.getDragboard().getString();
//            System.out.println("type: " + type);
            StackPane stack = FXMLLoader.load(getClass().getResource("/fxml/component.fxml"));
            try {
                Image image = new Image("/img/components/" + type + ".png");
                ((ImageView) stack.getChildren().get(1)).setImage(image);
            }
            catch(Exception e) { }

            double x = Math.floor(event.getX() / 64) * 64;
            double y = Math.floor(event.getY() / 64) * 64;
            stack.setTranslateX(x);
            stack.setTranslateY(y);

            stack.setOnMouseClicked(e -> {
                Parameter[] parameters = new Parameter[2];
                parameters[0] = new Parameter(); parameters[0].label = "label_1"; parameters[0].value = "value_1";
                parameters[1] = new Parameter(); parameters[1].label = "label_2"; parameters[1].value = "value_2";
                setParameters(parameters);
            });

            viewComponents.getChildren().add(stack);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    @FXML public void listDragDetected(MouseEvent event) {
        Dragboard db = listViewComponents.startDragAndDrop(TransferMode.ANY);
        ClipboardContent content = new ClipboardContent();
        content.putString(listViewComponents.getSelectionModel().getSelectedItem());
        db.setContent(content);
        db.setDragView(new Image("/img/components/template.png"));
        event.consume();
    }

    @FXML public void viewMouseClicked() {
        viewComponents.requestFocus();
    }

    public void shiftView(double dx, double dy) {
        for(Node node : viewComponents.getChildren()) {
            node.setTranslateX(node.getTranslateX() + dx);
            node.setTranslateY(node.getTranslateY() + dy);
        }
    }

    public void setParameters(Parameter[] parameters) {
        try {
            parametersContainer.getChildren().clear();
            for (Parameter parameter : parameters) {
                HBox hbox = FXMLLoader.load(getClass().getResource("/fxml/parameter.fxml"));
                ((Label) hbox.getChildren().get(0)).setText(parameter.label);
                ((TextField) hbox.getChildren().get(2)).setText(parameter.value);
                parametersContainer.getChildren().add(hbox);
            }
        }
        catch(Exception e) { e.printStackTrace(); /* TODO */ }
    }
}

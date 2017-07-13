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
import nl.jessevogel.synthesizer.synth.data.Components;
import nl.jessevogel.synthesizer.synth.info.ComponentTypes;
import nl.jessevogel.synthesizer.synth.data.Parameter;

public class ControllerComponents {
    @FXML public ChoiceBox<String> choiceBoxComponentGroup;
    @FXML public ListView<String> listViewComponents;
    @FXML public Pane viewComponents;
    @FXML public VBox parametersContainer;

    public double viewShiftX;
    public double viewShiftY;

    @FXML public void initialize() {
        GUI.controllerComponents = this;
        ComponentTypes components = GUI.controller.getComponentTypes();
        choiceBoxComponentGroup.getItems().addAll(components.groups);
        choiceBoxComponentGroup.setOnAction(event -> {
            String group = choiceBoxComponentGroup.getValue();
            ObservableList<String> list = listViewComponents.getItems();
            list.clear();
            for(ComponentTypes.ComponentType componentType : components.types) {
                if(!componentType.group.equals(group)) continue;
                list.add(componentType.name);
            }
        });

        Rectangle clipRect = new Rectangle(viewComponents.getWidth(), viewComponents.getHeight());
        clipRect.widthProperty().bind(viewComponents.widthProperty());
        clipRect.heightProperty().bind(viewComponents.heightProperty());
        viewComponents.setClip(clipRect);

        viewShiftX = 0.0;
        viewShiftY = 0.0;
    }

    @FXML public void viewDragOver(DragEvent event) {
        if(event.getDragboard().hasString())
            event.acceptTransferModes(TransferMode.ANY);
    }

    @FXML public void viewDragDropped(DragEvent event) {
        String type = event.getDragboard().getString();
        int x = (int) Math.floor(event.getX() / 64.0);
        int y = (int) Math.floor(event.getY() / 64.0);
        GUI.controller.getComponents().create(type, x, y);
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

    public void addComponent(Components.Component component, int x, int y) {
        try {
            StackPane stack = FXMLLoader.load(getClass().getResource("/fxml/component.fxml"));
            try {
                Image image = new Image("/img/components/" + component.type + ".png");
                ((ImageView) stack.getChildren().get(1)).setImage(image);
            }
            catch(Exception e) { }

            stack.setTranslateX(64.0 * x);
            stack.setTranslateY(64.0 * y);

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

    public void shiftView(double dx, double dy) {
        viewShiftX += dx;
        viewShiftY += dy;
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

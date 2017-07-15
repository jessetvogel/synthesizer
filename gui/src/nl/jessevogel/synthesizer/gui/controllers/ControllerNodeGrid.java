package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.DragEvent;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.*;
import javafx.scene.shape.Rectangle;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.util.Alert;
import nl.jessevogel.synthesizer.structure.data.Node;

import java.util.HashMap;

public class ControllerNodeGrid {

    @FXML public Pane nodeGrid;

    private double gridShiftX;
    private double gridShiftY;
    private HashMap<javafx.scene.Node, Node> map;

    @FXML public void initialize() {
        GUI.controllerNodeGrid = this;

        Rectangle clipRect = new Rectangle(nodeGrid.getWidth(), nodeGrid.getHeight());
        clipRect.widthProperty().bind(nodeGrid.widthProperty());
        clipRect.heightProperty().bind(nodeGrid.heightProperty());
        nodeGrid.setClip(clipRect);

        gridShiftX = 0.0;
        gridShiftY = 0.0;

        map = new HashMap<>();
    }

    @FXML public void gridMouseClicked() {
        nodeGrid.requestFocus();
    }

    @FXML public void gridDragOver(DragEvent event) {
        if(event.getDragboard().hasString())
            event.acceptTransferModes(TransferMode.ANY);
    }

    @FXML public void gridDragDropped(DragEvent event) {
        String type = event.getDragboard().getString();
        int x = (int) Math.floor((event.getX() - gridShiftX) / 64.0);
        int y = (int) Math.floor((event.getY() - gridShiftY) / 64.0);
        GUI.controller.getNodes().create(type, x, y);
    }

    public void addNode(Node node) {
        try {
            StackPane stack = FXMLLoader.load(getClass().getResource("/fxml/nodes_grid_item.fxml"));
            try {
                Image image = new Image("/img/components/" + node.type.name + ".png");
                ((ImageView) stack.getChildren().get(1)).setImage(image);
            }
            catch(Exception e) { }

            stack.setTranslateX(gridShiftX + node.x * 64.0);
            stack.setTranslateY(gridShiftY + node.y * 64.0);

            stack.setOnMouseClicked(e -> {
                Alert.show("click!", "groetjes!");
//                Parameter[] parameters = new Parameter[2];
//                parameters[0] = new Parameter(); parameters[0].label = "label_1"; parameters[0].value = "value_1";
//                parameters[1] = new Parameter(); parameters[1].label = "label_2"; parameters[1].value = "value_2";
//                setParameters(parameters);
            });

            nodeGrid.getChildren().add(stack);
            map.put(stack, node);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    public void shift(double dx, double dy) {
        gridShiftX += dx;
        gridShiftY += dy;
        for(javafx.scene.Node node : nodeGrid.getChildren()) {
            Node n = map.get(node);
            System.out.println("n.x = " + n.x + "; n.y = " + n.y);
            node.setTranslateX(gridShiftX + n.x * 64.0);
            node.setTranslateY(gridShiftY + n.y * 64.0);
        }
        // TODO: move background
    }
}

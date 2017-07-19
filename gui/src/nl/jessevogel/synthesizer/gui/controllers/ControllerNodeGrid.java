package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.DragEvent;
import javafx.scene.input.TransferMode;
import javafx.scene.layout.*;
import javafx.scene.shape.Rectangle;
import nl.jessevogel.synthesizer.gui.FXMLFiles;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.structure.data.Node;

import java.io.File;
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
            // Load grid item
            StackPane stack = (StackPane) FXMLFiles.load("node_grid_item.fxml");

            // Try to load image
            String imagePath = node.type.directory + "/" + node.type.image;
            if(imagePath != null) {
                try {
                    Image image = new Image(new File(imagePath).toURI().toURL().toExternalForm());
                    ((ImageView) stack.getChildren().get(1)).setImage(image);
                } catch (Exception ignored) {}
            }

            stack.setTranslateX(gridShiftX + node.x * 64.0);
            stack.setTranslateY(gridShiftY + node.y * 64.0);

            stack.setOnMouseClicked(e -> {
                GUI.controller.getNodes().edit(node);
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
            node.setTranslateX(gridShiftX + n.x * 64.0);
            node.setTranslateY(gridShiftY + n.y * 64.0);
        }
        // TODO: move background
    }
}

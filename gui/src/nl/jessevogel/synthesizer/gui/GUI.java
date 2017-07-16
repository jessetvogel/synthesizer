package nl.jessevogel.synthesizer.gui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.controllers.ControllerMenu;
import nl.jessevogel.synthesizer.gui.controllers.ControllerNodeInfo;
import nl.jessevogel.synthesizer.gui.controllers.ControllerNodes;
import nl.jessevogel.synthesizer.gui.controllers.ControllerNodeGrid;
import nl.jessevogel.synthesizer.main.Controller;

import javax.swing.*;
import java.awt.*;

public class GUI extends Application {
    public static Controller controller;
    public static Stage stage;

    public static ControllerMenu controllerMenu;
    public static ControllerNodes controllerNodes;
    public static ControllerNodeGrid controllerNodeGrid;
    public static ControllerNodeInfo controllerNodeInfo;

    @Override
    public void start(Stage primaryStage) throws Exception {
        // Store stage
        GUI.stage = primaryStage;

        // Load main fxml
        Pane pane = FXMLLoader.load(getClass().getResource("/fxml/main.fxml"));

        // Create scene and show window
        Scene scene = new Scene(pane);
        stage.setMaximized(true);
        stage.setMinWidth(640);
        stage.setMinHeight(480);
        stage.setScene(scene);

        // Set icon TODO
        Image image = Toolkit.getDefaultToolkit().getImage("/Users/jessetvogel/Desktop/application.png");
        com.apple.eawt.Application.getApplication().setDockIconImage(image);

        // Set title
        stage.setTitle("Synthesizer GUI");

        scene.setOnKeyPressed(event -> KeyboardHandler.handle(event));

        stage.show();
    }

    public static void initialize(Controller controller, String[] args) {
        GUI.controller = controller;
        launch(args);
    }
}

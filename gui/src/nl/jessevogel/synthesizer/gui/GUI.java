package nl.jessevogel.synthesizer.gui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.controllers.ControllerComponents;
import nl.jessevogel.synthesizer.gui.controllers.ControllerNodeGrid;
import nl.jessevogel.synthesizer.main.Controller;

public class GUI extends Application {
    public static Controller controller;
    public static Stage stage;

    public static ControllerComponents controllerComponents;
    public static ControllerNodeGrid controllerNodeGrid;

    @Override
    public void start(Stage primaryStage) throws Exception {
        // Store stage
        GUI.stage = primaryStage;

        // Load main fxml
        Pane pane = FXMLLoader.load(getClass().getResource("/fxml/main.fxml"));

        // Create scene and show window
        Scene scene = new Scene(pane);
        primaryStage.setMaximized(true);
        primaryStage.setMinWidth(640);
        primaryStage.setMinHeight(480);
        primaryStage.setScene(scene);

        scene.setOnKeyPressed(event -> KeyboardHandler.handle(event));

        primaryStage.show();
    }

    public static void initialize(Controller controller, String[] args) {
        GUI.controller = controller;
        launch(args);
    }
}

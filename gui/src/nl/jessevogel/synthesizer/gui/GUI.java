package nl.jessevogel.synthesizer.gui;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class GUI extends Application {

    @Override
    public void start(Stage window) throws Exception {

        Scene scene = new Scene(FXMLLoader.load(getClass().getResource("/fxml/main.fxml")));
//        scene.getStylesheets().add("css/main.css");

        window.setMaximized(true);
        window.setMinWidth(640);
        window.setMinHeight(480);
        window.setScene(scene);
        window.show();

        ControllerMain.stage = window;

    }

    public static void startup(String[] args) {
        launch(args);
    }
}

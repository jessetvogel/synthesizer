package nl.jessevogel.synthesizer.gui.controllers.windows;

import javafx.fxml.FXMLLoader;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class ControllerWindowSettings {

    public static void show() {
        try {
            // Create new stage
            Stage window = new Stage();
            window.initModality(Modality.APPLICATION_MODAL);
            window.setTitle("Settings");
            window.setResizable(false);

            // Load settings.fxml
            Scene scene = new Scene(FXMLLoader.load(ControllerWindowSettings.class.getResource("/fxml/settings.fxml")));

            // Show stage
            window.setScene(scene);
            window.showAndWait();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

}

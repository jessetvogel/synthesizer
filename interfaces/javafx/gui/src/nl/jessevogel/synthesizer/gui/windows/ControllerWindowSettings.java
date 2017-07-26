package nl.jessevogel.synthesizer.gui.windows;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.Pane;
import javafx.stage.Modality;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.FXMLFiles;

public class ControllerWindowSettings {

    @FXML public Pane content;

    public static void show() {
        try {
            // Create new stage
            Stage window = new Stage();
            window.initModality(Modality.APPLICATION_MODAL);
            window.setTitle("Settings");
            window.setResizable(false);

            // Load settings.fxml
            Scene scene = new Scene(FXMLFiles.load("settings.fxml"));

            // Show stage
            window.setScene(scene);
            window.setWidth(600);
            window.setHeight(400);
            window.showAndWait();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    @FXML public void clickMIDIDevices(ActionEvent event) {
        try {
            Pane pane = FXMLFiles.load("settings_mididevices.fxml");

            content.getChildren().clear();
            content.getChildren().add(pane);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    @FXML public void clickAudioDevices(ActionEvent event) {
        try {
            Pane pane = FXMLFiles.load("settings_audiodevices.fxml");

            content.getChildren().clear();
            content.getChildren().add(pane);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    @FXML public void clickGeneral(ActionEvent event) {
        try {
            Pane pane = FXMLFiles.load("settings_general.fxml");

            content.getChildren().clear();
            content.getChildren().add(pane);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
}

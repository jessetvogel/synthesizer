package nl.jessevogel.synthesizer.gui;

import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.util.Alert;

import java.io.File;

public class ControllerMain {

    public static Stage stage;

    public Button buttonNew;
    public Button buttonOpen;
    public Button buttonSave;
    public Button buttonPlay;
    public Button buttonComponents;
    public Button buttonDevices;
    public Button buttonSettings;

    public void buttonClick(ActionEvent event) {
        Object source = event.getSource();
        if(source == buttonSettings) {
            Alert.show("Alert!", "Clicked settings.");
        }

        if(source == buttonOpen) {
            FileChooser fileChooser = new FileChooser();
            fileChooser.setTitle("Open Resource File");
            File file = fileChooser.showOpenDialog(stage);
            if(file != null) {
                System.out.println(file.getAbsolutePath());
            }
        }
    }
}

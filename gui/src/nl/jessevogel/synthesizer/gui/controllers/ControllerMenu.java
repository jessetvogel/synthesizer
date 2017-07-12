package nl.jessevogel.synthesizer.gui.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.stage.FileChooser;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.util.Alert;

import java.io.File;

public class ControllerMenu {
    @FXML public Button buttonNew;
    @FXML public Button buttonOpen;
    @FXML public Button buttonSave;
    @FXML public Button buttonPlay;
    @FXML public Button buttonComponents;
    @FXML public Button buttonDevices;
    @FXML public Button buttonSettings;

    @FXML
    public void buttonClick(ActionEvent event) {
        Object source = event.getSource();
        if(source == buttonSettings) {
            Alert.show("Alert!", "Clicked settings.");
        }

        if(source == buttonOpen) {
            FileChooser fileChooser = new FileChooser();
            fileChooser.setTitle("Open Resource File");
            File file = fileChooser.showOpenDialog(GUI.stage);
            if(file != null) {
                System.out.println(file.getAbsolutePath());
            }
        }
    }
}

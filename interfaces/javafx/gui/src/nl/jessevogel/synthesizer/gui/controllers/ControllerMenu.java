package nl.jessevogel.synthesizer.gui.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.stage.FileChooser;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.windows.ControllerWindowSettings;
import nl.jessevogel.synthesizer.structure.files.FileOpen;
import nl.jessevogel.synthesizer.structure.files.FileSave;

import java.io.File;

public class ControllerMenu {

    @FXML public Button buttonNew;
    @FXML public Button buttonLoad;
    @FXML public Button buttonSave;
    @FXML public Button buttonPlay;
    @FXML public Button buttonSettings;

    @FXML public void initialize() {
        GUI.controllerMenu = this;
    }

    @FXML public void clickSettings(ActionEvent event) {
        ControllerWindowSettings.show();
    }

    @FXML public void clickPlay(ActionEvent event) {
        GUI.controller.getDevices().toggle();
    }

    @FXML public void clickSave(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();

        // Set extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("Synth files (*.syn)", "*.syn");
        fileChooser.getExtensionFilters().add(extFilter);

        // Show save file dialog
        File file = fileChooser.showSaveDialog(GUI.stage);

        if(file != null) {
            FileSave saver = new FileSave(GUI.controller);
            saver.save(file.getAbsolutePath());
        }
    }

    @FXML public void clickLoad(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();

        // Set extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("Synth files (*.syn)", "*.syn");
        fileChooser.getExtensionFilters().add(extFilter);

        // Show open file dialog
        File file = fileChooser.showOpenDialog(GUI.stage);

        if(file != null) {
            GUI.controller.reset();
            FileOpen loader = new FileOpen(GUI.controller);
            loader.open(file.getAbsolutePath());
        }
    }

    public void updatePlayIcon(boolean playing) {
        if(playing)
            buttonPlay.getStyleClass().add("playing");
        else
            buttonPlay.getStyleClass().remove("playing");
    }
}

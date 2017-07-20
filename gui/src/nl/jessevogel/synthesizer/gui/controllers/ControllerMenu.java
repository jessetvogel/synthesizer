package nl.jessevogel.synthesizer.gui.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.stage.FileChooser;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.controllers.windows.ControllerWindowSettings;
import nl.jessevogel.synthesizer.structure.files.FileSaver;

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
        GUI.controller.getAudio().toggle();
    }

    @FXML public void clickSave(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();

        // Set extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("Synth files (*.syn)", "*.syn");
        fileChooser.getExtensionFilters().add(extFilter);

        // Show save file dialog
        File file = fileChooser.showSaveDialog(GUI.stage);

        if(file != null) {
            FileSaver saver = new FileSaver(GUI.controller);
            saver.save(file.getAbsolutePath());
        }
    }

    @FXML public void clickLoad(ActionEvent event) {

    }

    public void updatePlayIcon(boolean playing) {
        if(playing)
            buttonPlay.getStyleClass().add("playing");
        else
            buttonPlay.getStyleClass().remove("playing");
    }
}

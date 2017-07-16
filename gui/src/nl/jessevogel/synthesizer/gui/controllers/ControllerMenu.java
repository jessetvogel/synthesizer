package nl.jessevogel.synthesizer.gui.controllers;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.controllers.windows.ControllerWindowSettings;

public class ControllerMenu {

    @FXML public Button buttonNew;
    @FXML public Button buttonOpen;
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

    public void updatePlayIcon(boolean playing) {
        if(playing)
            buttonPlay.getStyleClass().add("playing");
        else
            buttonPlay.getStyleClass().remove("playing");
    }
}

package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.response.Settings;

public class ControllerSettingsGeneral {

    @FXML public VBox list;

    @FXML public void initialize() {
        // Header
        Pane header = ListItems.createHeader("General");
        list.getChildren().add(header);

        // Settings fields
        Pane item;
        Settings settings = GUI.controller.getPreferences().getSettings();
        item = ListItems.createChoiceBox("Sustain pedal polarity", new String[] {"normal", "inverted"}, settings.sustainPedalPolarity ? "inverted" : "normal", event -> {
            ChoiceBox choiceBox = (ChoiceBox) event.getSource();
            settings.sustainPedalPolarity = choiceBox.getValue().equals("inverted");
            settings.push(GUI.controller);
        });
        list.getChildren().add(item);

        item = ListItems.createTextField("Pitch wheel range", String.valueOf(settings.pitchWheelRange), event -> {
            TextField textField = (TextField) event.getSource();
            String value = textField.getText();
            if(value.matches("\\d+")) { // TODO
                textField.getStyleClass().remove("invalid");
                settings.pitchWheelRange = Double.parseDouble(value);
                settings.push(GUI.controller);
            }
            else {
                textField.getStyleClass().add("invalid");
            }
        });
        list.getChildren().add(item);
    }
}

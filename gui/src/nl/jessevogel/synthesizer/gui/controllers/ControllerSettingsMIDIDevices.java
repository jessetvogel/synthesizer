package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.response.Device;
import nl.jessevogel.synthesizer.structure.response.Response;

import java.util.ArrayList;

public class ControllerSettingsMIDIDevices {

    @FXML public VBox list;

    @FXML public void initialize() {
        Response response = GUI.controller.getInterface().command("status midi_devices");
        ArrayList<Device> midiDevices = response.getMIDIDevices();

        // Header
        Pane header = ListItems.createHeader("MIDI devices");
        list.getChildren().add(header);

        for(Device device : midiDevices) {
            try {
                Pane item = ListItems.createCheckBox(device.name, device.active, event -> {
                    if(((CheckBox) event.getSource()).isSelected())
                        GUI.controller.getDevices().addMIDIDevice(device);
                    else
                        GUI.controller.getDevices().removeMIDIDevice(device);
                });
                list.getChildren().add(item);
            }
            catch(Exception e) {
                e.printStackTrace();
            }
        }
    }
}

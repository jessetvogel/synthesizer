package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.ChoiceBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.response.Device;

import java.util.Collection;

public class ControllerSettingsAudioDevices {

    @FXML public VBox list;

    @FXML public void initialize() {
        Collection<Device> inputDevices = GUI.controller.getDevices().getInputDevices();
        Collection<Device> outputDevices = GUI.controller.getDevices().getOutputDevices();

        // Header
        Pane header = ListItems.createHeader("Devices devices");
        list.getChildren().add(header);

        String[] inputDeviceOptions = new String[inputDevices.size() + 1];
        inputDeviceOptions[0] = Device.NONE.name;
        String activeInputDevice = Device.NONE.name;
        int i = 0;
        for(Device device : inputDevices) {
            inputDeviceOptions[++i] = device.name;
            if(device.active) activeInputDevice = device.name;
        }
        Pane input = ListItems.createChoiceBox("Input device", inputDeviceOptions, activeInputDevice, event -> {
            String value = ((ChoiceBox) event.getSource()).getValue().toString();
            for(Device device : inputDevices) {
                if(device.name.equals(value)) {
                    GUI.controller.getDevices().setInputDevice(device);
                    return;
                }
            }
            GUI.controller.getDevices().setInputDevice(Device.NONE);
        });
        list.getChildren().add(input);

        String[] outputDeviceOptions = new String[outputDevices.size()];
        String activeOutputDevice = "";
        int j = 0;
        for(Device device : outputDevices) {
            outputDeviceOptions[j++] = device.name;
            if(device.active) activeOutputDevice = device.name;
        }
        Pane output = ListItems.createChoiceBox("Output device", outputDeviceOptions, activeOutputDevice, event -> {
            String value = ((ChoiceBox) event.getSource()).getValue().toString();
            for(Device device : outputDevices) {
                if(device.name.equals(value)) {
                    GUI.controller.getDevices().setOutputDevice(device);
                    return;
                }
            }
        });
        list.getChildren().add(output);
    }
}

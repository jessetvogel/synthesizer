package nl.jessevogel.synthesizer.gui.controllers;

import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.response.Device;
import nl.jessevogel.synthesizer.structure.response.Response;

import java.util.ArrayList;

public class ControllerSettingsAudioDevices {

    @FXML
    public VBox list;

    @FXML public void initialize() {
        Response response = GUI.controller.getInterface().command("status input_devices output_devices");
        ArrayList<Device> inputDevices = response.getInputDevices();
        ArrayList<Device> outputDevices = response.getOutputDevices();

        // Header
        Pane header = ListItems.createHeader("Audio devices");
        list.getChildren().add(header);

        int inputDevicesLength = inputDevices.size();
        String[] inputDeviceOptions = new String[inputDevicesLength + 1];
        inputDeviceOptions[0] = "<none>";
        String activeInputDevice = "<none>";
        for(int i = 0;i < inputDevicesLength; ++i) {
            Device device = inputDevices.get(i);
            inputDeviceOptions[i + 1] = device.name;
            if(device.active) activeInputDevice = device.name;
        }
        Pane input = ListItems.createChoiceBox("Input device", inputDeviceOptions, activeInputDevice, event -> {
            String value = ((ChoiceBox) event.getSource()).getValue().toString();
            for(int i = 0;i < inputDevicesLength; ++i) {
                if(value.equals(inputDevices.get(i).name)) {
                    GUI.controller.getAudio().setInputDevice(inputDevices.get(i).id);
                    return;
                }
            }
            GUI.controller.getAudio().setInputDevice(-1);
        });
        list.getChildren().add(input);

        int outputDevicesLength = outputDevices.size();
        String[] outputDeviceOptions = new String[outputDevicesLength];
        String activeOutputDevice = "";
        for(int i = 0;i < inputDevicesLength; ++i) {
            Device device = outputDevices.get(i);
            outputDeviceOptions[i] = device.name;
            if(device.active) activeOutputDevice = device.name;
        }
        Pane output = ListItems.createChoiceBox("Output device", outputDeviceOptions, activeOutputDevice, event -> {
            String value = ((ChoiceBox) event.getSource()).getValue().toString();
            for(int i = 0;i < outputDevicesLength; ++i) {
                if(value.equals(outputDevices.get(i).name)) {
                    GUI.controller.getAudio().setOutputDevice(outputDevices.get(i).id);
                    return;
                }
            }
            GUI.controller.getAudio().setInputDevice(-1);
        });
        list.getChildren().add(output);
    }
}

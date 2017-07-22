package nl.jessevogel.synthesizer.structure;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.response.Device;
import nl.jessevogel.synthesizer.structure.response.Response;
import nl.jessevogel.synthesizer.structure.response.State;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

public class Devices {

    private Controller controller;
    private boolean active;

    private Map<Integer, Device> midiDevices;
    private Map<Integer, Device> inputDevices;
    private Map<Integer, Device> outputDevices;

    public Devices(Controller controller) {
        this.controller = controller;

        // Create maps
        midiDevices = new HashMap<>();
        inputDevices = new HashMap<>();
        outputDevices = new HashMap<>();

        // Load all devices
        Response response = controller.getInterface().command("status midi_devices input_devices output_devices");
        for(Device device : response.getMIDIDevices())
            midiDevices.put(device.id, device);
        for(Device device : response.getInputDevices())
            inputDevices.put(device.id, device);
        for(Device device : response.getOutputDevices())
            outputDevices.put(device.id, device);
    }

    public boolean play() {
        if(active) return false;
        Response response = controller.getInterface().command("play start");
        return handleResponse(response);
    }

    public boolean stop() {
        if(!active) return false;
        Response response = controller.getInterface().command("play stop");
        return handleResponse(response);
    }

    public boolean toggle() {
        Response response = controller.getInterface().command("play toggle");
        return handleResponse(response);
    }

    // TODO: move this.
    private boolean handleResponse(Response response) {
        State state = response.getState();
        if(state == null) return false;
        active = state.playing;
        GUI.controllerMenu.updatePlayIcon(active);
        return true;
    }

    public void addMIDIDevice(Device device) {
        Response response = controller.getInterface().command("midi_add_device " + device.id);
        // TODO handle response

        // Update preferences
        controller.getPreferences().setActiveMIDIDevice(device.name, true);
    }

    public void removeMIDIDevice(Device device) {
        Response response = controller.getInterface().command("midi_remove_device " + device.id);
        // TODO handle response

        // Update preferences
        controller.getPreferences().setActiveMIDIDevice(device.name, false);
    }

    public void setInputDevice(Device device) {
        // Can only change input/output devices when non-active
        boolean wasActive = active;
        if(active) stop();

        Response response = controller.getInterface().command("audio_set_input_device " + device.id);
        handleResponse(response);

        if(wasActive) play();

        // Update preferences
        controller.getPreferences().setPreferredInputDevice(device.name);
    }

    public void setOutputDevice(Device device) {
        // Can only change input/output devices when non-active
        boolean wasActive = active;
        if(active) stop();

        Response response = controller.getInterface().command("audio_set_output_device " + device.id);
        handleResponse(response);

        if(wasActive) play();

        // Update preferences
        controller.getPreferences().setPreferredOutputDevice(device.name);
    }

    public Device getMIDIDevice(int id) {
        return midiDevices.get(id);
    }

    public Device getInputDevice(int id) {
        return inputDevices.get(id);
    }

    public Device getOutputDevice(int id) {
        return outputDevices.get(id);
    }

    public Collection<Device> getMIDIDevices() {
        return midiDevices.values();
    }

    public Collection<Device> getInputDevices() {
        return inputDevices.values();
    }

    public Collection<Device> getOutputDevices() {
        return outputDevices.values();
    }
}

package nl.jessevogel.synthesizer.structure.controllers;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.response.Device;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;

public class Preferences {

    private Controller controller;

    private ArrayList<String> midiDevices;
    private ArrayList<String> inputDevices;
    private ArrayList<String> outputDevices;

    public Preferences(Controller controller) {
        // Set controller
        this.controller = controller;

        midiDevices = new ArrayList<>();
        inputDevices = new ArrayList<>();
        outputDevices = new ArrayList<>();
        load();
    }

    public void setDevices() {
        // Set preferred MIDI, input and output devices
        Devices devices = controller.getDevices();
        for(Device device : devices.getMIDIDevices()) {
            if(midiDevices.contains(device.name))
                devices.addMIDIDevice(device);
        }
        Device preferredInputDevice = getPreferredInputDevice(devices.getInputDevices());
        if(preferredInputDevice != null)
            devices.setInputDevice(preferredInputDevice);
        Device preferredOutputDevice = getPreferredOutputDevice(devices.getOutputDevices());
        if(preferredOutputDevice != null)
            devices.setOutputDevice(preferredOutputDevice);
    }

    private void load() {
        try {
            String path = controller.getInfo().getPreferencesPath();

            String text = new String(Files.readAllBytes(Paths.get(path)));
            JSONObject object = new JSONObject(text);
            Iterator<String> keys = object.keys();
            while(keys.hasNext()) {
                String key = keys.next();
                switch(key) {
                    case "midiDevices":
                        parseMIDIDevices(object.getJSONArray(key));
                        break;
                    case "inputDevices":
                        parseInputDevices(object.getJSONArray(key));
                        break;
                    case "outputDevices":
                        parseOutputDevices(object.getJSONArray(key));
                        break;
                }
            }
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    private void parseMIDIDevices(JSONArray array) {
        midiDevices.clear();
        for(Object object : array)
            midiDevices.add((String) object);
    }

    private void parseInputDevices(JSONArray array) {
        inputDevices.clear();
        for(Object object : array)
            inputDevices.add((String) object);
    }

    private void parseOutputDevices(JSONArray array) {
        outputDevices.clear();
        for(Object object : array)
            outputDevices.add((String) object);
    }

//    private boolean activeMIDIDevice(String device) {
//        return midiDevices.contains(device);
//    }

    private Device getPreferredInputDevice(Collection<Device> devices) {
        for(String option : inputDevices) {
            for(Device device : devices) {
                if(device.name.equals(option))
                    return device;
            }
        }
        return null;
    }

    private Device getPreferredOutputDevice(Collection<Device> devices) {
        for(String option : outputDevices) {
            for(Device device : devices) {
                if(device.name.equals(option))
                    return device;
            }
        }
        return null;
    }

    public void setActiveMIDIDevice(String device, boolean active) {
        if(active && !midiDevices.contains(device))
            midiDevices.add(device);

        if(!active)
            midiDevices.remove(device);
    }

    public void setPreferredInputDevice(String device) {
        // Move this device to preference #1
        inputDevices.remove(device);
        inputDevices.add(0, device);
    }

    public void setPreferredOutputDevice(String device) {
        // Move this device to preference #1
        outputDevices.remove(device);
        outputDevices.add(0, device);
    }

    public void save() {
        try {
            String path = controller.getInfo().getPreferencesPath();

            PrintWriter writer = new PrintWriter(path);
            writer.write('{');

            writer.write("\"midiDevices\":[");
            boolean comma = false;
            for(String deviceName : midiDevices) {
                if(comma) writer.write(','); else comma = true;
                writer.write("\"" + deviceName + "\"");
            }
            writer.write("],\"inputDevices\":[");
            comma = false;
            for(String deviceName : inputDevices) {
                if(comma) writer.write(','); else comma = true;
                writer.write("\"" + deviceName + "\"");
            }
            writer.write("],\"outputDevices\":[");
            comma = false;
            for(String deviceName : outputDevices) {
                if(comma) writer.write(','); else comma = true;
                writer.write("\"" + deviceName + "\"");
            }
            writer.write("]}\n");
            writer.close();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
}

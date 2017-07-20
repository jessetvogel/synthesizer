package nl.jessevogel.synthesizer.structure.response;

import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.data.NodeOutput;
import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Iterator;

public class Response {

    private JSONObject json;

    public Response(String string) {
        json = new JSONObject(string);
    }

    public ArrayList<Message> getInfo() {
        if(!json.has("info")) return null;
        try {
            JSONArray info = json.getJSONArray("info");
            int n = info.length();
            if(n == 0) return null;
            ArrayList<Message> messages = new ArrayList<>();
            for(int i = 0;i < n; ++i) {
                Message message = new Message();
                message.type = Message.Type.Info;
                message.message = info.getJSONObject(i).getString("message");
            }
            return messages;
        }
        catch(Exception e) {
            return null;
        }
    }

    public ArrayList<Message> getWarning() {
        if(!json.has("warning")) return null;
        try {
            JSONArray warning = json.getJSONArray("warning");
            int n = warning.length();
            if(n == 0) return null;
            ArrayList<Message> messages = new ArrayList<>();
            for(int i = 0;i < n; ++i) {
                Message message = new Message();
                message.type = Message.Type.Warning;
                message.message = warning.getJSONObject(i).getString("message");
            }
            return messages;
        }
        catch(Exception e) {
            return null;
        }
    }

    public ArrayList<Message> getError() {
        if(!json.has("error")) return null;
        try {
            JSONArray error = json.getJSONArray("error");
            int n = error.length();
            if(n == 0) return null;
            ArrayList<Message> messages = new ArrayList<>();
            for(int i = 0;i < n; ++i) {
                Message message = new Message();
                message.type = Message.Type.Error;
                message.message = error.getJSONObject(i).getString("message");
            }
            return messages;
        }
        catch(Exception e) {
            return null;
        }
    }

    public ArrayList<Device> getMIDIDevices() {
        if(!json.has("midiDevices")) return null;
        try {
            ArrayList<Device> devices = new ArrayList<>();
            JSONArray midiDevices = json.getJSONArray("midiDevices");
            int n = midiDevices.length();
            for(int i = 0;i < n; ++i) {
                JSONObject obj = midiDevices.getJSONObject(i);
                Device device = new Device();
                device.type = Device.Type.MIDI;
                device.id = obj.getInt("id");
                device.name = obj.getString("name");
                device.active = obj.getBoolean("active");
                devices.add(device);
            }
            return devices;
        }
        catch(Exception e) {
            return null;
        }
    }

    public ArrayList<Device> getInputDevices() {
        if(!json.has("inputDevices")) return null;
        try {
            ArrayList<Device> devices = new ArrayList<>();
            JSONArray inputDevices = json.getJSONArray("inputDevices");
            int n = inputDevices.length();
            for(int i = 0;i < n; ++i) {
                JSONObject obj = inputDevices.getJSONObject(i);
                Device device = new Device();
                device.type = Device.Type.Input;
                device.id = obj.getInt("id");
                device.name = obj.getString("name");
                device.active = obj.getBoolean("active");
                devices.add(device);
            }
            return devices;
        }
        catch(Exception e) {
            return null;
        }
    }

    public ArrayList<Device> getOutputDevices() {
        if(!json.has("outputDevices")) return null;
        try {
            ArrayList<Device> devices = new ArrayList<>();
            JSONArray outputDevices = json.getJSONArray("outputDevices");
            int n = outputDevices.length();
            for(int i = 0;i < n; ++i) {
                JSONObject obj = outputDevices.getJSONObject(i);
                Device device = new Device();
                device.type = Device.Type.Output;
                device.id = obj.getInt("id");
                device.name = obj.getString("name");
                device.active = obj.getBoolean("active");
                devices.add(device);
            }
            return devices;
        }
        catch(Exception e) {
            return null;
        }
    }

    public NodeInfo getNodeInfo() {
        try {
            NodeInfo nodeInfo = new NodeInfo();
            JSONObject node = json.getJSONObject("node");

            nodeInfo.id = node.getString("id");
            nodeInfo.type = node.getString("type");
            nodeInfo.keyNode = node.getBoolean("keyNode");

            JSONArray inputs = node.getJSONArray("inputs");
            int inputsLength = inputs.length();
            nodeInfo.inputs = new NodeInput[inputsLength];
            for(int i = 0;i < inputsLength; ++i) {
                JSONObject obj = inputs.getJSONObject(i);
                nodeInfo.inputs[i] = new NodeInput();
                nodeInfo.inputs[i].label = obj.getString("label");
                nodeInfo.inputs[i].type = obj.getString("type");
                nodeInfo.inputs[i].value = obj.getString("value");
            }

            JSONArray outputs = node.getJSONArray("outputs");
            int outputsLength = outputs.length();
            nodeInfo.outputs = new NodeOutput[outputsLength];
            for(int i = 0;i < outputsLength; ++i) {
                JSONObject obj = outputs.getJSONObject(i);
                nodeInfo.outputs[i] = new NodeOutput();
                nodeInfo.outputs[i].label = obj.getString("label");
            }

            return nodeInfo;
        }
        catch(Exception e) {
            return null;
        }
    }

    public State getState() {
        try {
            State state = new State();
            JSONObject obj = json.getJSONObject("state");

            state.playing = obj.getBoolean("playing");

            return state;
        }
        catch(Exception e) {
            return null;
        }
    }
}

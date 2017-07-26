package nl.jessevogel.synthesizer.structure.files;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.data.Option;
import nl.jessevogel.synthesizer.structure.data.NodeType;
import org.json.JSONArray;
import org.json.JSONObject;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Iterator;

public class FileOpen {

    public Controller controller;

    public FileOpen(Controller controller) {
        this.controller = controller;
    }

    public void open(String path) {
        try {
            String text = new String(Files.readAllBytes(Paths.get(path)));
            JSONObject json = new JSONObject(text);
            Iterator<String> keys = json.keys();
            while(keys.hasNext()) {
                String key = keys.next();
                switch(key) {
                    case "nodes":
                        parseNodes(json.getJSONArray(key));
                        break;
                }
            }
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    private void parseNodes(JSONArray array) {
        // First create all nodes
        for(Object object : array) {
            JSONObject node = (JSONObject) object;
            String id = node.getString("id");
            String type = node.getString("type");
            int x = node.getInt("x");
            int y = node.getInt("y");
            JSONObject jsonOptions = node.getJSONObject("options");

            NodeType nodeType = controller.getNodeTypes().getNodeType(type);
            if(nodeType == null) {
                System.out.println("NodeType '" + type + "' does not exist!");
                return;
            }

            ArrayList<Option> options = nodeType.createOptions();
            Iterator<String> keys = jsonOptions.keys();
            while(keys.hasNext()) {
                String key = keys.next();
                for(Option option : options) {
                    if(option.label.equals(key))
                        option.value = jsonOptions.getString(key);
                }
            }

            controller.getNodes().create(id, nodeType, options, x, y);
        }

        // Then set all inputs
        for(Object object : array) {
            JSONObject node = (JSONObject) object;
            String id = node.getString("id");
            JSONObject inputs = node.getJSONObject("inputs");
            Iterator<String> keys = inputs.keys();
            while(keys.hasNext()) {
                String key = keys.next();
                controller.getNodes().set(id, key, inputs.getString(key));
            }
        }
    }
}

package nl.jessevogel.synthesizer.structure.files;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.info.NodeType;
import org.json.JSONArray;
import org.json.JSONObject;

import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Iterator;

public class FileLoader {

    public Controller controller;

    public FileLoader(Controller controller) {
        this.controller = controller;
    }

    public void load(String path) {
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
            JSONObject options = node.getJSONObject("options");
            HashMap<String, String> map = new HashMap<>();
            map.put("id", id);
            Iterator<String> keys = options.keys();
            while(keys.hasNext()) {
                String key = keys.next();
                map.put(key, options.getString(key));
            }
            NodeType nodeType = controller.getNodeTypes().getNodeType(type);
            if(nodeType == null) {
                System.out.println("NodeType '" + type + "' does not exist!");
            }
            else
                controller.getNodes().create(nodeType, x, y, map);
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

package nl.jessevogel.synthesizer.structure.controllers;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.data.Option;
import nl.jessevogel.synthesizer.structure.data.NodeType;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class NodeTypes {

    private Controller controller;

    private static final String NODES_DIRECTORY = "nodes";
    private static final String INFO_FILE = "info.json";

    private ArrayList<NodeType> types;
    private ArrayList<String> groups;

    public NodeTypes(Controller controller) {
        // Set controller
        this.controller = controller;

        types = new ArrayList<>();
        groups = new ArrayList<>();
        initialize();
    }

    private void initialize() {
        // Load all node types
        File nodesDirectory = new File(controller.getInfo().getNodesDirectory());
        if(!nodesDirectory.exists() || !nodesDirectory.isDirectory())  return; // TODO
        scanDirectory(nodesDirectory);
    }

    private void scanDirectory(File directory) {
        // Does not make sense to scan files
        if(!directory.isDirectory()) return;

        // Scan all subdirectories
        for(File file : directory.listFiles()) {
            if(file.isDirectory()) {
                scanDirectory(file);
                continue;
            }

            // Search for info files
            if(!file.getName().equals(INFO_FILE)) continue;

            // Get all information from file about node type
            try {
                String text = new String(Files.readAllBytes(Paths.get(file.getAbsolutePath())));
                JSONObject info = new JSONObject(text);

                String name = info.getString("name");
                String group = info.getString("group");
                String image = info.has("image") ? info.getString("image") : "image.png";
                JSONArray optionsArray = info.getJSONArray("options");
                int optionsLength = optionsArray.length();
                Option[] options = new Option[optionsLength];
                for (int i = 0; i < optionsLength; ++i) {
                    JSONObject optionObject = optionsArray.getJSONObject(i);
                    options[i] = new Option();
                    options[i].label = optionObject.getString("label");
                    options[i].type = Option.getType(optionObject.getString("type"));
                    options[i].description = optionObject.getString("description");
                    if(optionObject.has("default"))
                        options[i].value = optionObject.getString("default");
                    else
                        options[i].value = Option.getDefaultValue(options[i].type);
                }

                JSONArray filesArray = info.getJSONArray("files");
                int filesLength = filesArray.length();
                String[] files = new String[filesLength];
                for (int i = 0; i < filesLength; ++i)
                    files[i] = filesArray.getString(i);

                // Create the node type
                NodeType nodeType = new NodeType();
                nodeType.name = name;
                nodeType.group = group;
                nodeType.options = options;
                nodeType.directory = directory.getAbsolutePath();
                nodeType.files = files;
                nodeType.image = image;
                types.add(nodeType);

                // Add group if not yet
                if(!groups.contains(group))
                    groups.add(group);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public NodeType getNodeType(String name) {
        for(NodeType nodeType : types) {
            if(nodeType.name.equals(name))
                return nodeType;
        }
        return null;
    }

    public ArrayList<String> getGroups() {
        return groups;
    }
    public ArrayList<NodeType> getTypes() {
        return types;
    }
}

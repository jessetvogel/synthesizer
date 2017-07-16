package nl.jessevogel.synthesizer.structure.info;

import jdk.nashorn.internal.parser.JSONParser;
import nl.jessevogel.synthesizer.main.Controller;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.File;
import java.io.FileReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;

public class NodeTypes {
    private static final String NODES_DIRECTORY = "nodes";
    private static final String INFO_FILE = "info.json";

    private Controller controller;
    public ArrayList<NodeType> types;
    public ArrayList<String> groups;

    public NodeTypes(Controller controller) {
        this.controller = controller;

        types = new ArrayList<>();
        groups = new ArrayList<>();

        initialize();
    }

    private void initialize() {
        // Load all node types
        File nodesDirectory = new File(controller.getInfo().getDirectory() + NODES_DIRECTORY);
        if(!nodesDirectory.exists() || !nodesDirectory.isDirectory())  return; // TODO
        scanDirectory(nodesDirectory);
    }

    private void scanDirectory(File directory) {
        if(!directory.isDirectory()) return;

        for (File file : directory.listFiles()) {
            if (file.isDirectory()) {
                scanDirectory(file);
                continue;
            }

            if(!file.getName().equals(INFO_FILE)) continue;

            // Get all information from file about node type
            try {
                String text = new String(Files.readAllBytes(Paths.get(file.getAbsolutePath())));
                JSONObject info = new JSONObject(text);

                String name = info.getString("name");
                String group = info.getString("group");
                JSONArray optionsArray = info.getJSONArray("options");
                int optionsLength = optionsArray.length();
                NodeType.Option[] options = new NodeType.Option[optionsLength];
                for (int i = 0; i < optionsLength; ++i) {
                    JSONObject optionObject = optionsArray.getJSONObject(i);
                    options[i] = new NodeType.Option();
                    options[i].label = optionObject.getString("label");
                    options[i].type = NodeType.Option.getType(optionObject.getString("type"));
                    options[i].description = optionObject.getString("description");
                    if(optionObject.has("default"))
                        options[i].defaultValue = optionObject.getString("default");
                    else
                        options[i].defaultValue = NodeType.Option.getDefaultValue(options[i].type);
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
}
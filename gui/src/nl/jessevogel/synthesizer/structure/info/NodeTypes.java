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
        File nodesDirectory = new File(controller.getInfo().getDirectory() + NODES_DIRECTORY);
        for (File f : nodesDirectory.listFiles()) {
            if (!f.isDirectory()) continue;

            // Make sure there is a info.json file
            String infoPath = f.getAbsolutePath() + "/" + INFO_FILE;
            File infoFile = new File(infoPath);
            if (!infoFile.exists() || infoFile.isDirectory()) continue;

            // Get all information from file about node type
            try {
                String text = new String(Files.readAllBytes(Paths.get(infoPath)));
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

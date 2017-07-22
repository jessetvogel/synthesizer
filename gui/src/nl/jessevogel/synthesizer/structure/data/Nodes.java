package nl.jessevogel.synthesizer.structure.data;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.controllers.windows.ControllerWindowOptions;
import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.info.NodeType;
import nl.jessevogel.synthesizer.structure.response.NodeInfo;
import nl.jessevogel.synthesizer.structure.response.Response;

import java.util.ArrayList;
import java.util.HashMap;

public class Nodes {
    public Controller controller;
    public ArrayList<Node> nodes;

    public Nodes(Controller controller) {
        this.controller = controller;
        nodes = new ArrayList<>();
    }

    public void create(String typeName, int x, int y) {
        // Retrieve node type
        NodeType nodeType = controller.getNodeTypes().getNodeType(typeName);

        // Ask for options, and abort if not successful
        HashMap<String, String> map = ControllerWindowOptions.show(nodeType);
        if (map == null) return;

        create(nodeType, x, y, map);
    }

    public void create(NodeType nodeType, int x, int y, HashMap<String, String> map) {
        // Create a node
        Node node = new Node();
        node.type = nodeType;
        node.id = map.get("id");
        node.x = x;
        node.y = y;
        node.options = map;

        // Send it to interface
        NodeLoader nodeLoader = new NodeLoader(controller);
        if(!nodeLoader.load(nodeType, map)) return;

        // Remove id from map
        map.remove("id");

        // Add it to list of nodes
        nodes.add(node);

        // Add it to the grid
        GUI.controllerNodeGrid.addNode(node);
    }

    public void delete(Node node) {
        Response response = controller.getInterface().command("node_delete " + node.id);
        // TODO
        GUI.controllerNodeGrid.removeNode(node);
        nodes.remove(node);
    }

    public void edit(Node node) {
        // Request node info
        Response response = controller.getInterface().command("node_info " + node.id);
        NodeInfo nodeInfo = response.getNodeInfo();

        // Show node inputs
        GUI.controllerNodeInfo.setInfo(nodeInfo);
    }

    public Node getNode(String id) {
        for(Node node : nodes) {
            if(node.id.equals(id))
                return node;
        }
        return null;
    }

    public boolean set(String nodeId, String inputLabel, String value) {
        Response response = controller.getInterface().command("node_set " + nodeId + "." + inputLabel + " " + value);
        if(response.getError() == null)
            return true;
        return false;
    }

    public boolean rename(String id, String newId) {
        Response response = controller.getInterface().command("node_rename " + id + " " + newId);
        if(response.getError() == null) {
            getNode(id).id = newId;
            return true;
        }
        return false;
    }
}

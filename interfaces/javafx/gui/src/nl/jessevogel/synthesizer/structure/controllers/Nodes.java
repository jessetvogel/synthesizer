package nl.jessevogel.synthesizer.structure.controllers;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.windows.ControllerWindowOptions;
import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.data.Node;
import nl.jessevogel.synthesizer.structure.data.NodePusher;
import nl.jessevogel.synthesizer.structure.data.Option;
import nl.jessevogel.synthesizer.structure.data.NodeType;
import nl.jessevogel.synthesizer.structure.response.NodeInfo;
import nl.jessevogel.synthesizer.structure.response.Response;

import java.util.ArrayList;

public class Nodes {

    private Controller controller;

    private ArrayList<Node> nodes;

    public Nodes(Controller controller) {
        // Set controller
        this.controller = controller;

        nodes = new ArrayList<>();
    }

    public void create(String typeName, int x, int y) {
        // Retrieve node type
        NodeType nodeType = controller.getNodeTypes().getNodeType(typeName);
        if(nodeType == null) return;

        // Ask for options, and abort if not successful
        ArrayList<Option> options = ControllerWindowOptions.show(nodeType);
        if (options == null) return;

        // Actually create the node
        create(ControllerWindowOptions.getId(), nodeType, options, x, y);
    }

    public void create(String id, NodeType nodeType, ArrayList<Option> options, int x, int y) {
        // Create a node
        Node node = new Node();
        node.id = id;
        node.type = nodeType;
        node.options = options;
        node.x = x;
        node.y = y;

        // Push it to interface
        NodePusher nodePusher = new NodePusher(controller);
        if(!nodePusher.load(node)) return;

        // Add it to list of nodes
        nodes.add(node);

        // Add it to the grid
        GUI.controllerNodeGrid.addNode(node);
    }

    public boolean delete(Node node) {
        Response response = controller.getInterface().command("node_delete " + node.id);
        if(response.getError() == null) {
            GUI.controllerNodeGrid.removeNode(node);
            nodes.remove(node);
            return true;
        }
        return false;
    }

    public void edit(Node node) {
        // Request node info
        Response response = controller.getInterface().command("node_info " + node.id);
        NodeInfo nodeInfo = response.getNodeInfo();
        node.keyNode = nodeInfo.keyNode;
        node.inputs = nodeInfo.inputs;
        node.outputs = nodeInfo.outputs;

        // Show node inputs
        GUI.controllerNodeInfo.setInfo(node);
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

    public String newId() {
        int n = 1;
        while(getNode("node_" + n) != null)
            n ++;
        return "node_" + n;
    }

    public ArrayList<Node> getNodes() {
        return nodes;
    }
}

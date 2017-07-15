package nl.jessevogel.synthesizer.structure.data;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.info.NodeType;

import java.util.ArrayList;

public class Nodes {
    public Controller controller;
    public ArrayList<Node> nodes;

    public Nodes(Controller controller) {
        this.controller = controller;
        nodes = new ArrayList<>();
    }

    public void create(String type, int x, int y) {
        // TODO: create window where you pass some parameters
        NodeType nodeType = controller.getNodeTypes().getNodeType(type);
        Node node = new Node();
        node.type = nodeType;
        node.id = "NewID";                                                                 // TODO
        node.x = x;
        node.y = y;
        controller.getInterface().command("node_create " + nodeType.name + " " + node.id); // TODO
        nodes.add(node);
        GUI.controllerNodeGrid.addNode(node);
    }
}

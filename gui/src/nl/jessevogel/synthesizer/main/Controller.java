package nl.jessevogel.synthesizer.main;

import nl.jessevogel.synthesizer.structure.Audio;
import nl.jessevogel.synthesizer.structure.data.Nodes;
import nl.jessevogel.synthesizer.structure.info.NodeTypes;
import nl.jessevogel.synthesizer.structure.info.Info;
import nl.jessevogel.synthesizer.structure.Interface;

public class Controller {
    private Info info;
    private NodeTypes nodeTypes;
    private Interface inter;
    private Nodes nodes;
    private Audio audio;

    public Controller() {
        initialize();
    }

    private void initialize() {
        // Create instances
        info = new Info(this, System.getProperty("user.home") + "/Projects/synthesizer/files/");
        nodeTypes = new NodeTypes(this);
        inter = new Interface(this);
        nodes = new Nodes(this);
        audio = new Audio(this);
    }

    public Info getInfo() {return info; }
    public NodeTypes getNodeTypes() {return nodeTypes; }
    public Interface getInterface() { return inter; }
    public Nodes getNodes() { return nodes; }
    public Audio getAudio() { return audio; }
}

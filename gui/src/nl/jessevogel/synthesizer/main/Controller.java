package nl.jessevogel.synthesizer.main;

import nl.jessevogel.synthesizer.structure.Devices;
import nl.jessevogel.synthesizer.structure.Preferences;
import nl.jessevogel.synthesizer.structure.data.Nodes;
import nl.jessevogel.synthesizer.structure.info.NodeTypes;
import nl.jessevogel.synthesizer.structure.info.Info;
import nl.jessevogel.synthesizer.structure.Interface;

public class Controller {
    private Info info;
    private Preferences preferences;
    private NodeTypes nodeTypes;
    private Interface inter;
    private Nodes nodes;
    private Devices devices;

    public Controller() {
        initialize();
    }

    private void initialize() {
        // Create info
        info = new Info(this, System.getProperty("user.home") + "/Projects/synthesizer/files");

        // Create and start interface
        inter = new Interface(this);
        inter.start();

        // Create other instances
        devices = new Devices(this);
        preferences = new Preferences(this);
        nodeTypes = new NodeTypes(this);
        nodes = new Nodes(this);

        // Set stuff
        preferences.setDevices();
    }

    public Info getInfo() {return info; }
    public Preferences getPreferences() {return preferences; }
    public NodeTypes getNodeTypes() {return nodeTypes; }
    public Interface getInterface() { return inter; }
    public Nodes getNodes() { return nodes; }
    public Devices getDevices() { return devices; }

    public void reset() {
        // TODO: implement, reset everything! Clear all nodes, restart interface, etc
    }
}

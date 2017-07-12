package nl.jessevogel.synthesizer.main;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.synth.Components;
import nl.jessevogel.synthesizer.synth.Data;
import nl.jessevogel.synthesizer.synth.Interface;

public class Controller {
    private Data data;
    private Components components;
    private Interface inter;

    public Controller() {
        initialize();
    }

    private void initialize() {
        // Create instances
        data = new Data(this, "/Users/jessetvogel/Projects/synthesizer/files/");
        components = new Components(this);
        inter = new Interface(this);
    }

    public Data getData() {return data; }
    public Components getComponents() {return components; }
    public Interface getInterface() { return inter; }
}

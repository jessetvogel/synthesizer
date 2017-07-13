package nl.jessevogel.synthesizer.main;

import nl.jessevogel.synthesizer.synth.data.Components;
import nl.jessevogel.synthesizer.synth.info.ComponentTypes;
import nl.jessevogel.synthesizer.synth.info.Info;
import nl.jessevogel.synthesizer.synth.Interface;

public class Controller {
    private Info info;
    private ComponentTypes componentTypes;
    private Interface inter;
    private Components components;


    public Controller() {
        initialize();
    }

    private void initialize() {
        // Create instances
        info = new Info(this, "/Users/jessetvogel/Projects/synthesizer/files/");
        componentTypes = new ComponentTypes(this);
        inter = new Interface(this);
        components = new Components(this);
    }

    public Info getInfo() {return info; }
    public ComponentTypes getComponentTypes() {return componentTypes; }
    public Interface getInterface() { return inter; }
    public Components getComponents() { return components; }
}

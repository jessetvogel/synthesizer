package nl.jessevogel.synthesizer.synth.data;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.main.Controller;

import java.util.ArrayList;

public class Components {
    public Controller controller;
    public ArrayList<Component> components;

    public Components(Controller controller) {
        this.controller = controller;
        components = new ArrayList<>();
    }

    public void create(String type, int x, int y) {
        Component component = new Component();
        component.id = "NewID"; // TODO
        component.type = type;
        controller.getInterface().command("unit_create " + type + " " + component.id); // TODO
        components.add(component);
        GUI.controllerComponents.addComponent(component, x, y);
    }

    public class Component {
        public String id;
        public String type;

        public int x, y;
    }
}

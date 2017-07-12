package nl.jessevogel.synthesizer.synth;

import nl.jessevogel.synthesizer.main.Controller;

import java.io.File;
import java.util.ArrayList;

public class Components {
    private static final String COMPONENTS_DIRECTORY = "components";
    private static final String EXTENSION = "block";

    private Controller controller;
    public ArrayList<String> groups = new ArrayList<>();
    public ArrayList<Component> items = new ArrayList<>();

    public Components(Controller controller) {
        this.controller = controller;
        initialize();
    }

    private void initialize() { // TODO : make it cleaner and stuff with extensions better
        File componentsDirectory = new File(controller.getData().getDirectory() + COMPONENTS_DIRECTORY);
        for(File f : componentsDirectory.listFiles()) {
            if(!f.isDirectory()) continue;
            String group = f.getName();
            File[] components = f.listFiles();
            boolean empty = true;
            for(File g : components) {
                String filename = g.getName();
                String extension = filename.substring(filename.lastIndexOf("."));
                if(!extension.equals("." + EXTENSION)) continue;
                Component component = new Component();
                component.name = filename.substring(0, filename.lastIndexOf("."));
                component.group = group;
                items.add(component);
                empty = false;
            }
            if(!empty)
                groups.add(group);
        }
    }

    public class Component {
        public String name;
        public String group;
    }
}

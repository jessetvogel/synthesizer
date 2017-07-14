package nl.jessevogel.synthesizer.synth.info;

import nl.jessevogel.synthesizer.main.Controller;

import java.io.File;
import java.util.ArrayList;

public class ComponentTypes {
    private static final String COMPONENTS_DIRECTORY = "nodes";
    private static final String EXTENSION = "block";

    private Controller controller;
    public ArrayList<String> groups = new ArrayList<>();
    public ArrayList<ComponentType> types = new ArrayList<>();

    public ComponentTypes(Controller controller) {
        this.controller = controller;
        initialize();
    }

    private void initialize() { // TODO : make it cleaner and stuff with extensions better
        File componentsDirectory = new File(controller.getInfo().getDirectory() + COMPONENTS_DIRECTORY);
        for(File f : componentsDirectory.listFiles()) {
            if(!f.isDirectory()) continue;
            String group = f.getName();
            File[] components = f.listFiles();
            boolean empty = true;
            for(File g : components) {
                String filename = g.getName();
                String extension = filename.substring(filename.lastIndexOf("."));
                if(!extension.equals("." + EXTENSION)) continue;
                ComponentType componentType = new ComponentType();
                componentType.name = filename.substring(0, filename.lastIndexOf("."));
                componentType.group = group;
                types.add(componentType);
                empty = false;
            }
            if(!empty)
                groups.add(group);
        }
    }

    public class ComponentType {
        public String name;
        public String group;
    }
}

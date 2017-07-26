package nl.jessevogel.synthesizer.structure.data;

import nl.jessevogel.synthesizer.structure.data.Option;

import java.util.ArrayList;

public class NodeType {

    public String name;
    public String group;

    public Option[] options;
    public String directory;
    public String[] files;
    public String image;

    public ArrayList<Option> createOptions() {
        ArrayList<Option> list = new ArrayList<>();
        for(Option option : options)
            list.add(new Option(option));
        return list;
    }
}
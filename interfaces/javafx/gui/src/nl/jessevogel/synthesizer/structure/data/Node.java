package nl.jessevogel.synthesizer.structure.data;

import java.util.ArrayList;

public class Node {
    public String id;
    public NodeType type;
    public boolean keyNode;

    public NodeInput[] inputs;
    public NodeOutput[] outputs;

    public ArrayList<Option> options;

    public int x, y;
}
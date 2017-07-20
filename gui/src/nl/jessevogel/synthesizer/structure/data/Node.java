package nl.jessevogel.synthesizer.structure.data;

import nl.jessevogel.synthesizer.structure.info.NodeType;

import java.util.Map;

public class Node {
    public String id;
    public NodeType type;
    public Map<String, String> options;

    public int x, y;
}
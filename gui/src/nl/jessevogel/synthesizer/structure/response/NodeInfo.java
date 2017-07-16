package nl.jessevogel.synthesizer.structure.response;

import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.data.NodeOutput;

public class NodeInfo {

    public String id;
    public String type;
    public boolean keyNode;

    public NodeInput[] inputs;
    public NodeOutput[] outputs;

}

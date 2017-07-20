package nl.jessevogel.synthesizer.structure.files;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.data.Node;
import nl.jessevogel.synthesizer.structure.data.NodeInput;
import nl.jessevogel.synthesizer.structure.info.NodeType;
import nl.jessevogel.synthesizer.structure.response.NodeInfo;
import nl.jessevogel.synthesizer.structure.response.Response;

import java.io.PrintWriter;
import java.util.Map;

public class FileSaver {

    public Controller controller;

    public FileSaver(Controller controller) {
        this.controller = controller;
    }

    public void save(String path) {
        try {
            PrintWriter writer = new PrintWriter(path);

            writer.write('{');

            writeNodes(writer);

            writer.write('}');

            writer.write('\n');
            writer.close();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    private void writeNodes(PrintWriter writer) {
        writer.write("\"nodes\":[");

        boolean comma = false;
        for(Node node : controller.getNodes().nodes) {
            Response response = controller.getInterface().command("node_info " + node.id);
            NodeInfo nodeInfo = response.getNodeInfo();

            if(comma) writer.write(','); else comma = true;
            writer.write('{');

            writer.write("\"id\":\"" + node.id + "\",");
            writer.write("\"type\":\"" + node.type.name + "\",");
            writer.write("\"x\":" + node.x + ",");
            writer.write("\"y\":" + node.y + ",");
            writer.write("\"options\":{");
            boolean commaOptions = false;
            for(Map.Entry<String, String> entry : node.options.entrySet()) {
                if(commaOptions) writer.write(','); else commaOptions = true;
                writer.write("\"" + entry.getKey() + "\":\"" + entry.getValue() + "\"");
            }
            writer.write("},");
            writer.write("\"inputs\":{");
            boolean commaInputs = false;
            for(NodeInput input : nodeInfo.inputs) {
                if(commaInputs) writer.write(','); else commaInputs = true;
                writer.write("\"" + input.label + "\":\"" + input.value + "\"");
            }
            writer.write("}");

            writer.write('}');
        }

        writer.write(']');
    }
}

package nl.jessevogel.synthesizer.structure.data;

import nl.jessevogel.synthesizer.main.Controller;

import java.io.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class NodePusher {

    private Controller controller;
    private Node node;

    public NodePusher(Controller controller) {
        this.controller = controller;
    }

    public boolean load(Node node) {
        this.node = node;

        String tmpDirectory = controller.getInfo().getTemporaryDirectory();
        NodeType nodeType = node.type;
        for(String file : nodeType.files) {
            if(!copySubstituted(nodeType.directory + "/" + file, tmpDirectory + "/" + file))
                return false;
        }

        // Let the interface open it
        controller.getInterface().command("include " + tmpDirectory + "/" + nodeType.files[0]);
        // TODO: check if successful

        return true;
    }

    private boolean copySubstituted(String sourcePath, String destinationPath) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader(sourcePath));
            PrintWriter writer = new PrintWriter(destinationPath);
            String line;
            while((line = reader.readLine()) != null) {
                line = substituteLine(line);
                if(line == null) { writer.close(); return false; }
                writer.println(line);
            }

            writer.close();
            return true;
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private Pattern optionPattern = Pattern.compile("\\$(\\w+)");
    private String substituteLine(String line) {
        Matcher matcher = optionPattern.matcher(line);
        while(matcher.find()) {
            String replacement = null;
            if(matcher.group(1).equals("id")) replacement = node.id;
            for(Option option : node.options) {
                if(option.label.equals(matcher.group(1)))
                    replacement = option.value;
            }
            if(replacement == null) return null;
            line = line.replace("$" + matcher.group(1), replacement);
            matcher = optionPattern.matcher(line);
        }
        return line;
    }
}

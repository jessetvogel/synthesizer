package nl.jessevogel.synthesizer.structure.data;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.info.NodeType;

import java.io.*;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class NodeLoader {

    private Controller controller;

    public NodeLoader(Controller controller) {
        this.controller = controller;
    }

    public boolean load(NodeType type, HashMap<String, String> map) {
        String tmpDirectory = controller.getInfo().getTemporaryDirectory();

        for(String file : type.files) {
            if(!copySubstituted(type.directory + "/" + file, tmpDirectory + "/" + file, map))
                return false;
        }

        // Let the interface load it
        controller.getInterface().command("include " + tmpDirectory + "/" + type.files[0]);
        // TODO: check if successful

        return true;
    }

    private boolean copySubstituted(String sourcePath, String destinationPath, HashMap<String, String> map) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader(sourcePath));
            PrintWriter writer = new PrintWriter(destinationPath);
            String line;
            while((line = reader.readLine()) != null) {
                line = substituteLine(line, map);
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
    private String substituteLine(String line, HashMap<String, String> map) {
        Matcher matcher = optionPattern.matcher(line);
        while(matcher.find()) {
            String replacement = map.get(matcher.group(1));
            if(replacement == null) return null;
            line = line.replace("$" + matcher.group(1), replacement);
            matcher = optionPattern.matcher(line);
        }
        return line;
    }
}

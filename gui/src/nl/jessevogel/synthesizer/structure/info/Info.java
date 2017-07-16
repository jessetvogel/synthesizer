package nl.jessevogel.synthesizer.structure.info;

import nl.jessevogel.synthesizer.main.Controller;

import java.io.File;

public class Info {
    private static final String FILE_SETTINGS = "info/settings.info";

    private Controller controller;
    private String directory;
    private String settingsPath;

    public Info(Controller controller, String directory) {
        this.controller = controller;
        this.directory = directory;
        initialize();
    }

    private void initialize() {
        settingsPath = directory + FILE_SETTINGS;
    }

    public String getDirectory() {
        return directory;
    }
    public String getSettingsPath() {
        return settingsPath;
    }

    public String getTemporaryDirectory() {
        File tmpDirectory = new File(directory + "/tmp");
        if(tmpDirectory.isDirectory()) deleteDirectory(tmpDirectory);

        tmpDirectory.mkdir();
        return tmpDirectory.getAbsolutePath();
    }

    private static boolean deleteDirectory(File directory)  {
        boolean success = true;
        if(directory.isDirectory()) {
            String[] children = directory.list();
            for(String child : children)
                success = success && deleteDirectory(new File(directory, child));
        }
        // The directory is now empty or this is a file so delete it
        return success && directory.delete();
    }
}

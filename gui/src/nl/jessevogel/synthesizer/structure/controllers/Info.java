package nl.jessevogel.synthesizer.structure.controllers;

import nl.jessevogel.synthesizer.main.Controller;

import java.io.File;

public class Info {

    private Controller controller;

    private static final String FILE_SETTINGS = "info/settings.json";
    private static final String FILE_PREFERENCES = "info/preferences.json";
    private static final String DIRECTORY_NODES = "nodes";
    private static final String DIRECTORY_TMP = "tmp";

    private String directory;
    private String settingsPath;
    private String preferencesPath;

    public Info(Controller controller, String directory) {
        // Set controller and directory, and initialize
        this.controller = controller;
        this.directory = directory;
        initialize();
    }

    private void initialize() {
        settingsPath = directory + "/" + FILE_SETTINGS;
        preferencesPath = directory + "/" + FILE_PREFERENCES;
    }

    public String getDirectory() {
        return directory;
    }
    public String getSettingsPath() {
        return settingsPath;
    }
    public String getPreferencesPath() {
        return preferencesPath;
    }

    public String getTemporaryDirectory() {
        File tmpDirectory = new File(directory + "/" + DIRECTORY_TMP);
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

    public String getNodesDirectory() {
        return directory + "/" + DIRECTORY_NODES;
    }

}

package nl.jessevogel.synthesizer.synth.info;

import nl.jessevogel.synthesizer.main.Controller;

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

}

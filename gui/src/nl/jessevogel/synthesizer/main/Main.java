package nl.jessevogel.synthesizer.main;

import nl.jessevogel.synthesizer.gui.GUI;

public class Main {

    public static void main(String[] args) {
        // Create controller object
        Controller controller = new Controller();

        // Start GUI and join thread
        Thread thread = new Thread(() -> GUI.initialize(controller, args));
        thread.start();

        try {
            thread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Stop interface
        controller.getInterface().stop();

        // Save preferences
        controller.getPreferences().save();
    }
}

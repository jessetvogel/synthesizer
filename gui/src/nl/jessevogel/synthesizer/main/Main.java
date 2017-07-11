package nl.jessevogel.synthesizer.main;

import nl.jessevogel.synthesizer.synth.Synthesizer;
import nl.jessevogel.synthesizer.gui.GUI;

public class Main {

    public static void main(String[] args) {

        Thread thread = new Thread(() -> GUI.startup(args));
        thread.start();

        Synthesizer synthesizer = new Synthesizer();
        synthesizer.start();
        synthesizer.command("include main.synth");

        try {
            thread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        synthesizer.stop();
    }

}

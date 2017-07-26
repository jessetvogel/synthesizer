package nl.jessevogel.synthesizer.structure.response;

import nl.jessevogel.synthesizer.main.Controller;

public class Settings {

    public double sampleRate;
    public int bufferSize;

    public boolean sustainPedalPolarity;
    public double pitchWheelRange;

    public void push(Controller controller) {
        // Update all changeable settings
        controller.getInterface().command("settings_set sustain_pedal_polarity " + (sustainPedalPolarity ? "inverted" : "normal"));
        controller.getInterface().command("settings_set pitch_wheel_range " + pitchWheelRange);
    }
}

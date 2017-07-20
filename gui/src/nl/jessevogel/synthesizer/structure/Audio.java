package nl.jessevogel.synthesizer.structure;

import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.response.Response;
import nl.jessevogel.synthesizer.structure.response.State;

public class Audio {

    private Controller controller;
    private boolean active;

    public Audio(Controller controller) {
        this.controller = controller;
    }

    public boolean play() {
        if(active) return false;
        Response response = controller.getInterface().command("play start");
        return handleResponse(response);
    }

    public boolean stop() {
        if(!active) return false;
        Response response = controller.getInterface().command("play stop");
        return handleResponse(response);
    }

    public boolean toggle() {
        Response response = controller.getInterface().command("play toggle");
        return handleResponse(response);
    }

    private boolean handleResponse(Response response) {
        State state = response.getState();
        if(state == null) return false;
        active = state.playing;
        GUI.controllerMenu.updatePlayIcon(active);
        return true;
    }

    public void setInputDevice(int id) {
        boolean wasActive = active;
        if(active) stop();

        Response response = controller.getInterface().command("audio_set_input_device " + id);
        handleResponse(response);

        if(wasActive) play();
    }

    public void setOutputDevice(int id) {
        boolean wasActive = active;
        if(active) stop();

        Response response = controller.getInterface().command("audio_set_output_device " + id);
        handleResponse(response);

        if(wasActive) play();
    }
}

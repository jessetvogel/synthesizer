package nl.jessevogel.synthesizer.synth;

import nl.jessevogel.synthesizer.main.Controller;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;

public class Interface {

    private Controller controller;
    private Process process;
    private BufferedReader input;
    private PrintWriter output;

    public Interface(Controller controller) {
        this.controller = controller;
    }

    public boolean start() {
        try {
            String settingsPath = controller.getData().getSettingsPath();
            process = Runtime.getRuntime().exec ("./synthesizer " + settingsPath); // TODO
            input = new BufferedReader(new InputStreamReader(process.getInputStream()));
            output = new PrintWriter(process.getOutputStream());

            String json = input.readLine();
            Response response = new Response(json);
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean stop() {
        try {
            command("exit");
            process.waitFor();
            return true;
        } catch (InterruptedException e) {
            e.printStackTrace();
            return false;
        }
    }

    public Response command(String command) {
        try {
            output.write(command + "\n");
            output.flush();

            String response = input.readLine();
            return new Response(response);
        }
        catch(IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}

package nl.jessevogel.synthesizer.structure.controllers;

import nl.jessevogel.synthesizer.main.Controller;
import nl.jessevogel.synthesizer.structure.response.Response;

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

    public Response start() {
        try {
            String settingsPath = controller.getInfo().getSettingsPath();
            process = Runtime.getRuntime().exec ("./synthesizer " + settingsPath); // TODO
            input = new BufferedReader(new InputStreamReader(process.getInputStream()));
            output = new PrintWriter(process.getOutputStream());

            String json = input.readLine();
            return new Response(json);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public boolean stop() {
        try {
            command("exit");
            process.waitFor();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public Response command(String command) {
        System.out.println("[COMMAND] " + command);
        try {
            output.write(command + "\n");
            output.flush();

            String response = input.readLine();
            System.out.println("[RESPONSE]: " + response);
            return new Response(response);
        }
        catch(IOException e) {
            e.printStackTrace();
            return null;
        }
    }

}

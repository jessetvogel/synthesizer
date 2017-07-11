package nl.jessevogel.synthesizer.synth;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Iterator;

public class Synthesizer {

    private Process process;
    private BufferedReader input;
    private PrintWriter output;

    public Synthesizer() {

    }

    public void start() {
        try {
            process = Runtime.getRuntime().exec ("./synthesizer");
            input = new BufferedReader(new InputStreamReader(process.getInputStream()));
            output = new PrintWriter(process.getOutputStream());

            String response = input.readLine();
            handleResponse(response);

        } catch (IOException e) {
            e.printStackTrace();
        }
        
    }

    public void stop() {
        try {
            command("exit");
            process.waitFor();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void command(String command) {
        try {
            output.write(command + "\n");
            output.flush();

            String response = input.readLine();
            handleResponse(response);
        }
        catch(IOException e) {
            e.printStackTrace();
        }
    }

    public void handleResponse(String response) {
        JSONObject json = new JSONObject(response);
        Iterator<String> keys = json.keys();
        while(keys.hasNext()) {
            String key = keys.next();
            switch(key) {
                case "info":
                    JSONArray infoArray = json.getJSONArray(key);
                    int n = infoArray.length();
                    for(int i = 0;i < n; ++i) {
                        JSONObject info = infoArray.getJSONObject(i);
                        System.out.println("[INFO] " + info.get("message"));
                    }
                    break;

                case "warning":
                    break;

                case "error":
                    JSONArray errorArray = json.getJSONArray(key);
                    n = errorArray.length();
                    for(int i = 0;i < n; ++i) {
                        JSONObject info = errorArray.getJSONObject(i);
                        System.out.println("[ERROR] " + info.get("message"));
                    }
                    break;

                default:
                    System.out.println("Unknown key '" + key + "' in JSON response");
                    break;
            }
        }
    }

}

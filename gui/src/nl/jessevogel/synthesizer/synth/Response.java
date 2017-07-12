package nl.jessevogel.synthesizer.synth;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Iterator;

public class Response { // TODO

    public Response(String string) {
        JSONObject json = new JSONObject(string);
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

package nl.jessevogel.synthesizer.gui;

import javafx.fxml.FXMLLoader;
import javafx.scene.layout.Pane;

public class FXMLFiles {

    private static Object controller;

    public static Pane load(String file) {
        try {
            FXMLLoader loader = new FXMLLoader(FXMLFiles.class.getResource("/fxml/" + file));
            Pane pane = loader.load();
            controller = loader.getController();
            return pane;
        }
        catch(Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static Object getController() {
        return controller;
    }
}

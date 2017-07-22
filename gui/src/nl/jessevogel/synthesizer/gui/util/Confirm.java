package nl.jessevogel.synthesizer.gui.util;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.stage.Modality;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.FXMLFiles;

public class Confirm {

    @FXML private Label title;
    @FXML private Label description;

    private static boolean success;

    public static boolean show(String title, String description) {
        // By default, set success to false
        success = false;

        Pane root = FXMLFiles.load("util/confirm.fxml");

        Confirm confirm = (Confirm) FXMLFiles.getController();
        confirm.title.setText(title);
        confirm.description.setText(description);

        Stage window = new Stage();
        window.initModality(Modality.APPLICATION_MODAL);
        window.setTitle("");
        window.setResizable(false);
        window.setScene(new Scene(root));
        window.showAndWait();

        return success;
    }

    public void clickNo(ActionEvent event) {
        success = false;
        ((Stage) ((Button) event.getSource()).getScene().getWindow()).close();
    }

    public void clickYes(ActionEvent event) {
        success = true;
        ((Stage) ((Button) event.getSource()).getScene().getWindow()).close();
    }
}

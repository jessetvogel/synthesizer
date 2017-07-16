package nl.jessevogel.synthesizer.gui.controllers.windows;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.structure.info.NodeType;

import java.util.HashMap;

public class ControllerWindowOptions {

    private static boolean success;
    private static HashMap<String, String> map = new HashMap<>();

    private NodeType.Option[] options;

    @FXML public VBox optionItemContainer;

    public static HashMap<String, String> show(NodeType.Option[] options) {
        // By default, set success to false
        success = false;

        try {
            // Create new stage
            Stage window = new Stage();
            window.initModality(Modality.APPLICATION_MODAL);
            window.setTitle("TODO: voer hier titel in");
            window.setResizable(false);

            // Load options.fxml
            FXMLLoader fxmlLoader = new FXMLLoader(ControllerWindowOptions.class.getResource("/fxml/options.fxml"));
            Pane pane = fxmlLoader.load();
            ControllerWindowOptions controllerWindowOptions = fxmlLoader.getController();
            controllerWindowOptions.options = options;
            Scene scene = new Scene(pane);

            VBox optionItemContainer = controllerWindowOptions.optionItemContainer;
            for(NodeType.Option option : options) {
                HBox optionItem = FXMLLoader.load(ControllerWindowOptions.class.getResource("/fxml/option.fxml"));
                ((Label) optionItem.getChildren().get(0)).setText(option.description);
                ((TextField) optionItem.getChildren().get(1)).setText(option.defaultValue);
                optionItemContainer.getChildren().add(optionItem);
            }

            // Clear the map
            map.clear();

            // Show stage
            window.setScene(scene);
            window.showAndWait();
        }
        catch(Exception e) {
            e.printStackTrace();
        }

        if(!success) return null;

        return map;
    }

    @FXML public void onClickCreate(ActionEvent event) {
        boolean invalid = false;
        map.clear();

        // Verify id
        TextField idField = (TextField) ((HBox) optionItemContainer.getChildren().get(0)).getChildren().get(1);
        String id = idField.getText();
        if(GUI.controller.getNodes().getNode(id) == null) {
            map.put("id", id);
            idField.getStyleClass().remove("invalid");
        }
        else {
            invalid = true;
            idField.getStyleClass().add("invalid");
        }

        // Verify other inputs
        int size = optionItemContainer.getChildren().size();
        for(int i = 1;i < size; ++i) {
            NodeType.Option option = options[i - 1]; // Note: account for id
            HBox hbox = (HBox) optionItemContainer.getChildren().get(i);
            TextField field = (TextField) hbox.getChildren().get(1);
            String value = field.getText();
            if(!option.validValue(value)) {
                invalid = true;
                field.getStyleClass().add("invalid");
            }
            else
                map.put(option.label, value);
                field.getStyleClass().remove("invalid");
        }

        // If all fields are filled validly
        if(!invalid)
            success = true;

        // Close window on success
        if(success)
            ((Stage) ((Button) event.getSource()).getScene().getWindow()).close();
    }

    @FXML public void onClickCancel(ActionEvent event) {
        // Just to be sure
        success = false;

        // Close window
        ((Stage) ((Button) event.getSource()).getScene().getWindow()).close();
    }
}

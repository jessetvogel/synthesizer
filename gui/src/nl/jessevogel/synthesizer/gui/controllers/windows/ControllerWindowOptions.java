package nl.jessevogel.synthesizer.gui.controllers.windows;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.FXMLFiles;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.info.NodeType;

import java.util.HashMap;

public class ControllerWindowOptions {

    private static boolean success;
    private HashMap<String, String> map;

    @FXML public VBox list;

    public static HashMap<String, String> show(NodeType nodeType) {
        // By default, set success to false
        success = false;

        try {
            // Create new stage
            Stage window = new Stage();
            window.initModality(Modality.APPLICATION_MODAL);
            window.setTitle("TODO: voer hier titel in");
            window.setResizable(false);

            // Load options.fxml
            Pane pane = FXMLFiles.load("options.fxml");
            ControllerWindowOptions controllerWindowOptions = (ControllerWindowOptions) FXMLFiles.getController();
            Scene scene = new Scene(pane);

            VBox list = controllerWindowOptions.list;

            // Create a new map
            controllerWindowOptions.map = new HashMap<>();

            // Header
            Pane header = ListItems.createHeader("Create '" + nodeType.name + "'");
            list.getChildren().add(header);

            // Option id
            String defaultId = "node123"; // TODO
            Pane id = ListItems.createTextField("id", defaultId, event -> {
                TextField field = (TextField) event.getSource();
                String value = field.getText();
                if(GUI.controller.getNodes().getNode(value) != null)
                    field.getStyleClass().add("invalid");
                else {
                    field.getStyleClass().remove("invalid");
                    controllerWindowOptions.map.put("id", value);
                }
            });
            list.getChildren().add(id);
            controllerWindowOptions.map.put("id", defaultId);

            // Node options
            for(NodeType.Option option : nodeType.options) {
                Pane item = ListItems.createTextField(option.description, option.value, event -> {
                    TextField field = (TextField) event.getSource();
                    String value = field.getText();
                    if (!option.validValue(value))
                        field.getStyleClass().add("invalid");
                    else {
                        controllerWindowOptions.map.put(option.label, value);
                        field.getStyleClass().remove("invalid");
                    }
                });
                list.getChildren().add(item);
                controllerWindowOptions.map.put(option.label, option.value);
            }

            // Show stage
            window.setScene(scene);
            window.showAndWait();

            if(!success) return null;
            return controllerWindowOptions.map;
        }
        catch(Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    @FXML public void onClickCreate(ActionEvent event) {
        // TODO: check if there are any fields with .invalid class

        // If all fields are filled validly
//        if(!invalid)
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

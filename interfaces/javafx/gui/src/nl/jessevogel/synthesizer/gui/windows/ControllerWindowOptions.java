package nl.jessevogel.synthesizer.gui.windows;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import nl.jessevogel.synthesizer.gui.FXMLFiles;
import nl.jessevogel.synthesizer.gui.GUI;
import nl.jessevogel.synthesizer.gui.ListItems;
import nl.jessevogel.synthesizer.structure.data.Option;
import nl.jessevogel.synthesizer.structure.data.NodeType;

import java.util.ArrayList;

public class ControllerWindowOptions {

    private static boolean success;
    private static String id;
    private ArrayList<Option> options;

    @FXML public VBox list;

    public static ArrayList<Option> show(NodeType nodeType) {
        // By default, set success to false
        success = false;

        try {
            // Create new stage
            Stage window = new Stage();
            window.initModality(Modality.APPLICATION_MODAL);
            window.setTitle("Create New Node");
            window.setResizable(false);

            // Load options.fxml
            Pane pane = FXMLFiles.load("options.fxml");
            ControllerWindowOptions controllerWindowOptions = (ControllerWindowOptions) FXMLFiles.getController();
            Scene scene = new Scene(pane);

            VBox list = controllerWindowOptions.list;

            // Create a new ArrayList
            controllerWindowOptions.options = nodeType.createOptions();

            // Header
            Pane header = ListItems.createHeader("Create '" + nodeType.name + "'");
            list.getChildren().add(header);

            // Option id
            id = GUI.controller.getNodes().newId();
            Pane idField = ListItems.createTextField("id", id, event -> {
                TextField field = (TextField) event.getSource();
                String value = field.getText();
                if(GUI.controller.getNodes().getNode(value) != null)
                    field.getStyleClass().add("invalid");
                else {
                    field.getStyleClass().remove("invalid");
                    id = value;
                }
            });
            list.getChildren().add(idField);

            // Node options
            for(Option option : controllerWindowOptions.options) {
                Pane item = listItemFromOption(option);
                list.getChildren().add(item);
            }

            // Show stage
            window.setScene(scene);
            window.showAndWait();

            if(!success) return null;
            return controllerWindowOptions.options;
        }
        catch(Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private static Pane listItemFromOption(Option option) {
        Pane pane;
        switch(option.type) {
            case Boolean:
                pane = ListItems.createChoiceBox(option.description, new String[] {"true", "false"}, option.value, event -> {
                    ChoiceBox choiceBox = (ChoiceBox) event.getSource();
                    option.value = (String) choiceBox.getValue();
                });
                return pane;
            case Float:
            case Integer:
                pane = ListItems.createTextField(option.description, option.value, event -> {
                    TextField field = (TextField) event.getSource();
                    String value = field.getText();
                    if (!option.validValue(value))
                        field.getStyleClass().add("invalid");
                    else {
                        option.value = value;
                        field.getStyleClass().remove("invalid");
                    }
                });
                return pane;
        }
        return null;
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

    public static String getId() {
        return id;
    }
}

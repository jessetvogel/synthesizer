package nl.jessevogel.synthesizer.gui;

import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;

public class KeyboardHandler {

    public static void handle(KeyEvent event) {
        switch(event.getCode()) {
            case W:case A:case S:case D:
                double dx = 0.0, dy = 0.0;
                if(event.getCode() == KeyCode.W) dy = 1.0;
                if(event.getCode() == KeyCode.A) dx = 1.0;
                if(event.getCode() == KeyCode.S) dy = -1.0;
                if(event.getCode() == KeyCode.D) dx = -1.0;
                GUI.controllerNodeGrid.shift(dx * 32.0, dy * 32.0);
                break;
        }
    }
}

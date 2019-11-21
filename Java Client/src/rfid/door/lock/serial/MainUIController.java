/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rfid.door.lock.serial;

import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.CornerRadii;
import javafx.scene.paint.Color;

/**
 * FXML Controller class
 *
 * @author mathew
 */
public class MainUIController implements Initializable {
    
    //State colors
    Color CONNECTED = Color.GREEN;
    Color DISCONNECTED = Color.RED;
    Color INTERMEDIATE = Color.YELLOW;
    Color SUCESSFUL = Color.BLUE;
    
    CardReader READER;
    
    @FXML
    public ComboBox PORT_SELECT = new ComboBox();
    @FXML
    public TextArea OUTPUT = new TextArea();
    @FXML
    Button CONNECT = new Button();
    @FXML
    TextField STATUS = new TextField();
    private List<Port> PORTS = null;
    String ACC_OUT = "";

    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        updatePorts();
        print("Port list updated.");
    }

    private void setStatus(Color color, String desc) {
        STATUS.setText(desc);
        STATUS.setBackground(new Background(new BackgroundFill(color,CornerRadii.EMPTY, Insets.EMPTY)));
    }

    public void clearOutput() {
        ACC_OUT = "";
        OUTPUT.setText(ACC_OUT);
    }

    public void print(String s) {
        ACC_OUT += s + "\n";
        OUTPUT.setText(ACC_OUT);
        OUTPUT.setScrollTop(Double.MAX_VALUE);
    }

    public void updatePorts() {
        PORTS = Serial.listPorts();
        updateCombo();
    }

    public void updateCombo() {
        ObservableList<String> list = FXCollections.observableArrayList();
        for (Port p : PORTS) {
            list.add(p.port + ": " + p.descriptor);
        }
        PORT_SELECT.setItems(list);
    }

    public void connect() {
        try {
            int selected = PORT_SELECT.getSelectionModel().getSelectedIndex();
            Port p = this.PORTS.get(selected);
            print("Connecting to " + p.descriptor + " (" + p.port + ")");
            this.setStatus(this.INTERMEDIATE, "Connecting");
            READER = new CardReader(p);
            
            if(READER.initSession()) this.setStatus(CONNECTED, "Connected ("+p.port+")");
            else this.setStatus(this.DISCONNECTED,"Failed to Connect");
        } catch (Exception e) {
            print("Connection failed");
        }
    }

}

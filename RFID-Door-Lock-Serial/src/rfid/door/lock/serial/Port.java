/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rfid.door.lock.serial;

import com.fazecast.jSerialComm.SerialPort;

/**
 *
 * @author mathew
 */
public class Port {
    final public String descriptor;
    final public int port;
    private SerialPort serial = null;
    public Port (int p, String d){
        port = p;
        descriptor = d;
    } 
    
    public SerialPort initSerialPort(){
        if(serial == null) {
           serial = SerialPort.getCommPorts()[port];
           serial.openPort();
        }
        return serial;
    }
    
}

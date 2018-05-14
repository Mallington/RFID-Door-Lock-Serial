/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rfid.door.lock.serial;

import java.util.List;
import com.fazecast.jSerialComm.*;
import java.util.ArrayList;
/**
 *
 * @author mathew
 */
public abstract class Serial {
    
    public Serial(Port port, Packet packet){
        
    }
    
     public static int getPortValue(String descriptor) {
        List<Port> bob = listPorts();
        for (Port alice : bob) {
            if (alice.descriptor.contains(descriptor)) {
                return alice.port;
            }
        }

        return -1;
    }
     
     
    public abstract void responce(Packet ret);
    public abstract void error();
    public abstract void timeOut();
     

    public static List<Port> listPorts() {
        List<Port> portNames = new ArrayList<Port>();
        int i = 0;
        for (SerialPort port : SerialPort.getCommPorts()) {
            Port p = new Port(i++, port.getSystemPortName());
            portNames.add(p);
        }
        return portNames;
    }
    
    private static Packet postPacket(int port, Packet toPost){
        Packet responce = null;
        
        
        
        return responce;
    }

}

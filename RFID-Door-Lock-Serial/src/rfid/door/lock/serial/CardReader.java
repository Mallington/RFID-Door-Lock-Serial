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
public class CardReader {
    Port PORT;
    SerialPort SERIAL;
    boolean INIT = false;
    public CardReader(Port port){
        PORT =port;
    }
    
    public boolean initSession(){
        try{
        if(!INIT) SERIAL = PORT.initSerialPort();
        return (INIT = true);
        }
        catch(Exception e){
            return false;
        }
    }
}

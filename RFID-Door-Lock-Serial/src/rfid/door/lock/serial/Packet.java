/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rfid.door.lock.serial;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author mathew
 */
public class Packet {
    
    private int HEADER;
    private List<List<Byte>> PAYLOAD = null;
    
    public Packet(int header,  List<List<Byte>> payload){
        HEADER = header;
        PAYLOAD = payload;
    }
    
    public List<Byte> getByteArray(){
        List<Byte> arr = new ArrayList<Byte>();
        arr.add((byte)ProtocolConfig.STX);
        arr.add((byte)HEADER);
        if(PAYLOAD != null){
        for(List<Byte> pL: PAYLOAD) {
            arr.add((byte)ProtocolConfig.US);
            for(Byte b : pL) arr.add(b);
        }}
        arr.add((byte)ProtocolConfig.ETX);
        return null;
    }
}

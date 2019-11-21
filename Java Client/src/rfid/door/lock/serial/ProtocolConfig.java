/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package rfid.door.lock.serial;

/**
 *
 * @author mathew
 */
public class ProtocolConfig {
//Serial
  //Functions
  public static int EXIT_SESSION = 0;
  public static int INIT_SESSION = 1;
  public static int READ_CARD = 2;
  public static int WRITE_CARD = 3;

  //Returns
  public static int FAIL = 0;
  public static int SUCCESS = 1;

  //Transmission Markers
  /**
   * Start of text
   */
  public static int STX = 2;
  /**
   * End of text
   */
  public static int ETX=3; 
  /**
   * Unit Separator
   */
  public static int US = 21;
}

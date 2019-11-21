* Author : Mathew Allington
*/
//LCD Crystal Display Variables & Imports
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // Sets
the dimensions of the display
byte timerChar[8] = {
0b00000,
0b01110,
0b10011,
0b11101,
0b10001,
0b01110,
0b00000,
0b00000
};
byte signalChar[8] = { //Bitmap created by
generator :
https://omerk.github.io/lcdchargen/
0b00000,
0b00000,
0b00000,
0b11111,
0b10000,
0b10111,
0b10100,
0b10101
};
byte openChar[8] = {
0b11111,
0b11001,
0b11101,
0b11101,
0b11101,
0b11111,
0b01100,
0b00100
};
byte lockedChar[8] = {
0b00000,
0b01110,
0b10001,
0b11111,
0b10001,
0b10101,
0b10001,
0b11111
};
byte deniedChar[8] = {
0b10001,
0b01010,
0b01010,
0b00100,
0b00100,
0b01010,
0b01010,
0b10001
};
byte adminChar[8] = {
0b00000,
0b11111,
0b10001,
0b10001,
0b11111,
0b01110,
0b11111,
0b00000
};
byte removedChar[8] = {
0b00100,
0b11111,
0b00000,
0b11111,
0b10101,
0b10101,
0b10101,
0b01110
};
byte addedChar[8] = {
0b11111,
0b10001,
0b10101,
0b11111,
0b10101,
0b10001,
0b11111,
0b00000
};
byte splashChar[8] = {
0b10001,
0b01010,
0b10101,
0b01010,
0b10101,
0b01010,
0b10101,
0b01010
};
byte errorChar[8] = {
0b00100,
0b01110,
0b01010,
0b11011,
0b11111,
0b11011,
0b11111,
0b00000
};
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args) write(args);
#else
#define printByte(args) print(args,BYTE);
#endif
int currentIcon = 1;
//-------------------------------
//RFID DoorLock Variables & Imports
int doorPin = 7;
//RFID Reader Variables & Imports
String masterKey= String("de9d54cd");
#include <SPI.h>
#include <MFRC522.h>
//#define SS_PIN 10
#define RST_PIN 3 // Changed to 3 because of
conflict with SD module
MFRC522 rfid(10, RST_PIN);
MFRC522::MIFARE_Key key;
byte currentCard[4];
//-------------------------------
// SD Card Reader Variables & Imports
#include <SD.h>
String* storedUIDs;//[2]=
{"2ed750cd","fee04364"};
File myFile;
int UIDsAmount =0;
String UIDLocation = "UIDs.txt";
int SDPin = 4; // this is the pin the SD
starts up on
//-------------------------------
//Main program variables
int MODE = 0;
int NORMAL = 0;
int ADMIN_SNIFFER = 1;
//--------------------
void setup() {
setUpLock();
setLockOn(true);
Serial.begin(9600);
Serial.println("Lock on");
setUpLCD("","");
Serial.println("Lcd on");
splashScreen(); // Boot sequence
Serial.println("Splash Screen up");
setUpReader();
initSD();
storedUIDs = loadUIDs();
printArray(storedUIDs, UIDsAmount);
Serial.println(String("Gathered")+=String(UI
DsAmount)+=String(" UIDs"));
//if(!(setUpSD())){
// setUpSD();
//while(true);
//}
homeScreen();
}
void loop() {
switch(MODE){
case 0:
if(cardPresent()){
Serial.println("Card Detected");
Serial.println("New Card!");
readCard();
String cardContents =
byteToHex(currentCard,4);// need to convert
to hexadecimal string!!!
Serial.println(cardContents);
if(idEquals(cardContents, masterKey)) {
adminScreen();
Serial.println("Waiting for new card");
delay(2000);
MODE = 1;
}
else if(authID(cardContents)){
setLockOn(false);
// errorScreen(cardContents);
unlockedScreen();
timer(5);
setLockOn(true);
homeScreen();
}
else{
deniedScreen();
Serial.println(cardContents);
timer(5);
homeScreen();
}
}
break;
case 1:
Serial.println("Looking for new card");
adminSniffer();
break;
}
}
// Main program methods
void adminSniffer(){
if(cardPresent()){
delay(1000);
readCard();}
String toAdd = byteToHex(currentCard,4);
if(toAdd.equals(masterKey)) return;
if(authID(toAdd)){
Serial.println(String("Attempting to remove
card:")+=toAdd);
removeAll(toAdd);
removedScreen(toAdd);
writeUIDs(storedUIDs, UIDsAmount);
Serial.println("Reloading");
storedUIDs = loadUIDs();
timer(5);
homeScreen();
MODE =0;
}
else{
Serial.println(String ("Adding new UID:
")+toAdd);
addedScreen(toAdd);
addUID(toAdd);
Serial.println("Reloading");
storedUIDs = loadUIDs();
timer(5);
homeScreen();
MODE =0;
}
}
//------------------------
void removeAll(String syntax){
for(int i = 0; i< UIDsAmount; i++){
if(idEquals( storedUIDs[i], syntax)){
storedUIDs[i] = "";
}
}
}
//LCD Crystal Display Methods
void transition(int timePer){
for(int i = 0; i<=16;i++){
delay(timePer);
lcd.scrollDisplayLeft();
}
}
void testScreen(int wait){
addedScreen("TEST");
delay(wait);
removedScreen("TEST");
delay(wait);
adminScreen();
delay(wait);
unlockedScreen();
delay(wait);
deniedScreen();
delay(wait);
homeScreen();
delay(wait);
}
void errorScreen(String reason){
cleanPrint("Error",reason, true);
printIcon(15,0,errorChar);
printIcon(0,0,errorChar);
}
void addedScreen(String UID){
cleanPrint("Added Card",UID, true);
printIcon(15,0,addedChar);
}
void removedScreen(String UID){
cleanPrint("Removed Card",UID, true);
printIcon(15,0,removedChar);
}
void adminScreen(){
cleanPrint("Admin Mode","Add /
Remove",true);
printIcon(15,0,adminChar);
}
void unlockedScreen(){
cleanPrint("Accepted,","now enter!",true);
printIcon(15,0,openChar);
}
void deniedScreen(){
cleanPrint("DENIED,","invalid key.",true);
printIcon(15,0,deniedChar);
}
void homeScreen(){
cleanPrint("Tap a card","to the
reader:",true);
printIcon(0,0,signalChar);
printIcon(15,0,lockedChar);
}
void splashScreen(){
printIcon(0,0,splashChar);
printIcon(0,1,splashChar);
printIcon(15,0,splashChar);
printIcon(15,1,splashChar);
for(int i= 0; i<=16; i++){
String toPrint = String("");
for(int j =0; j<i; j++){
toPrint += "_";
}
cleanPrint("Allington Tech",toPrint,false);
delay(50);
}
delay(2000);
}
void timer(int period){
printIcon(0,0,timerChar);
lcd.setCursor(1,0);
for(int i=period; 0<i;i--){
lcd.setCursor(1,0);
lcd.print(String(" "));
lcd.setCursor(1,0);
lcd.print(String("")+=i);
delay(1000);
}
}
void printIcon(int collum, int line, byte
icon[]){
lcd.createChar(currentIcon, icon);
lcd.home();
lcd.setCursor(collum, line);
lcd.printByte(currentIcon);
currentIcon++;
if(currentIcon>12) currentIcon = 1; //
because of char limit it resets the number
}
void setUpLCD(String header, String
message){
lcd.init();
//lcd.begin(16,2);
lcd.backlight();
cleanPrint(header, message,true);
}
void cleanPrint(String header, String
message,bool clearLCD){
if(clearLCD){
lcd.clear();}
lcd.setCursor((16-header.length())/2, 0);
lcd.print(header);
lcd.setCursor((16-message.length())/2, 1);
lcd.print(message);
}
//-------------------------------
//Door Lock Methods
void setUpLock(){
pinMode(doorPin, OUTPUT);
}
void setLockOn(boolean on){
if(on){
digitalWrite(doorPin, HIGH);
}
else{
digitalWrite(doorPin, LOW);
}
}
//--------------------------
//RFID Reader Methods
void setUpReader(){
SPI.begin(); // Init SPI bus
rfid.PCD_Init(); // Init MFRC522
for (byte i = 0; i < 6; i++) { //
Initializes the byte array for the key
variable
key.keyByte[i] = 0xFF;
}
}
boolean cardPresent(){
if ( ! rfid.PICC_IsNewCardPresent())
return false;
// Verify if the NUID has been readed
if ( ! rfid.PICC_ReadCardSerial())
return false;
}
boolean cardTypeValid(){
MFRC522::PICC_Type piccType =
rfid.PICC_GetType(rfid.uid.sak);
// Check is the PICC of Classic MIFARE type
if (piccType !=
MFRC522::PICC_TYPE_MIFARE_MINI &&
piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
return false;
}
return true;
}
boolean newCard(){
return (rfid.uid.uidByte[0] !=
currentCard[0] ||
rfid.uid.uidByte[1] != currentCard[1] ||
rfid.uid.uidByte[2] != currentCard[2] ||
rfid.uid.uidByte[3] != currentCard[3] );
}
void readCard(){
for (byte i = 0; i < 4; i++) {
currentCard[i] = rfid.uid.uidByte[i];
}
}
//-------------------------------
// SD Card Reader Methods
void initSD(){
if (!SD.begin(4)) {
Serial.println("initialization failed!");
return;
}
Serial.println("initialization done.");
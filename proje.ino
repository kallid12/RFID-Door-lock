#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 8


#define GREEN_LED       4
#define BEYAZ_LED     6
#define BUZZER        2


MFRC522 mfrc522(SS_PIN, RST_PIN);  



#define NOTE_G4  392
#define NOTE_C5  523
#define NOTE_G5  784
#define NOTE_C6  1047




int TrueMelody[] = {NOTE_G5, NOTE_C6};
int TrueNoteDurations[] = {12, 8};

int FalseMelody[] = {NOTE_C5, NOTE_G4};
int FalseNoteDurations[] = {4, 8};



#define playTrueMelody() playMelody(TrueMelody, TrueNoteDurations, 2)
#define playFalseMelody() playMelody(FalseMelody, FalseNoteDurations, 2)



void setup() {
  Serial.begin(9600);   
  SPI.begin();          
  Serial.println("LÜTFEN KARTINIZI OKUTUN");

  pinMode(GREEN_LED, OUTPUT);
  pinMode(BEYAZ_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
 

}

void loop() {
  
  mfrc522.PCD_Init();   
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }

 
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

 
  Serial.print("UID tag :");
  String RFIDtag = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    RFIDtag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    RFIDtag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mesaj : ");
  RFIDtag.toUpperCase();


  if (( RFIDtag.substring(1) == "E3 32 6E 1A" ) || (RFIDtag.substring(1) =="23 80 85 14"))
  {
    Serial.println("Access izin verildi");
 
    digitalWrite(BEYAZ_LED, HIGH);
  playTrueMelody();   
  delay(500);
   
    
    
    digitalWrite(BEYAZ_LED, LOW);
    delay(250);
   
       
    Serial.println("LÜTFEN KARTINIZI OKUTUN");
  }

  else  
  {
    Serial.println("Access izin verilmedi");

    digitalWrite(GREEN_LED, HIGH);
    playFalseMelody();
    delay(500);
    
    
       
        digitalWrite(GREEN_LED, LOW);
    delay(250);
        
   
   
    Serial.println("LÜTFEN KARTINIZI OKUTUN");
  }
}

void playMelody(int *melody, int *noteDurations, int notesLength)
{
  pinMode(BUZZER, OUTPUT);

  for (int thisNote = 0; thisNote < notesLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
  }
}

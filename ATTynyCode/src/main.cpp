#include <Arduino.h>

#define LED 13
#define SIGNAL_PIN 2
#define REF_OUT 3

#define SAMPLE_CYCLE 2 //ms
#define PULSE_CYCLE 16  //ms
#define PULSE_LENGTH 6 / SAMPLE_CYCLE   //Signal pulse width (6ms) in sample count
#define SIGNAL_LENGTH PULSE_CYCLE / SAMPLE_CYCLE

#define PACKET 16

//#define SERIAL_DEBUG
//#define KEYBOARD
#define BTKEY

#ifdef KEYBOARD
  #include "Keyboard.h"
#endif

uint32_t sampleTimer;
bool buffer[SIGNAL_LENGTH];
uint8_t bufPointer;

uint8_t sampleCounter = 0;

bool packet[PACKET];
uint8_t packetPointer = 0;

uint8_t byteSignal, byteSignalPointer;
uint8_t adjustCycle = 0;

bool sentKey = false;

void setup(){
  #ifdef SERIAL_DEBUG
    Serial.begin(9600);
  #endif

  #ifdef BTKEY
    Serial.begin(9600);
  #endif

  #ifdef KEYBOARD
    Keyboard.begin();
  #endif

  pinMode(LED, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT_PULLUP);
  pinMode(REF_OUT, OUTPUT);

  digitalWrite(LED, LOW);
  digitalWrite(REF_OUT, LOW);

  sampleTimer = millis();
  bufPointer = 0xFF;

}

bool readSignal(){
  bool sample = false;
  uint32_t loop = millis() - sampleTimer;
  if(loop >= SAMPLE_CYCLE){
    sample = true;

    bufPointer ++;
    if(bufPointer >= SIGNAL_LENGTH) bufPointer = 0;

    if(digitalRead(SIGNAL_PIN) == HIGH){
      buffer[bufPointer] = true;
      digitalWrite(LED, HIGH);
      digitalWrite(REF_OUT, HIGH);
    }
    else{
      buffer[bufPointer] = false;
      digitalWrite(LED, LOW);
      digitalWrite(REF_OUT, LOW);
    }
    sampleTimer = millis();
  }
  return sample;
}

uint8_t getValue(){
  uint8_t value = 0xFF;
  //if signal rise is detected or reached one cycle count
  if((bufPointer != 0 && buffer[bufPointer] && !buffer[bufPointer-1])
|| (bufPointer >= SIGNAL_LENGTH - 1)){

    uint8_t signalCounter = 0;
    for(uint8_t i = 0; i < SIGNAL_LENGTH; i ++){
      if(buffer[i]) {
        if(i != bufPointer) signalCounter ++;
      }
    }

    //if signal "1" is detected
    if(signalCounter >= PULSE_LENGTH - 1){
      value = 1;
    }
    else value = 0;

    bufPointer = 0xFF;
  }
  return value;
}

uint8_t findStartPoint(){

  uint8_t table[PACKET];
  uint8_t startPt = 0xFF;

  for(uint8_t i = 0; i < PACKET ; i ++){
    for(uint8_t j = 0; j < PACKET ; j ++){
      if((i + j) < PACKET) table[j] = i + j;
      else table[j] = i + j - PACKET;
    }

    if(packet[table[0]] && packet[table[1]] && packet[table[2]]){
      for(uint8_t j = 0; j < PACKET ; j ++){
    }
      if(!packet[table[11]] && packet[table[12]] && packet[table[13]]){
        startPt = i;
        i = PACKET;
      }
    }
  }
  return startPt;
}

char getChar(uint8_t pointer){
  char extChar = 0;
  uint8_t table[PACKET];
  for(uint8_t j = 0; j < PACKET ; j ++){
    if((pointer + j) < PACKET) table[j] = pointer + j;
    else table[j] = pointer + j - PACKET;
  }

  for(uint8_t i = 0; i < 8 ; i ++){
    if(packet[table[i + 3]]){
      extChar |= B1 << (7 - i);
    }
  }
  return extChar;
}

void typeKey(char key){
  if(key != 0){
    if(!sentKey){
      #ifdef KEYBOARD
        Keyboard.print(key);
      #endif

      #ifdef BTKEY
        Serial.write(key);
      #endif

    sentKey = true;
    }
  }
  else sentKey = false;
}

void loop(){
  char sentCharacter = 0;

  if(readSignal()){
    uint8_t val = getValue();

    // if value is detected
    if(val <= 1){
      //if it is adjust cyle,waste the value
//      if(adjustCycle != 0) adjustCycle --;
//      else{
        packetPointer ++;
        //if signal "1" is detected
        if(val == 1) packet[packetPointer] = true;
        //if signal is "0"
        else if(val == 0) packet[packetPointer] = false;
//      }

      //when the packet buffer is full
      if(packetPointer >= PACKET - 1){

        uint8_t startPoint = findStartPoint();

        if(startPoint != 0xFF){
          sentCharacter = getChar(startPoint);
//          if(startPoint != 0){
//            adjustCycle = PACKET - 1 - startPoint;
//          }
        }

        for(uint8_t i = 0; i < PACKET ; i ++){
          if(packet[i]){
            #ifdef SERIAL_DEBUG
              Serial.print(0x1);
            #endif
          }
          else {
            #ifdef SERIAL_DEBUG
              Serial.print(0x0);
            #endif
          }
          #ifdef SERIAL_DEBUG
            Serial.print(" ");
          #endif
        }

        #ifdef SERIAL_DEBUG
          Serial.print("chr:");
          Serial.print(sentCharacter);
          Serial.println();
        #endif

        #ifdef KEYBOARD
          typeKey(sentCharacter);
        #endif

        #ifdef BTKEY
          typeKey(sentCharacter);
        #endif

        //clear up all
        for(uint8_t i = 0; i < PACKET ; i ++){
          packet[packetPointer] = false;
        }
        packetPointer = 0xFF;
      }
    }
  }
}

#include <Adafruit_NeoPixel.h>

int pins[50] = { 2,3,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53 };

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, 2, NEO_GRB + NEO_KHZ800);

// Protocol details (two header bytes, 7 value bytes, checksum)

// ____________________________________________
// | 0xBA | 0xBE | pin | R | G | B | checksum |
// --------------------------------------------

const int kProtocolHeaderFirstByte = 0xBA;
const int kProtocolHeaderSecondByte = 0xBE;

const int kProtocolHeaderLength = 2;
const int kProtocolBodyLength = 4;
const int kProtocolChecksumLength = 1;

//const int animationFrames = 15;
const int pixelsPerStrip = 60;

// Buffers and state

bool appearToHaveValidMessage;
byte receivedMessage[4];

void setup() {
  for(int i = 0; i < 50; i++) {    
    strip.setPin(pins[i]);
    strip.begin();
    strip.show();
  }
  
  appearToHaveValidMessage = false;
  
  Serial.begin(230400);
  Serial.println("READY");
}

void loop() {
  int availableBytes = Serial.available();
  
  if (!appearToHaveValidMessage) {
    
    // If we haven't found a header yet, look for one.
    if (availableBytes >= kProtocolHeaderLength) {
      
      // Read then peek in case we're only one byte away from the header.
      byte firstByte = Serial.read();
      byte secondByte = Serial.peek();
      
      if (firstByte == kProtocolHeaderFirstByte && secondByte == kProtocolHeaderSecondByte) {
            
          // We have a valid header. We might have a valid message!
          appearToHaveValidMessage = true;
          
          // Read the second header byte out of the buffer and refresh the buffer count.
          Serial.read();
          availableBytes = Serial.available();
      }
    }
  }
  
  if (availableBytes >= (kProtocolBodyLength + kProtocolChecksumLength) && appearToHaveValidMessage) {
     
    // Read in the body, calculating the checksum as we go.
    byte calculatedChecksum = 0;
    
    for (int i = 0; i < kProtocolBodyLength; i++) {
      receivedMessage[i] = Serial.read();
      calculatedChecksum ^= receivedMessage[i];
    }
    
    byte receivedChecksum = Serial.read();
    
    if (receivedChecksum == calculatedChecksum) {
      
        // push value to pin
        strip.setPin(pins[receivedMessage[0]]);
        setPixels(receivedMessage[1], receivedMessage[2], receivedMessage[3], 0);
        
        // DEPRECATED: in favor of not fades
//        int animationFrames = random(8, 25);
//        
//        // TODO: this is probably calculating 1 over so when we go to send the color it goes to zero
//        // calculate the frame steps
//        int frameStepR = int((receivedMessage[1] - receivedMessage[4])/animationFrames)*-1;
//        int frameStepG = int((receivedMessage[2] - receivedMessage[5])/animationFrames)*-1;
//        int frameStepB = int((receivedMessage[3] - receivedMessage[6])/animationFrames)*-1;
//        
//        animateStrip(receivedMessage[1], receivedMessage[2], receivedMessage[3], frameStepR, frameStepG, frameStepB, animationFrames);
      Serial.print("P");
    } else {
      
      Serial.print("F");
    }
    
    appearToHaveValidMessage = false;
  }
}

void resetPins() {
  for(int i = 0; i < 25; i++) {
    strip.setPin(pins[i]);
    setPixels(0,0,0,0);
  }
}

// animate strip
void animateStrip(int fR, int fG, int fB, int frameR, int frameG, int frameB, int frames) {
  int wait = 8;
  
  // foreach transition
  for(int x = 0; x < frames; x++) {
    
    int currR = constrain((fR + (frameR * x)), 0, 255);
    int currG = constrain((fG + (frameG * x)), 0, 255);
    int currB = constrain((fB + (frameB * x)), 0, 255);
    
    setPixels(currR, currG, currB, wait);
  }
}

void setPixels(uint8_t uR, uint8_t uG, uint8_t uB, int wait) {
  // foreach pixel on strip
      for(int p = 0; p < pixelsPerStrip; p++) {
        strip.setPixelColor(p, uR, uG, uB);
      }
      strip.show();
      delay(wait);
}

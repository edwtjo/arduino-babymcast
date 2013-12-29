#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <SPI.h>
#include <Udp.h>
#include <Ethernet.h>
#include <limits.h>

/* LOCAL_OSI_LVL2 */
byte unoMac[] = {
  0xB1, 0x6B, 0x00, 0xB1, 0x1E, 0xE5 // babies love big boobiees..
};

/* LOCAL_OSI_LVL3 */
byte unoIp[] = {
  192, 168, 2, 65
};

/* REMOTE IP/PORT */
byte babyMcastIp[] = {
  239, 255, 47, 79
}; /* this multicast group is
      for screaming babies. */
unsigned int babyMcastPort = 61391; /* this multicast port is to
                                       transfer baby scream detections. */
typedef struct {
  const int audioA0;
  const int audioD0;
} pins_t;
pins_t pins = {A0,0}; /* AudioA0 is connected to UNO A0 and
                         AudioD0 is connected to UNO D2(interrupt) */

const int hz = 124; /* sample rate */

void writeUDP(char dat[]) {
  // multicast udp message using the Udp monad.. no just kidding.
  Udp.sendPacket(dat,babyMcastIp,babyMcastPort);
}

/*
   ISR, see http://www.gammon.com.au/forum/?id=11488
   TL;DR keep it short and sweet and avoid shared state..
*/
volatile int heardBaby = 0;
static void babyScreamDetect(void) {
  heardBaby = 1;
}

void babyScream() {
  char *msg = "WUÄÄÄ!!";
  Serial.println(msg);
  writeUDP(msg);
}

void collectSamples() {
  unsigned int sample=0,min=INT_MAX,max=0,avg=0;
  Serial.println("!");
  for(int i=0;i<hz;i++) {
    sample = analogRead(pins.audioA0);
    if(sample > max) max = sample;
    if(sample < min) min = sample;
    avg += sample;
  }
  avg /= hz;
  Serial.print("analogSample, avg:");
  Serial.print(avg);
  Serial.print(", min:");
  Serial.print(min);
  Serial.print(", max:");
  Serial.println(max);
}

void setup() {
  // 9600 baud serial line
  Serial.begin(9600);

  // networking
  Ethernet.begin(unoMac,unoIp);
  Udp.beginMulti(babyMcastPort,babyMcastIp);

  // register as an ISR
  attachInterrupt(pins.audioD0, babyScreamDetect, RISING);
}

void loop() {
  if(analogRead(pins.audioA0) >= 100) {
    collectSamples();
    babyScream();
    heardBaby = 0;
  } else {
    Serial.print(".");
  }
  delay(10);
}

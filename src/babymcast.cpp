#pragma GCC diagnostic ignored "-Wwrite-strings"
#include <SPI.h>
#include <Udp.h>
#include <Ethernet.h>
#include <limits.h>

/* LOCAL_OSI_LVL2 */
byte unoMac[] = {
  0x02, 0xB1, 0x6B, 0x00, 0xB1, 0xE5 // babies love 2 big boobies..
};
EthernetUDP BabyIGMP;

/* LOCAL_OSI_LVL3 */
/* STATIC FALLBACK CONFIG */
IPAddress unoIp(192,168,2,65);
IPAddress gwIP(192,168,2,60);
IPAddress netM(255,255,255,0);
IPAddress dnsIP(192,168,2,60);

/* REMOTE IP/PORT */
IPAddress babyMcastIp(239,255,47,79); /* this multicast group is for
                                         screaming babies. */
unsigned int babyMcastPort = 61391; /* this multicast port is to
                                       transfer baby scream detections. */

typedef struct {
  const int audioA0;
  const int audioD0;
} pins_t;
pins_t pins = {A0,0}; /* AudioA0 is connected to UNO A0 and
                         AudioD0 is connected to UNO D2(interrupt) */

const int hz = 124; /* sample rate */

void doBabyIGMP(char dat[]) {
  // multicast udp message using the BabyIGMP monad.. no just kidding.
  BabyIGMP.beginPacket(BabyIGMP.remoteIP(),BabyIGMP.remotePort());
  BabyIGMP.write(dat);
  BabyIGMP.endPacket();
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
  doBabyIGMP(msg);
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
  while(!Serial) { /* we dont have a serial line */ }
  // networking
  Serial.println("Networking..");
#ifndef __STATIC_IP__
  if (Ethernet.begin(unoMac) == 0) { /* No DHCP */
#endif
    Serial.println("No DHCP, using static config");
    Ethernet.begin(unoMac,unoIp,dnsIP,gwIP,netM); /* static config, fallback */
#ifndef __STATIC_IP__
  }
#endif
  Serial.print("IP:"); Serial.print(Ethernet.localIP());
  Serial.print(", NETMASK:"); Serial.print(Ethernet.subnetMask());
  Serial.print(", GW:"); Serial.print(Ethernet.gatewayIP());
  Serial.print(", DNS:"); Serial.println(Ethernet.dnsServerIP());
  Serial.print("MCAST IP:"); Serial.println(babyMcastIp);
  BabyIGMP.beginMulti(babyMcastIp, babyMcastPort);
  // register as an ISR
  Serial.println("Registering babyScreamDetect as an ISR on UNO.D2");
  attachInterrupt(pins.audioD0, babyScreamDetect, RISING);
}

void loop() {
  if(analogRead(pins.audioA0) >= 100) {
    babyScream();
    collectSamples();
    heardBaby = 0;
  } else {
    Serial.print(".");
  }
  delay(10);
}

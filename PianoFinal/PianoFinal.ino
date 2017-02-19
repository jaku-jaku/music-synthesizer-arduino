
#include <SPI.h>
#include <RH_NRF24.h>
// Singleton instance of the radio driver
RH_NRF24 nrf24;

int recordBtnStateID = 9;
int recordID = 0;
int playID = 2;
int playBtnStateID = 6;
int size_ = 10;
int nodes[10][2];
int TOL = 6;
int minBand = 0;
int maxBand = 1024;
int BLUE_LED = 4;
int RED_LED = 3;
int button = 5;
int ledTick = 0;
int ax = 0, ay = 0, az = 0;
bool flash = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(recordID, INPUT);
  pinMode(playID, INPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(recordBtnStateID, OUTPUT);
  pinMode(playBtnStateID, OUTPUT);
  for (int i = 0; i < size_; i++) {
    nodes[i][0] = 0;
    nodes[i][1] = 0;
  }
  minBand = (analogRead(A1)) / 2;
  maxBand = (analogRead(A2)) / 2 + 510;

  while (!Serial);
  if (!nrf24.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(2))
    Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");
}

bool recordBtnState = false;
bool playBtnState = false;
int speaker = 7;
bool changed = false;
bool changed0 = false;
int frequency = 0;
//recording vars
int ticks = 0;
int nodeIndex = 0;
int pastFrequency = 0;
bool changed1 = false;
bool digitBtn = false;

void loop() {
  btnLatch();
  wristbandReceiver() ;
  //  Serial.println(frequency);
  if (recordBtnState) {

    if (abs(pastFrequency - frequency) >= TOL * 2 && analogRead(A0) != 0) {
      if (pastFrequency <= 0)
        nodes[nodeIndex][0] = 0;
      else
        nodes[nodeIndex][0] = pastFrequency;
      nodes[nodeIndex][1] = ticks + 1;
      nodeIndex++;
      pastFrequency = frequency;
    }

  } else if (!playBtnState) {
    nodeIndex = 0;
    ticks = 0;
  }
  if (nodeIndex >= size_) {
    recordBtnState = false;
    playBtnState = false;
    ticks = 0;
    nodeIndex = 0;
  }
  if (playBtnState) {
    //    Serial.println(nodeIndex);
    if (nodes[nodeIndex][0] > 0) {
      if(!(digitBtn&&az<=-10)){
        tone( speaker, nodes[nodeIndex][0]+ay*5*(digitBtn), 100+(ax*20+360)*digitBtn);
        delayMicroseconds(100+(ax*20+360)*digitBtn);
      }
    }
    if (ticks >= nodes[nodeIndex][1]) {
      nodeIndex++;
      if (nodeIndex >= size_) {
        nodeIndex = 0;
        ticks = 0;
      }
    }
  } else {
    //    Serial.println(nodeIndex);
    frequencyTuner();
    if (analogRead(A0) != 0 &&!(digitBtn&&az<=-10))
      tone( speaker, frequency, 100+(ax*20+360)*digitBtn);
    else
      noTone(speaker);
  }
  ticks++;

  //signal for wireless on
  ledTick++;
  if (ledTick % 50 == 0)
    flash = !flash;
  if (ledTick > 1000)
    ledTick = 0;
}

void btnLatch() {
  while (analogRead(A0) == 0 && digitalRead(recordID) == 0 && !changed && !playBtnState) {
    changed = true;
    recordBtnState = !recordBtnState;
  }
  if (digitalRead(recordID) == 1)
    changed = false;

  while (analogRead(A0) == 0 && digitalRead(playID) == 1 && !changed0 && !recordBtnState) {
    changed0 = true;
    playBtnState = !playBtnState;
  }
  if (digitalRead(playID) == 0)
    changed0 = false;
  digitalWrite(recordBtnStateID, recordBtnState);
  digitalWrite(playBtnStateID, playBtnState);
}

void frequencyTuner() {
  int oldfrequency = frequency;
  minBand = (analogRead(A1)) / 2;
  maxBand = (analogRead(A2)) / 2 + 510;
  frequency = minBand + ((analogRead(A0) - 510) / 510.0 * (maxBand - minBand))+ay*5*(digitBtn);
  if (abs(oldfrequency - frequency) <= TOL)
    frequency = oldfrequency;
}


void wristbandReceiver() {
  while (digitalRead(button) == 1 && !changed1) {
    changed1 = true;
    digitBtn = !digitBtn;
  }
  if (digitalRead(button) == 0)
    changed1 = false;
  if (digitBtn) {

    if (nrf24.available())
    {
      // Should be a message for us now
      uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (nrf24.recv(buf, &len))
      {
        //      NRF24::printBuffer("request: ", buf, len);
//        Serial.print("got request: ");
        ax = (int)buf[0];
        ay = (int)buf[1];
        az = (int)buf[2];

        if (ax > 126)
          ax = ax - 255;
        if (ay > 126)
          ay = ay - 255;
        if (az > 126)
          az = az - 255;
  ay=-ay;
//        Serial.print(ax);
//        Serial.print(" | ");
//        Serial.print(ay);
//        Serial.print(" | ");
//        Serial.println(az);
//        

        // Send a reply
//        uint8_t data[] = "And hello back to you";
//        nrf24.send(data, sizeof(data));
//        nrf24.waitPacketSent();
          digitalWrite (BLUE_LED, flash);
      }
      else
      {
        Serial.println("recv failed");
      }
    }

  } else {
    digitalWrite (RED_LED, LOW);
    digitalWrite (BLUE_LED, LOW);
  }
}


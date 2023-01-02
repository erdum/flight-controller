#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Controls Pinouts
#define SERVO1 3
#define SERVO2 5
#define SERVO3 6
#define SERVO4 9
#define SERVO5 10
#define LEFT_LATCH 2
#define RIGHT_LATCH 4

// Trasnceiver Pinouts
#define TRX_CS 7
#define TRX_CSN 8

struct Controls {
    uint8_t throttle;
    uint8_t rudder;
    uint8_t elevator;
    uint8_t ailerons;
    uint8_t flaps;
    uint8_t rudderTrim;
    uint8_t elevatorTrim;
    uint8_t aileronsTrim;
    bool leftLatch;
    bool rightLatch;
}

struct Instruments {
    uint16_t voltage;
    uint16_t currentConsumption;
    uint16_t barometricPressure;
    uint16_t altitude;
    uint16_t heading;
    uint16_t latitude;
    uint16_t longitude;
    uint16_t gpsAltitude;
    uint16_t gpsSpeed;
    uint8_t temperature;
}

Controls controls;

controls.throttle = 0;
controls.rudderTrim = 0;
controls.elevatorTrim = 0;
controls.aileronsTrim = 0;
controls.flaps = 0;
controls.leftLatch = false;
controls.rightLatch = false;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

RF24 radio(TRX_CS, TRX_CSN);
const uint8_t address[][6] = {"00000"};

void setup() {
    radio.begin();
    radio.setChannel(125);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(0, address[0]);
    radio.startListening();

    servo1.attach(SERVO1);
    servo2.attach(SERVO2);
    servo3.attach(SERVO3);
    servo4.attach(SERVO4);
    servo5.attach(SERVO5);

    pinMode(LEFT_LATCH, OUTPUT);
    pinMode(RIGHT_LATCH, OUTPUT);
}

void loop() {
    controls.rudder = controls.rudderTrim;
    controls.elevator = controls.elevatorTrim;
    controls.ailerons = controls.aileronsTrim;

    while(radio.available()) {
        radio.read(&controls, sizeof(controls));
    }

    servo1.write(controls.throttle);
    servo2.write(controls.rudder);
    servo3.write(controls.elevator);
    servo4.write(controls.ailerons);
    servo5.write(controls.flaps);

    digitalWrite(LEFT_LATCH, controls.leftLatch);
    digitalWrite(RIGHT_LATCH, controls.rightLatch);
}

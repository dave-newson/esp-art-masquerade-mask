#pragma once

#include "Arduino.h"
#include "ESP32Servo.h"

class Fins
{
public:
    Fins(Servo* _servos, uint _servoCount) {
        servos = _servos;
        servoCount = _servoCount;
    };

    void begin() {
        setAllAngle(0);
    };

    Servo* getServo(uint num) {
        if (num > servoCount) return nullptr;
        return &servos[num];
    }

    void setServoAngle(uint num, uint value) {
        if (num > servoCount) return;
        (&servos[num])->write(value);
    }

    void setAllAngle(uint value) {
        for (uint i=0; i<servoCount; i++) {
            setServoAngle(i, value);
        }
    }

private:
    Servo* servos;
    uint servoCount;
};

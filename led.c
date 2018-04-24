#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//------------------------------------------
// LED API
void InitLEDs() {
    GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN6);   // color LED red on sensor board
    GPIO_setAsOutputPin    (GPIO_PORT_P2,    GPIO_PIN4);   // color LED green on sensor board
    GPIO_setAsOutputPin    (GPIO_PORT_P5,    GPIO_PIN6);   // color LED blue on sensor board
    GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P2,    GPIO_PIN4);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5,    GPIO_PIN6);
}

// 3 functions for LED on booster
void Toggle_Booster_LED(){
    GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6);
}

void TurnON_Booster_LED(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
}

void TurnOFF_Booster_LED(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
}

void redLED(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
}
void greenLED(){
    GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
}
void blueLED(){
    GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin (GPIO_PORT_P5, GPIO_PIN6);
}
void yellowLED(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputHighOnPin (GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputLowOnPin (GPIO_PORT_P5, GPIO_PIN6);
}
void violetLED(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputLowOnPin (GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin (GPIO_PORT_P5, GPIO_PIN6);
}
void cyanLED(){
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputHighOnPin (GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin (GPIO_PORT_P5, GPIO_PIN6);
}
void whiteLED(){
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
    GPIO_setOutputHighOnPin (GPIO_PORT_P2, GPIO_PIN4);
    GPIO_setOutputHighOnPin (GPIO_PORT_P5, GPIO_PIN6);
}

#include <stdlib.h>
#include "swtimer.h"
#include "maplogic.h"
#include "button.h"
#include "display.h"
#include "hwtimer.h"
#include "goertzel.h"
#include "dtmf.h"
#include "microphone.h"
#include "led.h"
#include "sound.h"

#define DURATION 1

//sound HAL
//pwm initialization
Timer_A_PWMConfig pwmConfig = {
TIMER_A_CLOCKSOURCE_SMCLK,
                                TIMER_A_CLOCKSOURCE_DIVIDER_1,
                                (int) (SYSTEMCLOCK / 523.25),         // C5
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        (int) ((SYSTEMCLOCK / 2) / 523.25) };

//initialize buzzer
void InitSound()
{
    // This function switches the IC pin connected to
    // the buzzer from GPIO functionality to Timer_A functionality
    // so that we can drive it with PWM.
    GPIO_setAsPeripheralModuleFunctionOutputPin(
    GPIO_PORT_P2,
                                                GPIO_PIN7,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
}

//Play sound in different tones in different length
void PlaySound(tnote n, unsigned ms)
{
    tSWTimer oneshot;
    int timeCount1 = SYSTEMCLOCK / 10;
    uint32_t period = timeCount1 * ms;
    InitSWTimer(&oneshot, TIMER32_1_BASE, period);
    if (n == note_c5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 523.25);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 392);
        blueLED();
    }
    if (n == note_d5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 587.33);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 587.33);
        violetLED();
    }
    if (n == note_e5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 659.25);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 659.25);
        cyanLED();
    }
    if (n == note_f5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 698.46);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 698.46);
        redLED();
    }
    if (n == note_fs5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 739.99);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 739.99);
        greenLED();
    }
    if (n == note_g5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 783.99);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 783.99);
        yellowLED();
    }
    if (n == note_a5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 880);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 880);
        whiteLED();
    }
    if (n == note_b5)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 987.77);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 987.77);
        blueLED();
    }
    if (n == note_c6)
    {
        pwmConfig.timerPeriod = (SYSTEMCLOCK / 1046.5);
        pwmConfig.dutyCycle = ((SYSTEMCLOCK / 2) / 1046.5);
        yellowLED();
    }
    if (n == note_silent)
    {
        pwmConfig.timerPeriod = 0;
        pwmConfig.dutyCycle = 0;
        TurnOFF_Booster_LED();
    }
    Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig);
    while (!SWTimerOneShotExpired(&oneshot));
    Timer_A_stopTimer(TIMER_A0_BASE);
}
//circle win sound
void playSoundCircle()
{
    PlaySound(note_g5, 2 * DURATION);   // G4 // 3
    PlaySound(note_a5, 2 * DURATION);   // A4 // 1
    PlaySound(note_b5, 2 * DURATION);   // B4 // 2
    PlaySound(note_c5, 2 * DURATION);   // C5 // 2
    PlaySound(note_e5, 2 * DURATION);   // D5 // 2
    PlaySound(note_e5, 2 * DURATION);   // E5 // 2
    PlaySound(note_f5, 2 * DURATION);   // F5 // 2
    PlaySound(note_fs5, 2 * DURATION);   // F#5 // 2
    PlaySound(note_g5, 4 * DURATION);   // G5 // 16
    PlaySound(note_silent, DURATION / 2);
}
//cross win sound
void playSoundCross()
{
    PlaySound(note_g5, 2 * DURATION);   // G5 // 2
    PlaySound(note_e5, 2 * DURATION);   // G4 // 2
    PlaySound(note_a5, 2 * DURATION);   // A4 // 2
    PlaySound(note_c5, 2 * DURATION);   // C5 // 2
    PlaySound(note_d5, 2 * DURATION);   // D5 // 2
    PlaySound(note_c6, DURATION);   // C6 // 1
    PlaySound(note_silent, DURATION / 2);
}
//tie sound
void playSoundTie()
{
    PlaySound(note_b5, 2 * DURATION);
    PlaySound(note_c6, 2 * DURATION);
    PlaySound(note_b5, 2 * DURATION);
    PlaySound(note_c6, 2 * DURATION);
    PlaySound(note_b5, 2 * DURATION);
    PlaySound(note_c6, DURATION);
    PlaySound(note_silent, DURATION / 2);
}
//computer sound when computer thinking
void computerSound(int i)
{
    if (i == 0)
    {
        PlaySound(note_c5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_g5, DURATION);
    }
    else if (i == 1)
    {
        PlaySound(note_c5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_a5, DURATION);
    }
    else if (i == 2)
    {
        PlaySound(note_c5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_b5, DURATION);
    }
    else if (i == 3)
    {
        PlaySound(note_d5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_g5, DURATION);
    }
    else if (i == 4)
    {
        PlaySound(note_d5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_a5, DURATION);
    }
    else if (i == 5)
    {
        PlaySound(note_d5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_b5, DURATION);
    }
    else if (i == 6)
    {
        PlaySound(note_e5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_g5, DURATION);
    }
    else if (i == 7)
    {
        PlaySound(note_e5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_a5, DURATION);
    }
    else if (i == 8)
    {
        PlaySound(note_e5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_a5, DURATION);
    }
    else if (i == 9)
    {
        PlaySound(note_e5, DURATION);
        PlaySound(note_silent, DURATION / 2);
        PlaySound(note_b5, DURATION);
    }
}


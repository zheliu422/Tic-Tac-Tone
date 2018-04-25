#include "dtmf.h"
#include "display.h"
#include "maplogic.h"
#include "sound.h"

#define SOUND_HOLD 6096

// TO BE DEFINED BY YOU
//
// Compute Goertzel Filter Coefficients for
// Frequency Detectors for the following frequencies:
//
//      Gtap structure     Fgoertzel
//      -----------------------------
//      t697                 697 Hz
//      t770                 770 Hz
//      t852                 852 Hz
//      t1209               1209 Hz
//      t1336               1336 Hz
//      t1477               1477 Hz
//
// A Goertzel filter coefficient is defined by
//
//           c = 2*cos(2*pi*v)
//
// with v equal to Fgoertzel / Fsample.
// For example, if Fgoertzel = 1000Hz and Fsample = 8000Hz
// then c = 2 * cos (2*pi*1/8) = 1.4142
// Be sure to compute cos() in radians
//
// This results in the following filter coefficients
//
//      Gtap structure     Fgoertzel       Coefficient
//      ----------------------------------------------
//      t697                 697 Hz      0.8538689 * 2
//      t770                 770 Hz      0.8226405 * 2
//      t852                 852 Hz      0.7843435 * 2
//      t1209               1209 Hz      0.5820520 * 2
//      t1336               1336 Hz      0.4981851 * 2
//      t1477               1477 Hz      0.3993092 * 2
//
// To encode the Goertzel filter coefficient into Gtap,
// you have to convert the floating point value of c
// into a fixed point value.
//
// For a fixed point representation <WLEN,FRACBITS>,
// The resulting Gtap coef value is
//
//         coef =  (int) (c * (1 << FRACBITS))
//
// Make sure that you do NOT use double or float in your program.
// Use ONLY integer types (signed or unsigned).

// FILL IN THE FOLLOWING VALUES TO DEFINE PROPER COEF VALUES

static Gtap  t697 = {(int) ( 0.8538689 * 2 * (1 << FRACBITS)) };
static Gtap  t770 = {(int) ( 0.8226405 * 2 * (1 << FRACBITS)) };
static Gtap  t852 = {(int) ( 0.7843435 * 2 * (1 << FRACBITS)) };
static Gtap t1209 = {(int) ( 0.5820520 * 2 * (1 << FRACBITS)) };
static Gtap t1336 = {(int) ( 0.4981851 * 2 * (1 << FRACBITS)) };
static Gtap t1477 = {(int) ( 0.3993092 * 2 * (1 << FRACBITS)) };

void DTMFAddSample(unsigned x) {
    unsigned vx = ScaleSample(x);

    SampleGoertzel( &t697, vx);
    SampleGoertzel( &t770, vx);
    SampleGoertzel( &t852, vx);
    SampleGoertzel(&t1209, vx);
    SampleGoertzel(&t1336, vx);
    SampleGoertzel(&t1477, vx);
}

void DTMFReset() {
    ResetGoertzel( &t697);
    ResetGoertzel( &t770);
    ResetGoertzel( &t852);
    ResetGoertzel(&t1209);
    ResetGoertzel(&t1336);
    ResetGoertzel(&t1477);
}

//check input frequency
//different combination connect to different location
//for example, keyboard 1 represent 697Hz and 1209Hz
//if I get both 697Hz and 1209Hz, then return 0 for future use
int DTMFCheck() {
    unsigned p697  = PowerGoertzel(  &t697);
    unsigned p770  = PowerGoertzel(  &t770);
    unsigned p852  = PowerGoertzel(  &t852);
    unsigned p1209 = PowerGoertzel( &t1209);
    unsigned p1336 = PowerGoertzel( &t1336);
    unsigned p1477 = PowerGoertzel( &t1477);

    if ((p697 >= SOUND_HOLD )&&(p1209 >= SOUND_HOLD))
        return 0;
    else if ((p697 >= SOUND_HOLD )&&(p1336 >= SOUND_HOLD))
        return 1;
    else if ((p697 >= SOUND_HOLD )&&(p1477 >= SOUND_HOLD))
        return 2;
    else if ((p770 >= SOUND_HOLD )&&(p1209 >= SOUND_HOLD))
        return 3;
    else if ((p770 >= SOUND_HOLD )&&(p1336 >= SOUND_HOLD))
        return 4;
    else if ((p770 >= SOUND_HOLD )&&(p1477 >= SOUND_HOLD))
        return 5;
    else if ((p852 >= SOUND_HOLD )&&(p1209 >= SOUND_HOLD))
        return 6;
    else if ((p852 >= SOUND_HOLD )&&(p1336 >= SOUND_HOLD))
        return 7;
    else if ((p852 >= SOUND_HOLD )&&(p1477 >= SOUND_HOLD))
        return 8;

        return 9;
}

//from the last function, if I have 0 for the return
//that means I have 697Hz and 1209Hz
//so 697Hz and 1209Hz corresponding to map location 0
//location 0 is top left corner
//then place a circle on this place
void drawCir(gamestate_t *G, int winner)
{
    switch(winner)
    {
    case 0:
        G->map[0] = circle;
        //DrawBoard(G->map);
        break;
    case 1:
        G->map[1] = circle;
        //DrawBoard(G->map);
        break;
    case 2:
        G->map[2] = circle;
        //DrawBoard(G->map);
        break;
    case 3:
        G->map[3] = circle;
        //DrawBoard(G->map);
        break;
    case 4:
        G->map[4] = circle;
        //DrawBoard(G->map);
        break;
    case 5:
        G->map[5] = circle;
        //DrawBoard(G->map);
        break;
    case 6:
        G->map[6] = circle;
        //DrawBoard(G->map);
        break;
    case 7:
        G->map[7] = circle;
        //DrawBoard(G->map);
        break;
    case 8:
        G->map[8] = circle;
        //DrawBoard(G->map);
        break;
    }
}

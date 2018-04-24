#ifndef SOUND_H_
#define SOUND_H_

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

typedef enum
{
    note_silent,
    note_c4,
    note_d4,
    note_e4,
    note_f4,
    note_g4,
    note_a4,
    note_b4,
    note_c5,
    note_d5,
    note_e5,
    note_f5,
    note_fs5,
    note_g5,
    note_a5,
    note_b5,
    note_c6
} tnote;

void InitSound();
void PlaySound(tnote n, unsigned ms);
void playSoundCircle();
void playSoundCross();
void playSoundTie();
void computerSound(int i);
#endif /* SOUND_H_ */

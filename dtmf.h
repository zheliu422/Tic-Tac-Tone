#ifndef DTMF_H_
#define DTMF_H_

#include "goertzel.h"
#include "display.h"
#include "maplogic.h"

void     DTMFAddSample(unsigned x);
void     DTMFReset();
unsigned DTMFPower();
int      DTMFCheck();
void drawCir(gamestate_t *G,int winner);


#endif /* DTMF_H_ */

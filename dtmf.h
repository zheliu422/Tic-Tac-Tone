#ifndef DTMF_H_
#define DTMF_H_

#include "goertzel.h"
#include "display.h"
#include "maplogic.h"

typedef enum {t1,t2,t3,
              m1,m2,m3,
              b1,b2,b3,x} circle_place;

void     DTMFAddSample(unsigned x);
void     DTMFReset();
unsigned DTMFPower();
int      DTMFCheck();
void drawCir(gamestate_t *G,int winner);


#endif /* DTMF_H_ */

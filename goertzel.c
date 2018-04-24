#include "goertzel.h"

#define PTHRESHOLD 1000
#define FRACBITS 8
#define FIXMUL(A, B) ((A * B) >> FRACBITS)

int ScaleSample(unsigned s) {
    // scale microphone sample to fix <32, FRACBITS>
    // the microphone signal is assumed to lie
    // between 0x1000 (min) and 0x2FFF (max) with midpoint 0x1FFF
    //
    // we map to <32, 10>
    return ((int) s - 0x1FFF) >> (12 - FRACBITS);
}

void SampleGoertzel(Gtap *t, unsigned x) {
    // TO BE COMPLETED BY YOU
    //
    // Insert a sample x into the Goertzel filter
    // and update the filter state Gtap.
    //
    // The elements of the filter state are
    //    t->s1     First state variable
    //    t->s2     Second state variable
    //    t->coef   Goertzel filter coefficient
    //
    // After inserting a sample x, the filter state
    // is updated as follows
    //
    //        s0 = x + coef * s1 - s2;
    //        s2 = s1;
    //        s1 = s0;
    //
    // CAREFUL: This expression has to be computed
    // using fixed point arithmetic. So you cannot use
    // standard integer multiplication (*).

    int a = FIXMUL(t->coef, t->s1);
    int b = x + a;
    int s0 = b - t->s2;

    t->s2 = t->s1;
    t->s1 = s0;


    //==================================================
}

void ResetGoertzel(Gtap *t) {
    t->s1 = 0;
    t->s2 = 0;
}

int PowerGoertzel(Gtap *t) {
    // TO BE COMPLETED BY YOU
    //
    // This function computes the signal power
    // stored in the goertzel filter state
    //
    // The elements of the filter state are
    //    t->s1     First state variable
    //    t->s2     Second state variable
    //    t->coef   Goertzel filter coefficient
    //
    // The power is defined by the following expression
    //
    //        P = s1 * s1 + s2 * s2 - coef * s1 * s2
    //
    // CAREFUL: This expression has to be computed
    // using fixed point arithmetic. So you cannot use
    // standard integer multiplication (*).

    int term1 = FIXMUL(t->s1, t->s1);
    int term2 = FIXMUL(t->s2, t->s2);
    int term3_1 = FIXMUL(t->coef, t->s1);
    int term3 = FIXMUL(term3_1, t->s2);
    int P;
    P = term1 + term2 - term3;

        return P;

    //==================================================
}

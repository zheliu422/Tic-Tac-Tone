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

#define UPEDGE(A, B) (A && !(B))
#define PRESSED 1

int over = 0; //flag
int check = 1;
unsigned glbMax = 0;

typedef enum {idle, gameS1, gameS2} state_t;
unsigned glbListening = 0;

// This is the state definition for the top-level state machine,
// implemented in ProcessStep. As you build the game, you will
// have to define additional states to implement the actual game
// logic.

typedef enum
{
    processidle_idle,
    processidle_playingcircle,
    processidle_playingcross,
    processidle_winning
} processidle_state_t;

// This structure is a game state. It keeps track of the current
// playing field, the score achieved by the human, and the score
// achieved by the computer. The game state gets passed around
// during the game to allow different software components access
// to the game state.

typedef enum
{
    waiting, playingcircle, playingcross, winning, waiting2, waiting3
} state_t1;

static state_t1 states2 = waiting;
//interrupts
void T32_INT1_IRQHandler()
{
    unsigned vx;
    static unsigned SamplesListened = 0;
    if (glbListening)
    {
        vx = GetSampleMicrophone();
        if (vx > glbMax)
            glbMax = vx;
        DTMFAddSample(vx);
        SamplesListened++;
        if (SamplesListened == 400)
        {
            glbListening = 0;
            SamplesListened = 0;
        }
    }
    Timer32_clearInterruptFlag(TIMER32_0_BASE);
}
// cell map state. As the game proceeds, these
// locations fill up with crosses and circles
// until one wins, or it's a tie.
//this is state for Button S1
//basically let computer first play
void ProcessS1(gamestate_t *G, int sec2)
{
    unsigned w;
    int cir = 9;
    int titleCheck = 0;
    int i = 20;
    switch (states2)
    {
    case waiting:
        ClearMap(G->map);
        DrawBoardIdle(G->map);
        states2 = playingcross;
        break;

        // It's cross's turn to play
        // cross play is for computer
    case playingcross:
        DrawMessage("Thinking...   ", GRAPHICS_COLOR_YELLOW);
        // find a random location available and add a cross
        i = RandomAdd(G->map, cross);
        computerSound(i);
        // next turn will be circle playing
        states2 = playingcircle;
        // update the playing field
        DrawBoard(G->map);
        // check if the game is over
        if (CrossWins(G->map) || Tie(G->map))
            states2 = winning;
        break;

        //this is circle's turn to play
        //circle play is for user
    case playingcircle:
        if(titleCheck == 0)
        DrawMessage("Listening...   ", GRAPHICS_COLOR_YELLOW);
        DisplayMaxMin(glbMax);
        cir = DTMFCheck();
        if (cir == 9)
        {
            titleCheck = 0;
            glbListening = 1;
            cir = DTMFCheck();
        }
        while (G->map[cir] != empty && (cir != 9))
        {
            titleCheck = 1;
            DTMFReset();
            DrawMessage("Illegal Move ", GRAPHICS_COLOR_YELLOW);
            glbListening = 1;
            cir = DTMFCheck();
        }
        if ((G->map[cir] == empty) && (cir != 9))
        {
            titleCheck = 0;
            DTMFReset();
            glbListening = 0;
            drawCir(G, cir);
            states2 = playingcross;
            DrawBoard(G->map);
            if (CircleWins(G->map) || Tie(G->map))
                   states2 = winning;
        }
        break;

        // Game is finished. Show the winner
    case winning:
        // Highlight circle winning score
        if (w = CircleWins(G->map))
        {
            DrawWinner(G->map, w, GRAPHICS_COLOR_YELLOW);
            DrawMessage("You Win        ", GRAPHICS_COLOR_YELLOW);
            playSoundCircle();
            G->humanscore = G->humanscore + 1;
        }
        // Highlight cross winning score
        if (w = CrossWins(G->map))
        {
            DrawWinner(G->map, w, GRAPHICS_COLOR_YELLOW);
            DrawMessage("I Win          ", GRAPHICS_COLOR_YELLOW);
            playSoundCross();
            G->computerscore = G->computerscore + 1;
        }
        if (w = Tie(G->map))
        {
            DrawMessage("Tie            ", GRAPHICS_COLOR_YELLOW);
            playSoundTie();
        }
        states2 = waiting2;
        break;

        // Wait for button 2
    case waiting2:
        ClearMap(G->map);
        DrawBoardIdle(G->map);
        over = 1;
        states2 = waiting;
        break;
    }
}
// cell map state. As the game proceeds, these
// locations fill up with crosses and circles
// until one wins, or it's a tie.
//this is state for Button S2
//basically let user first play
void ProcessS2(gamestate_t *G, int sec2)
{
    unsigned w;
    int cir = 9;
    int titleCheck = 0;
    int i = 20;
    switch (states2)
    {
    case waiting:
        ClearMap(G->map);
        DrawBoardIdle(G->map);
        states2 = playingcircle;
        break;

        // It's cross's turn to play
    case playingcross:
        DrawMessage("Thinking...   ", GRAPHICS_COLOR_YELLOW);
        // find a random location available and add a cross
        i = RandomAdd(G->map, cross);
        computerSound(i);
        // next turn will be circle playing
        states2 = playingcircle;

        // update the playing field
        DrawBoard(G->map);
        // check if the game is over
        if (CrossWins(G->map) || Tie(G->map))
            states2 = winning;
        break;

    case playingcircle: //wo
        if(titleCheck == 0)
        DrawMessage("Listening...   ", GRAPHICS_COLOR_YELLOW);

        cir = DTMFCheck();
        DisplayMaxMin(glbMax);
        if (cir == 9)
        {
            titleCheck = 0;
            glbListening = 1;
            cir = DTMFCheck();
        }
        while (G->map[cir] != empty && (cir != 9))
        {
            titleCheck = 1;
            DTMFReset();
            DrawMessage("Illegal Move ", GRAPHICS_COLOR_YELLOW);
            glbListening = 1;
            cir = DTMFCheck();
        }
        if ((G->map[cir] == empty) && (cir != 9))
        {
            titleCheck = 0;
            DTMFReset();
            glbListening = 0;
            drawCir(G, cir);
            states2 = playingcross;
            DrawBoard(G->map);
            if (CircleWins(G->map) || Tie(G->map))
                   states2 = winning;
        }
        break;

        // Game is finished. Show the winner
    case winning:
        // Highlight circle winning score
        if (w = CircleWins(G->map))
        {
            DrawWinner(G->map, w, GRAPHICS_COLOR_YELLOW);
            DrawMessage("You Win        ", GRAPHICS_COLOR_YELLOW);
            playSoundCircle();
            G->humanscore = G->humanscore + 1;
        }
        // Highlight cross winning score
        if (w = CrossWins(G->map))
        {
            DrawWinner(G->map, w, GRAPHICS_COLOR_YELLOW);
            DrawMessage("I Win          ", GRAPHICS_COLOR_YELLOW);
            playSoundCross();
            G->computerscore = G->computerscore + 1;
        }
        if (w = Tie(G->map))
        {
            DrawMessage("Tie            ", GRAPHICS_COLOR_YELLOW);
            playSoundTie();
        }
        states2 = waiting2;
        break;
        // Wait for button 2
    case waiting2:
        ClearMap(G->map);
        DrawBoardIdle(G->map);
        over = 1;
        states2 = waiting;
        break;
    }
}

// This function implements the functionality of Tic Tac Tone during
// the idle state, i.e. when it is waiting for the player to provide
// a 'start game' command.
//
// This function is called every time the top-level FSM visits the
// idle state. The function has five parameters. You are welcome to
// add more, but know that the reference solution was implemented using
// only these 5 parameters.
//    b1   = 1 when button S1 is pressed, 0 otherwise
//    b2   = 1 when button S2 is pressed, 0 otherwise
//    sec  = 1 when the second-interval software timer elapses
//    ms50 = 1 when the 50ms-interval software timer elapses
//    G    = gamestate, as defined above. Pass by reference to make
//           sure that changes to G within this function will be
//           propagated out of the function.
//
// Note that this function RETURNS a state_t. This can be used to influence
// the state transition in the top-level FSM that will call processIdle.
// Currently, the ProcessIdle always returns idle (which means the top-level
// FSM will iterate in the idle state), but when the game is extended,
// the return state could change into something different (such as playing_circle
// or playing_cross, depending on whether S1 or S2 is pressed).

state_t ProcessIdle(int sec, int ms50,
                    gamestate_t *G) {

    // These are the states of a _local_ FSM.
    // The state labels are prefixed with 'processidle' to
    // make sure they are distinct from the labels used for the
    // top-level FSM.
    //
    // The local FSM functionality plays a game of tic-tac-toe
    // against itself, using randomized moves. However, the
    // rules of tic-tac-toe are followed, including the game
    // map drawing and coloring over the reference solution.

    unsigned w;

    static processidle_state_t localstate = processidle_idle;
    // We will run this local state machine only once per second,
    // therefore, we only run it when sec is 1. sec is
    // a software-timer generated in the main function.
    //
    // To add more functionality, you can extend this function. For example,
    // to display a label every three seconds, you can add a counter that is
    // incremented for every sec, modulo-3. When the counter is two, it means
    // that the three-second timer mark is reached.
    //
    // A longer counter period (eg modulo-12 iso modulo-3) can be used to
    // display rotating messages.
    if (sec) {

        switch (localstate) {

            case processidle_idle:
              // Initially, just draw the playing field

              ClearMap(G->map);
              DrawBoardIdle(G->map);
              localstate = processidle_playingcircle;

              break;

            case processidle_playingcircle:
              // This is circle who is playing. A circle is
              // added in a random (but valid) location. Next,
              // we check if the game ends, which happens when
              // circle or cross would win, or when there's a tie.
              // Decide what position to play
              RandomAdd(G->map, circle);

              // Next, it's cross' turn
              localstate = processidle_playingcross;

              // If we win or tie, go to winning state instead
              if (CircleWins(G->map) || Tie(G->map))
                  localstate = processidle_winning;

              // update game board status on display
              DrawBoardIdle(G->map);
              break;

            case processidle_playingcross:
              // This is cross who is playing. A cross is
              // added in a random (but valid) location. Next,
              // we check if the game ends, which happens when
              // circle or cross would win, or when there's a tie.
              // Decide what position to play
              RandomAdd(G->map, cross);

              // Next, it's circles' turn
              localstate = processidle_playingcircle;

              // If we win or tie, go to winning state instead
              if (CrossWins(G->map) || Tie(G->map))
                  localstate = processidle_winning;

              // update game board status on display
              DrawBoardIdle(G->map);
              break;

            case processidle_winning:
              // This state is entered when there is a winner,
              // or it's a tie. In this state, we redraw the
              // winning combination in the emphasis color.
              // After that, we go for the next round.
              if (w = CircleWins(G->map))
                DrawWinner(G->map, w, EMPHASISCOLOR);

              if (w = CrossWins(G->map))
                DrawWinner(G->map, w, EMPHASISCOLOR);

              localstate = processidle_idle;
              break;
        }
    }
    return idle;
}
//this is the title on the top line of the screen
void drawTitle(int sec3, gamestate_t G)
{
    typedef enum
    {
        title_title, title_s1, title_s2, title_score
    } title_state_t;
    static title_state_t defultTitle = title_s1;
    if (check == 1)
    {
        DrawMessage("*Tic Tac Tone*", GRAPHICS_COLOR_YELLOW);
        check = 0;
    }
    if(sec3)
    {
        switch(defultTitle)
        {
        case title_title:
            if (check == 1)
                defultTitle = title_s1;
            DrawMessage("*Tic Tac Tone*", GRAPHICS_COLOR_YELLOW);
            defultTitle = title_s1;
            break;
        case title_s1:
            DrawMessage("S1: I Start   ", GRAPHICS_COLOR_YELLOW);
            defultTitle = title_s2;
            break;
        case title_s2:
            DrawMessage("S2: You Start ", GRAPHICS_COLOR_YELLOW);
            defultTitle = title_score;
            break;
        case title_score:
            DrawScore(G.computerscore, G.humanscore , GRAPHICS_COLOR_YELLOW);
            //DrawMessage("Score XX - YY ", GRAPHICS_COLOR_YELLOW);
            defultTitle = title_title;
            break;
        }
    }
}

// This is the top-level FSM, which is called from within
// the cyclic executive. You will have to extend this FSM
// with the game logic. The FSM takes four inputs:
//
//    S1d is current button status
//    prev_S1d is previous button status
//    S2d is current button status
//    prev_S2d is previous button status
//    1 when button is pressed, 0 otherwise
//    ms50 = 1 when the 50ms-interval software timer elapses
void ProcessStep(int S1d, int S2d, int prev_S1d, int prev_S2d, int sec, int ms50, int sec3, int sec2) {
    static state_t S = idle;
    static gamestate_t G;

    switch (S) {

    case idle://on idle state, run screen saver
        drawTitle(sec3, G);
        S = ProcessIdle(sec, ms50, &G);
        if ((prev_S1d != PRESSED) && (S1d == PRESSED))
            S = gameS1;

        if ((prev_S2d != PRESSED) && (S2d == PRESSED))
            S = gameS2;

        break;

    case gameS1:
        ProcessS1(&G, sec2);
        if ((prev_S1d != PRESSED) && (S1d == PRESSED)) //surrender
        {
            AbortMap(G.map);
            states2 = winning;
            DrawBoard(G.map);
        }
        if ((prev_S2d != PRESSED) && (S2d == PRESSED)) //cheat mode
        {
            CheatMap(G.map);
            states2 = winning;
            DrawBoard(G.map);
        }

        if (over == 1) //once decide winner, return to idle state
        {
            S = idle;
            over = 0;
            check = 1;
        }
        break;
    case gameS2:
        ProcessS2(&G, sec2);
        if ((prev_S1d != PRESSED) && (S1d == PRESSED))
        {
            AbortMap(G.map);
            states2 = winning;
            DrawBoard(G.map);
        }
        if ((prev_S2d != PRESSED) && (S2d == PRESSED)) //cheat mode
        {
            CheatMap(G.map);
            states2 = winning;
            DrawBoard(G.map);
        }

        if (over == 1) //once decide winner, return to idle state
        {
            S = idle;
            over = 0;
            check = 1;
        }
        break;
    }
}

int main(void) {

    // Device Initialization
    InitTimer();
    InitDisplay();
    InitButtonS1();
    InitButtonS2();
    InitSound();
    InitMicrophone();
    InitLEDs();

    // Software Timer - per second
    // Note that software timers MUST be tied to TIMER32_1_BASE;
    // TIMER32_1_BASE is configured in continuous mode
    // (TIMER32_0_BASE can then be used for periodic interrupts
    //  which will be needed to take samples from microphone)
    tSWTimer everySec;
    InitSWTimer(&everySec, TIMER32_1_BASE, SYSTEMCLOCK);
    StartSWTimer(&everySec);

    tSWTimer every2Sec;
    InitSWTimer(&every2Sec, TIMER32_1_BASE, SYSTEMCLOCK * 2);
    StartSWTimer(&every2Sec);

    tSWTimer every3Sec;
    InitSWTimer(&every3Sec, TIMER32_1_BASE, SYSTEMCLOCK * 3);
    StartSWTimer(&every3Sec);

    // Software Timer - per 50ms = 20Hz
    tSWTimer every50ms;
    InitSWTimer(&every50ms, TIMER32_1_BASE, SYSTEMCLOCK/20);
    StartSWTimer(&every50ms);

    Interrupt_enableInterrupt(INT_T32_INT1);
    Interrupt_enableMaster();

    glbListening = 1;
    int S2d = 0, prev_S2d = 0;
    int S1d = 0, prev_S1d = 0;
    // The cyclic executive is simple: read the buttons and software
    // timers, and call the top-level FSM in ProcessStep.
    while (1) {

        S1d    = ButtonS1Pressed();
        S2d    = ButtonS2Pressed();
        int sec   = SWTimerExpired(&everySec);
        int sec2  = SWTimerExpired(&every2Sec);
        int sec3  = SWTimerExpired(&every3Sec);
        int ms50  = SWTimerExpired(&every50ms);

        ProcessStep(S1d, S2d, prev_S1d, prev_S2d, sec, ms50, sec3, sec2);
        prev_S1d = S1d;
        prev_S2d = S2d;
    }

}

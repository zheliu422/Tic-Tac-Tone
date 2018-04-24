#include "display.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

Graphics_Context g_sContext;
//initial LCD display
void InitDisplay() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, FOREGROUNDCOLOR);
    Graphics_setBackgroundColor(&g_sContext, BACKGROUNDCOLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmtt16);
    Graphics_clearDisplay(&g_sContext);
}

void DrawTime(unsigned minutes, unsigned seconds) {
    char buf[5];
    buf[4] = (seconds % 10) + '0';
    buf[3] = (seconds / 10) % 10 + '0';
    buf[2] = ':';
    buf[1] = (minutes % 10) + '0';
    buf[0] = (minutes / 10) % 10 + '0';
    Graphics_drawStringCentered(&g_sContext, (int8_t *) buf, 5, 64, 120, OPAQUE_TEXT);
}

//draw message function
void DrawMessage(char *s, uint32_t color) {
    Graphics_setForegroundColor(&g_sContext, color);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) s, -1, 64, 8, OPAQUE_TEXT);
}

//score function to display all scores
void DrawScore(int computerscore, int humanscore, uint32_t color) {
    char buf[16] = "Score XX - XX ";
    buf[ 7] =  (char) ('0' + ( computerscore % 10));
    buf[ 6] =  (char) ('0' + ((computerscore/10) % 10));
    buf[12] =  (char) ('0' + ( humanscore % 10));
    buf[11] =  (char) ('0' + ((humanscore/10) % 10));
    Graphics_setForegroundColor(&g_sContext, color);
    Graphics_drawStringCentered(&g_sContext, (int8_t *) buf, -1, 64, 8, OPAQUE_TEXT);
}

//draw empty or X or O on related places
void DrawBoard(tcellstate map[9]) {
    int8_t *symbolMap[3] = {" ", "X", "O"};

    Graphics_setForegroundColor(&g_sContext, LINECOLOR);
    Graphics_drawLineH(&g_sContext, 19,        109,    49);
    Graphics_drawLineH(&g_sContext, 19,        109,    79);
    Graphics_drawLineV(&g_sContext, 49,        19,     109);
    Graphics_drawLineV(&g_sContext, 79,        19,     109);
/*
    // Draw standard X O
    if (map[0] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1, 32, 32,
                                    OPAQUE_TEXT);
    }
    if (map[0] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1, 32, 32,
                                    OPAQUE_TEXT);
    }
    if (map[1] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1, 64, 32,
                                    OPAQUE_TEXT);
    }
    if (map[1] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1, 64, 32,
                                    OPAQUE_TEXT);
    }
    if (map[2] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1, 96, 32,
                                    OPAQUE_TEXT);
    }
    if (map[2] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1, 96, 32,
                                    OPAQUE_TEXT);
    }
    if (map[3] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1, 32, 64,
                                    OPAQUE_TEXT);
    }
    if (map[3] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1, 32, 64,
                                    OPAQUE_TEXT);
    }
    if (map[4] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1, 64, 64,
                                    OPAQUE_TEXT);
    }
    if (map[4] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1, 64, 64,
                                    OPAQUE_TEXT);
    }
    if (map[5] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1, 64, 96,
                                    OPAQUE_TEXT);
    }
    if (map[5] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1, 64, 96,
                                    OPAQUE_TEXT);
    }
    if (map[7] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1, 96, 64,
                                    OPAQUE_TEXT);
    }
    if (map[7] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1, 96, 64,
                                    OPAQUE_TEXT);
    }
    if (map[8] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1, 96, 96,
                                    OPAQUE_TEXT);
    }
    if (map[8] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1, 96, 96,
                                    OPAQUE_TEXT);
    }
    if (map[6] == cross)
    {
        Graphics_setForegroundColor(&g_sContext, XCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1, 96, 32,
                                    OPAQUE_TEXT);
    }
    if (map[6] == circle)
    {
        Graphics_setForegroundColor(&g_sContext, OCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1, 96, 32,
                                    OPAQUE_TEXT);
    }
    if ((map[0] == empty)||(map[1] == empty)||(map[2] == empty)||(map[3] == empty)||(map[4] == empty)
           || (map[5] == empty)||(map[6] == empty)||(map[7] == empty)||(map[8] == empty))
    {
    }
        */
        Graphics_setForegroundColor(&g_sContext, FOREGROUNDCOLOR);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);

        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);

        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);


}
//this draw board function is used for screen saver
void DrawBoardIdle(tcellstate map[9]) {
    int8_t *symbolMap[3] = {" ", "X", "O"};

    Graphics_setForegroundColor(&g_sContext, LINECOLOR);
    Graphics_drawLineH(&g_sContext, 19,        109,    49);
    Graphics_drawLineH(&g_sContext, 19,        109,    79);
    Graphics_drawLineV(&g_sContext, 49,        19,     109);
    Graphics_drawLineV(&g_sContext, 79,        19,     109);

    // Draw standard X O
    Graphics_setForegroundColor(&g_sContext, FOREGROUNDCOLOR);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);

    Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);

    Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
}

//highlight winning row and col
void DrawWinner(tcellstate map[9], int winner, uint32_t color) {
    int8_t *symbolMap[3] = {" ", "X", "O"};

    Graphics_setForegroundColor(&g_sContext, color);

    switch (winner) {
    case 1: // row 1
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
        break;
    case 2: // row 2
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);
        break;
    case 3: // row 3
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
        break;
    case 4: // col 1
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[3]], -1,  32,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        break;
    case 5: // col 2
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[1]], -1,  64,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[7]], -1,  64,  96, OPAQUE_TEXT);
        break;
    case 6: // col 3
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[5]], -1,  96,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
        break;
    case 7: // diagonal
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[0]], -1,  32,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[8]], -1,  96,  96, OPAQUE_TEXT);
        break;
    case 8: // back diagonal
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[2]], -1,  96,  32, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[4]], -1,  64,  64, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, symbolMap[map[6]], -1,  32,  96, OPAQUE_TEXT);
        break;
    }

}

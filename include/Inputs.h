#ifndef KEYS_H
#define KEYS_H

typedef struct Inputs
{
    bool keyPressedW = false;
    bool keyPressedA = false;
    bool keyPressedS = false;
    bool keyPressedD = false;
    bool keyPressedLeftShift = false;
    bool keyPressedLeftControl = false;
    bool keyPressedSpace = false;
    double cursorYPos;
    double cursorXPos;
} Inputs;
#endif
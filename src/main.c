#include "header.h"

void processDrawing();
void closeApp();

// in-development formatting for in-house documentation helper
/**********************************************************
* Entry Point
*    - Used to handle incoming arguments
*    - Used to orchestrate the calls to other files, but does a minimal amount of processing itself.
* argc    ::    Count the number of arguments. 
* argv    ::    Array of argument entries. argv[0] always equals the application name.
**********************************************************/ 

// TEMPS
float cToF(float c) { return (c * 1.8) + 32; }
float fToC(float f) { return (f - 32) / 1.8; }

float mmToInch(float mm) { return mm / 25.4; }
float inchToMM(float inch) { return inch * 25.4; }

float ozToGram(float oz) { return oz * 28.35; }
float gramToOZ(float gram) { return gram / 28.35; }

void processInput();
void checkMouseCollisions();

extern int activeElementsLength;



int main()
{
/*    ON INIT
******************************************************/
    if(!initUI()){ return -1; }
    
    
/*    ON TICK
******************************************************/
    while (!WindowShouldClose())
    {
        processInput();
        processDrawing();
        //checkMouseCollisions();
    }
       
/*    ON END
******************************************************/
    closeApp();
    return 0;
}


/*    PROCESSORS
******************************************************/
void processInput()
{
    // handling these with bound events
    // if (IsKeyPressed(KEY_H)) { switchTool(Home); }
    // if (IsKeyPressed(KEY_F)) { switchTool(Calculator); }
}


void processDrawing()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        drawUI();
    }
    EndDrawing();
}


void closeApp()
{
    CloseWindow();
}


void checkMouseCollisions()
{
    Vector2 mousePoint = GetMousePosition();
    
    bool result = false;
    
    // check if mouse overlaps a control in the tool
    for (int i = 0; i < activeElementsLength; i++)
    {
        result = CheckCollisionPointRec(mousePoint, getControlBounds(i));
        if (result)
        {
            hoverElement(i);
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { clickElement(i); break;}
            break;
        } else { unhoverElement(i); }  
    }

    // COULD optimize by checking if mouse.posX is far enough left to be in navbar area, and not run the check otherwise
    // check if mouse overlaps a control in the navbar
    for (int i = 0; i < 5; i++)
    {
        result = CheckCollisionPointRec(mousePoint, getNavControlBounds(i));
        
        if (result)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { clickNavElement(i); break; }
            hoverNavElement(i);
            break;
        } else { unhoverNavElement(i); }
    }
}
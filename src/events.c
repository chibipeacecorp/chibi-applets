#include "core.h"

/*
    Functions that directly change state of elements, without needing to know who/where the element is
*/

//extern e_Module currentTool;
//extern e_Module lastTool;

int lastTool = 0;
int currentTool = 0;


/*
A documented list if allowedEvents = {EID_ENABLE_ELEMENT = 1} should exist for simplicity?

EXAMPLE: toggle unrelated element's state when another element is hovered/unhovered

hoverElement() --> set's element.state = hovered

getElementHoverEventID(element_ptr) --> if !null --> triageHoverEventByID(ID)


triageHoverEventByID(instigator, affected*) // affected as a pointer to head of array of unknown size

eventOfID():

*/

// EID_HIDE_GROUP
// void hideElementGroup(int toolID, int* elementsList)
// {
//     /*
//         for each in 
//     */
// }


void triageHoverEventID(int ID)
{
    switch(ID)
    {
        case 1: { /* hoverFunct1() */ }
        default: { /*do nothing*/ }
    }
}


void setAciveTool(int newTool)
{
    lastTool = currentTool;
    currentTool = newTool;
}


void handleClickEvent(e_EventID id)
{   
    if (id == 0) {printf(".");}
    // switch(id)
    // {
    //     case EID_SHOW_HOME: { temp = Home; } break;
    //     case EID_SHOW_CALC: { temp = Calculator; } break;
    //     default: {/*do nothing*/}
    // }
    
    // setAciveTool(temp);
}


// should take ptr or ID???
// hover existing is immutable
// what can hover, and what happens when hover, is mutable
void hover() { }


void changeTool(int newToolID) { lastTool = currentTool; currentTool = newToolID; }
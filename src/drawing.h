#pragma once
#include "core.h"

// CONFIG AND SETUP
bool initUI();
void drawUI();


// Element Events
Rectangle getControlBounds(int index);
void hoverElement(int index);
void unhoverElement(int index);
void clickElement(int index);

Rectangle getNavControlBounds(int index);
void clickNavElement(int index);
void hoverNavElement(int index);
void unhoverNavElement(int index);
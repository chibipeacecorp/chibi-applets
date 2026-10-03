#pragma once

// CONSTANTS
#define MAX_MODULE_ELEMENTS 100
#define MAX_MODULES 5
#define MAX_CHILD_ELEMENTS 50
// likely needs to be much higher, but testing for now


// STD
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// TOML
#include "tomlc17.h"

// RAYSTUFF
#include <raylib.h>

// CORE 
#include "coreTypes.h"


// Utilities
int clampAddInt(int val, int adder, int max);
int clampSubInt(int val, int subber, int min);
int invertInt(int val);
int clampInt(int val, int min, int max);
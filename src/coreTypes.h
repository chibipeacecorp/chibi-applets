#pragma once
#include "core.h"



/*    ENUMS    ******************************************************/
typedef enum
{
  EID_NULL = 0,
  EID_CLOSE_APP,
  EID_SHOW_HOME,
  EID_SHOW_CALC,
  EID_HIDE_GROUP
  /*
  any system / OS function
  */
} e_EventID;

typedef enum
{
  TYPE_BTTN,
  TYPE_PANEL,
  MAX_TYPE
} e_ElementType;

// Likely needs replaced with something more flexible, like reading x-y floats from modules directly
typedef enum
{
  SIZE_H1,        // 100%
  SIZE_H2,        // 80%
  SIZE_H3,        // 60%
  SIZE_H4,        // 40%
  SIZE_H5,        // 20%
  SIZE_H6,        // 10%
  SIZE_P,         // 5%
  MAX_SIZE
}e_ElementSize;

typedef enum : uint8_t
{
  DISABLED = 1,
  IDLE = 2,
  HOVERED = 4,
  CLICKED = 8,
  SELECTED = 16,
  SELECTED_HOVERED = SELECTED|HOVERED,
  SELECTED_CLICKED = SELECTED|CLICKED
} e_State;


typedef enum : uint16_t
{
  CENTER = 1,
  LEFT = 2,
  RIGHT = 4,
  TOP = 8,
  BOTTOM = 16,
  CENTER_LEFT = CENTER|LEFT,
  CENTER_RIGHT = CENTER|RIGHT,
  CENTER_TOP = CENTER|TOP,
  CENTER_BOTTOM = CENTER|BOTTOM,
  TOP_LEFT = TOP|LEFT,
  TOP_RIGHT = TOP|RIGHT,
  BOTTOM_LEFT = BOTTOM|LEFT,
  BOTTOM_RIGHT = BOTTOM|RIGHT,
  ALIGN_ABSOLUTE = 32,
} e_Alignment;


typedef enum
{
  FILL,           // Default: fills entire space with element, respects siblings
  FILL_X,         // Horizontal fill, respects siblings
  FILL_Y,         // Vertical fill, respects siblings
  FILL_ABS,       // Ignore everything, fill parent
  MAX_FILL
}e_ElementFill;

typedef enum
{
  baseColor,
  outlineColor,
  textColor
} e_ThemeColorType;

typedef enum
{
  KEY_BOUNDS,
  KEY_PADDING,
  KEY_TEXT,
  KEY_TYPE,
  KEY_ALIGN,
  KEY_FILL,
  KEY_STATE,
  KEY_VISIBILITY,
  KEY_SIZE,         // used for h1-h6 and other design-common tags
  KEY_RESIZABLE,
  MAX_KEY           // used as null key, as it is never valid unless as a loop comparison
}e_ModuleKey;


typedef enum
{
  CON_SINGLE,       // single child container
  CON_HBOX,         // handle entire container as a single row
  CON_VBOX,         // handle entire container as a single column
  CON_TILE,         // a more literal, uniform grid
  MAX_CON           // defaults to single
}e_ElementContainer;

/*    STRUCTS    ******************************************************/
typedef struct
{
    int x;
    int y;
} IntVector2;


typedef struct
{
  IntVector2 size;
  bool resizable;
} s_ModuleConfig;


typedef struct
{
  Color disabled;
  Color idle;
  Color hovered;
  Color clicked;
  Color selected;
  Color selectedHovered;
} s_ThemeColors;

typedef struct
{
  s_ThemeColors base;
  s_ThemeColors outline;
  s_ThemeColors text;
  float outlineWidth;
  int fontID;
  int fontSize;
} s_ThemeData;


#warning "does a union of flags make sense?"
typedef struct 
{
  char* text;
  Rectangle bounds;
  int children[10];
  int padding[4];
  float size;
  int elementID;
  int parentID;           
  e_ElementType type;
  e_Alignment align;
  e_ElementFill fill;
  e_State state;
  e_ElementContainer containerType;
  e_EventID eid_OnHover;
  e_EventID eid_OnClick;
  bool visibility;
  bool resizable;
} s_Element;

typedef struct
{
  s_ModuleConfig config;
  s_Element elements[MAX_MODULE_ELEMENTS];
} s_Module;

typedef struct
{
  s_Module modules[MAX_MODULES];
  s_ThemeData theme[MAX_TYPE];
} s_UIArena;
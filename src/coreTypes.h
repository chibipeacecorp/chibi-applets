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

typedef enum
{
  STATE_DISABLED = 1 << 0,
  STATE_IDLE = 1 << 1,
  STATE_HOVERED = 1 << 2,
  STATE_CLICKED = 1 << 3,
  STATE_SELECTED = 1 << 4,
  MAX_STATE = 1 << 5
} e_ElementState;

typedef enum
{
  ALIGN_CENTER = 1 << 0,
  ALIGN_LEFT = 1 << 1,
  ALIGN_RIGHT = 1 << 2,
  ALIGN_TOP = 1 << 3,
  ALIGN_BOTTOM = 1 << 4,
  ALIGN_ABS = 1 << 5,
  MAX_ALIGN = 1 << 6
} e_ElementAlign;

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


// bit-masked in case a single att-value can have more than one value
typedef enum
{
  ATT_STRING = 1 << 0,
  ATT_INT = 1 << 1,
  ATT_FLOAT = 1 << 2,
  ATT_BOOL = 1 << 3
}e_AttributeType;


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


// would a union be performant or useful here?  
typedef struct
{
  // e_AttributeType att_Type; 
  const char* stringVal;
  int intVal;
  float floatVal;
  Rectangle shape;
  bool boolVal;
}s_ModuleAttributeValue;

typedef struct
{
  e_ModuleKey key;
  s_ModuleAttributeValue val;
}s_ModuleAttribute;

typedef struct 
{
  s_ModuleAttribute padding;
  s_ModuleAttribute bounds;
  s_ModuleAttribute text;                         
  s_ModuleAttribute type;
  s_ModuleAttribute align;
  s_ModuleAttribute fill;
  s_ModuleAttribute state;
  s_ModuleAttribute visibility;
  s_ModuleAttribute size;
  s_ModuleAttribute resizable;
  e_EventID eid_OnHover;
  e_EventID eid_OnClick;
  int parentID;
  s_Element children[10];
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
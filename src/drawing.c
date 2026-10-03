#include "core.h"
#include "events.h"
#include "modules.h"
#include "theme.h"


int activeElementsLength = 10;
s_UIArena arena = {0};
s_Element navBar[6] = {0};


// Setup
void buildNavBar(s_Element base);


// Generic elements
s_Element init_GenericPanel();


// Drawing elements
void drawElement(s_Element* element);
void drawTextElement(s_Element* element);
void drawPanel(s_Element* element);
void drawButton(s_Element* button);


// Element Events
void hoverElement(int index);
void unhoverElement(int index);
void clickElement(int index);


// Element Margins, Padding, and position math
IntVector2 getElementCenter(s_Element* element); 
IntVector2 getElementLeft(s_Element* element);
IntVector2 getElementRight(s_Element* element);

//another test file



/*    INIT
*******************************************************************************/
/*
Every module should have a config, context, and content

***MODULES DO NOT know about any navigational panel
The Nav element can be set to top or side, BUT
it is hardcoded in, using theme, and being generated based on the # of module files.
*/
bool initUI()
{
    getThemeData();
    s_ModuleConfig config = getModuleConfig();

    if ( config.size.x <= 0 || config.size.y <= 0 ) { return false; }
    InitWindow(config.size.x, config.size.y, "Chibi Calc");

    // test fill of first entry in elements
    arena.modules[0].elements[0] = getElementFromToml(getModuleContent());

    s_Element parent = init_GenericPanel();
    parent.bounds.val.shape = (Rectangle){0, 0, config.size.x, config.size.y}; 
    buildNavBar(parent);


    freeModFiles();
    return true;
}

/*
For every module,
 - get the current top-level element
 - create one, and pass it back to drawing
 - drawing caches it in the correct module arena member
 - drawing then handles it on the fly


// can use an upper limit of 5 modules for now, and 100 for elements per module and pray it aint too bad

*/

//#define INIT_ATT(KEY_VAL) (s_ModuleAttribute){.key=KEY_VAL, .val=(s_ModuleAttributeValue){0}}

float sizeToFloat(e_ElementSize size)
{
    float temp = 0;
    
    
    switch(size)
    {
        case SIZE_H1: {temp = 1.0;} break;
        case SIZE_H2: {temp = 0.8;} break;
        case SIZE_H3: {temp = 0.6;} break;
        case SIZE_H4: {temp = 0.4;} break;
        case SIZE_H5: {temp = 0.2;} break;
        case SIZE_H6: {temp = 0.1;} break;
        case SIZE_P: {temp = 0.05;} break;
        default: {}
    }
    
    return temp;
}


void setPositionByAlignment(s_Element* parent, s_Element* child)
{
    // gets the top-left point of the parent object
    // might as well be a fucking rect
    Vector2 tempPosition =
    {
        parent->bounds.val.shape.x,
        parent->bounds.val.shape.y
    };
    Vector2 tempSize =
    {
        parent->bounds.val.shape.width * sizeToFloat(child->size.val.intVal),
        parent->bounds.val.shape.height * sizeToFloat(child->size.val.intVal)
    };

    int xOffset = 0;
    int yOffset = 0;

    switch(child->align.val.intVal)
    {
        case ALIGN_CENTER:
        {
            // xOffset = ;
            // yOffset = ;
        } break;

        case ALIGN_TOP|ALIGN_LEFT: { /*default position*/ } break;

        case ALIGN_TOP|ALIGN_RIGHT:
        {
            xOffset = parent->bounds.val.shape.width - tempSize.x;
        }break;

        default: {}
    }

    tempPosition.x += xOffset;
    tempPosition.y += yOffset;
    child->bounds.val.shape = (Rectangle) {tempPosition.x, tempPosition.y, tempSize.x, tempSize.y};
}


void setPositionByFill(s_Element* parent, s_Element* child)
{    
    switch(child->fill.val.intVal)
    {
        case FILL:
        {
            child->bounds.val.shape = parent->bounds.val.shape;
        } break;
        case FILL_X:
        {
            child->bounds.val.shape.x = parent->bounds.val.shape.x;
            child->bounds.val.shape.y = parent->bounds.val.shape.y;
            child->bounds.val.shape.width = parent->bounds.val.shape.width;
            child->bounds.val.shape.height = parent->bounds.val.shape.height * sizeToFloat(child->size.val.intVal);
        } break;
        default: {};
    }
}

#warning "Padding is currently absolute, and does not preserve dimensions of element"
void positionElementByPadding(s_Element* element)
{
    if (element->padding.val.shape.y != 0)      // top
    { element->bounds.val.shape.y += element->padding.val.shape.y; }

    if (element->padding.val.shape.x != 0)      // left
    { element->bounds.val.shape.x += element->padding.val.shape.x; }

    if (element->padding.val.shape.height != 0) // bottom
    { element->bounds.val.shape.height -= element->padding.val.shape.height; }

    if (element->padding.val.shape.width != 0)  // right
    { element->bounds.val.shape.width -= element->padding.val.shape.width; }
}

/*
    if MAX_FILL {alignment + size} 
    if FILL_ABS {bounds = parent.bounds}
    if FILL {getRemainingSiblingSpace()}
    if FILL_X {getRemainingSiblingSpace()->fill_X}
    if FILL_Y {getRemainingSiblingSpace()->fill_Y}

    processPadding([50,0,50,0])
    TOP     = bounds.y + pad.y
    LEFT    = bounds.x + pad.x
    BOTTOM  = bounds.height - pad.height
    RIGHT   = bounds.width - pad.width
*/
void positionChildInParent(s_Element* parent, s_Element* child)
{
    if (child->fill.val.intVal == MAX_FILL) // this is treated as a null value
    {setPositionByAlignment(parent, child); positionElementByPadding(child); return;}

    if (child->fill.val.intVal == FILL_ABS)
    {child->bounds = parent->bounds; positionElementByPadding(child); return;}

    setPositionByFill(parent, child);
    positionElementByPadding(child);
}

void buildNavBar(s_Element base)
{
    s_Element nav_panel = init_GenericPanel();
    nav_panel.fill.val.intVal = FILL_X;
    nav_panel.align.val.intVal = ALIGN_CENTER;
    nav_panel.size.val.intVal = SIZE_H6;
    nav_panel.padding.val.shape = (Rectangle) {0,0,0,0};
    positionChildInParent(&base, &nav_panel);
    navBar[0] = nav_panel;

    s_Element nav_button = init_GenericPanel();
    nav_button.type.val.intVal = TYPE_BTTN;
    nav_button.fill.val.intVal = MAX_FILL;
    nav_button.size.val.intVal = SIZE_H5;
    nav_button.align.val.intVal = ALIGN_TOP|ALIGN_RIGHT;
    positionChildInParent(&nav_panel, &nav_button);
    navBar[1] = nav_button;


    // for (int i = 0; i < MAX_MODULES; i++)
    // {

    // }
}


/*    TICK
*******************************************************/
void drawUI()
{    
    DrawFPS(400, 500);

    // navbar
    drawElement(&navBar[0]);
    drawElement(&navBar[1]);

    // module content
    // get currently active module
    // for (int i = 0; i < MODULE_SIZE; i++)
    //s_Element* elePtr = &arena.modules[0].elements[0];
    //drawElement(&arena.modules[0].elements[0]);


    // maybe some copyright or other info. system usage?
}



/*    EVENTS
*******************************************************/
Rectangle getControlBounds(int index)
{
    int val = 0 * index;
    Rectangle temp = {val, 0, 0, 0};
    return temp;
}


/*    DRAWING
*******************************************************/
void drawElement(s_Element* element)
{
    // drop elements with an invalid type
    if(element->type.key != KEY_TYPE) {return;}
    if(element->type.val.intVal >= MAX_TYPE) {printf("invalid type val"); return;}
    
    switch( element->type.val.intVal )
    {
        case TYPE_BTTN: {drawButton(element);} break;
        case TYPE_PANEL: {drawPanel(element);} break;
        default: {}
    }
}

void drawTextElement(s_Element* element)
{
    DrawText(element->text.val.stringVal, element->bounds.val.shape.x, element->bounds.val.shape.y, 12, WHITE);
}

void drawPanel(s_Element* element)
{
    Color base = {0};
    Color outline = {0};
    s_ThemeData* ptr_Theme = &arena.theme[element->type.val.intVal];

    switch(element->state.val.intVal)
    {
        case STATE_DISABLED:
        {
            base = ptr_Theme->base.disabled;
            outline = ptr_Theme->outline.disabled;
        } break;
        case STATE_IDLE:
        {
            base = ptr_Theme->base.idle;
            outline = ptr_Theme->outline.idle;
        } break;
        case STATE_CLICKED:
        {
            base = ptr_Theme->base.clicked;
            outline = ptr_Theme->outline.clicked;
        } break;
        case STATE_HOVERED:
        {
            base = ptr_Theme->base.hovered;
            outline = ptr_Theme->outline.hovered;
        } break;
        case STATE_SELECTED:
        {
            base = ptr_Theme->base.selected;
            outline = ptr_Theme->outline.selected;
        } break;
        case STATE_SELECTED & STATE_HOVERED:
        {
            base = ptr_Theme->base.selectedHovered;
            outline = ptr_Theme->outline.selectedHovered;
        } break;
        default: {} break;
    }

    
    DrawRectangleRec(element->bounds.val.shape, base);
    DrawRectangleLinesEx(element->bounds.val.shape, ptr_Theme->outlineWidth, outline);
    // drawTextElement(element);
}

void drawButton(s_Element* element)
{
    drawPanel(element);
}





/*    UPDATES
*******************************************************/
void hoverElement(int index)
{
    if (index != 0) {printf(".");}
}

void unhoverElement(int index)
{
    if (index != 0) {printf(".");}
}


void clickElement(int index)
{
    if (index != 0) {printf(".");}
}


/*    NAVIGATIONAL BAR
*******************************************************/
Rectangle getNavControlBounds(int index)
{
    int val = 0 * index;
    Rectangle temp = {val, 0, 0, 0};

    return temp;
}
void hoverNavElement(int index) { hoverElement(index); }
void unhoverNavElement(int index) { unhoverElement(index); }
void clickNavElement(int index) { clickElement(index); }



/*  ELEMENT GENERICS
*******************************************************************************/
s_Element init_GenericPanel()
{
    return (s_Element)
    {
        .padding = (s_ModuleAttribute)
        {
            .key=KEY_PADDING,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = 0,
                .floatVal = 0,
                .shape = (Rectangle){0,0,0,0},
                .boolVal = false
            }
        },
        .bounds = (s_ModuleAttribute)
        {
            .key=KEY_BOUNDS,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = 0,
                .floatVal = 0,
                .shape = (Rectangle){0,0,25,25},
                .boolVal = false
            }
        },
        .text = (s_ModuleAttribute)
        {
            .key=KEY_TEXT,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "default",
                .intVal = 0,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        },
        .type = (s_ModuleAttribute)
        {
            .key=KEY_TYPE,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = TYPE_PANEL,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        },
        .align = (s_ModuleAttribute)
        {
            .key=KEY_ALIGN,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = ALIGN_CENTER,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        },
        .fill = (s_ModuleAttribute)
        {
            .key=KEY_FILL,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = MAX_FILL,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        },
        .state = (s_ModuleAttribute)
        {
            .key=KEY_STATE,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = STATE_IDLE,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        },
        .visibility = (s_ModuleAttribute)
        {
            .key=KEY_VISIBILITY,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = 0,
                .floatVal = 0,
                .shape = {0},
                .boolVal = true
            }
        },
        .size = (s_ModuleAttribute)
        {
            .key=KEY_SIZE,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = SIZE_H4,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        },
        .resizable = (s_ModuleAttribute)
        {
            .key=KEY_RESIZABLE,
            .val = (s_ModuleAttributeValue)
            {
                .stringVal = "",
                .intVal = 0,
                .floatVal = 0,
                .shape = {0},
                .boolVal = false
            }
        }
    };
}
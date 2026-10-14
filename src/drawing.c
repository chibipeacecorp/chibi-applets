#include "core.h"
#include "events.h"
#include "modules.h"
#include "theme.h"



s_UIArena arena = {0};
s_Element navBar[6] = {0};
int activeElementsLength = 10;
int currentModuleID = 0;

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

    arena.modules[0].elements[0] = getElementFromToml(getModuleContent());

    s_Element parent = init_GenericPanel();
    parent.bounds = (Rectangle){0, 0, config.size.x, config.size.y}; 
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

#warning "these std convertors can be bundle together elsewhere"
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

s_Module* getCurrentModule() { return &arena.modules[currentModuleID]; }
s_Element* getElementByChildID(s_Module* module, int id)
{
    if(module == NULL) { return &navBar[0]; }

    return &module->elements[id];
}

/*
Maybe an e_ContainerType to simplify what to look for
Most elements will be either single-child, or some type of box, vertical or horizontal. 
This inherently means that most elements will be cleanly aligned through-out the module, without needing much direct changing. think { [{ [] }] []}

At the most complex, a non-uniform grid container looks like:
each cell in the grid has a %
each alignment takes from a cell,
fill takes from a row/column
size determines amount of cell is taken, assuming no fill
*/
Rectangle getNextAvailableSpace(s_Element* parent)
{
    if(parent->containerType == CON_SINGLE) {return parent->bounds;}
    /*
    CONSTRAINED container types like hbox, forcefully set
    the size of it's children
    ergo, if there is such a parent, skip the fill/size check for children
    align is ignored, tho the parent container can be aligned
    provided it isnt also within a constrained parent
    */
    
    // divisor
    int presentChildren = 0;
    int childCounter = 0;
    int presentChildIDs[10] = {-1};
    s_Element* currentChild = 0;

    for(int i = 0; i < 10; i++)
    {
        currentChild = getElementByChildID(getCurrentModule(), parent->children[i]);

        // get valid children only; increment divisor
        if( (presentChildIDs[i] = currentChild->elementID) < 0) {continue;}
        presentChildren++;
   
    }
            
    switch(parent->containerType)
    {
        case CON_HBOX:
        {
            for(int i = 0; i < 10; i++)
            {    
                if(presentChildIDs[i] < 0) {continue;}
                currentChild->bounds.width = parent->bounds.width / presentChildren;
                currentChild->bounds.y = parent->bounds.y;
                currentChild->bounds.x = parent->bounds.x + (currentChild->bounds.width * childCounter);
                childCounter++;
            }
        }break;
        
        case CON_VBOX:
        {
            for(int i = 0; i < 10; i++)
            {    
                if(presentChildIDs[i] < 0) {continue;}
                currentChild->bounds.height = parent->bounds.height / presentChildren;
                currentChild->bounds.x = parent->bounds.x;
                currentChild->bounds.y = parent->bounds.y + (currentChild->bounds.height * childCounter);
                childCounter++;
            }   
        }break;
        
        case CON_TILE:
        {
            
        }break;
        
        default: {/*do nothing*/};
    }
    
}


void setPositionByAlignment(s_Element* parent, s_Element* child)
{
    // gets the top-left point of the parent object
    Vector2 tempPosition =
    {
        parent->bounds.x,
        parent->bounds.y
    };
    Vector2 tempSize =
    {
        parent->bounds.width*sizeToFloat(child->size),
        parent->bounds.height*(sizeToFloat(child->size)*2)
        #warning "Currently, element y scale is being bumped up with a hard-coded value"
    };

    int xOffset = 0;
    int yOffset = 0;
    e_Alignment testAlign = CENTER|RIGHT;

    switch(testAlign)
    {
        case CENTER: {/**/} break;
        case CENTER|RIGHT: {/**/} break;
        case RIGHT: {/**/} break;
        default: {/*yur*/}
    }

    switch(child->align)
    {
        case ALIGN_CENTER: // x=(p.w/2)-(c.w/2) ; y=(p.h/2)-(c.h/2)
        {
            xOffset = (parent->bounds.width / 2) - (tempSize.x / 2);
            yOffset = (parent->bounds.height / 2) - (tempSize.y / 2);
        } break;

        case ALIGN_TOP&ALIGN_LEFT: { /*default position*/ } break;

        case ALIGN_TOP&ALIGN_RIGHT:
        {
            xOffset = parent->bounds.width - tempSize.x;
        }break;

        default: {}
    }

    tempPosition.x += xOffset;
    tempPosition.y += yOffset;
    child->bounds = (Rectangle) {tempPosition.x, tempPosition.y, tempSize.x, tempSize.y};
}


void setPositionByFill(s_Element* parent, s_Element* child)
{    
    switch(child->fill)
    {
        case FILL:
        {
            child->bounds = parent->bounds;
        } break;
        case FILL_X:
        {
            child->bounds.x = parent->bounds.x;
            child->bounds.y = parent->bounds.y;
            child->bounds.width = parent->bounds.width;
            child->bounds.height = parent->bounds.height * sizeToFloat(child->size);
        } break;
        default: {};
    }
}

#warning "Padding is currently absolute, and does not preserve dimensions of element"
void positionElementByPadding(s_Element* element)
{
    if (element->padding[0] != 0)      // 
    { element->bounds.x += element->padding[0]; }

    if (element->padding[1] != 0)      // 
    { element->bounds.y += element->padding[1]; }

    if (element->padding[2] != 0) // 
    { element->bounds.height -= element->padding[2]; }

    if (element->padding[3] != 0)  // 
    { element->bounds.width -= element->padding[3]; }
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
    if (child->fill == MAX_FILL) // this is treated as a null value
    {setPositionByAlignment(parent, child); positionElementByPadding(child); return;}

    if (child->fill == FILL_ABS)
    {child->bounds = parent->bounds; positionElementByPadding(child); return;}

    setPositionByFill(parent, child);
    positionElementByPadding(child);
}

void buildNavBar(s_Element base)
{
    #warning "pass IDs into the init_Element() functions to allow for easy parenting/childing"
    s_Element nav_panel = init_GenericPanel();
    nav_panel.fill = FILL_X;
    nav_panel.align = ALIGN_CENTER;
    nav_panel.size = SIZE_H6;
    nav_panel.padding = {0};
    positionChildInParent(&base, &nav_panel);
    nav_panel.elementID = 0;
    navBar[nav_panel.elementID] = nav_panel;

    s_Element nav_button = init_GenericPanel();
    nav_button.type = TYPE_BTTN;
    nav_button.fill = MAX_FILL;
    nav_button.size = SIZE_H5;
    nav_button.align = ALIGN_CENTER;
    positionChildInParent(&nav_panel, &nav_button);
    nav_button.elementID = 1;
    nav_button.parentID = nav_panel.elementID;
    navBar[nav_button.elementID] = nav_button;


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
    // drop elements with an invalid types
    if(element->type == MAX_TYPE) {printf("invalid type val"); return;}
    
    switch( element->type )
    {
        case TYPE_BTTN: {drawButton(element);} break;
        case TYPE_PANEL: {drawPanel(element);} break;
        default: {/*if this happend something very broke*/}
    }
}

void drawTextElement(s_Element* element)
{
    DrawText(element->text, element->bounds.x, element->bounds.y, 12, WHITE);
}

void drawPanel(s_Element* element)
{
    Color base = {0};
    Color outline = {0};
    s_ThemeData* ptr_Theme = &arena.theme[element->type];

    switch(element->state)
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

    
    DrawRectangleRec(element->bounds, base);
    DrawRectangleLinesEx(element->bounds, ptr_Theme->outlineWidth, outline);
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
        .parentID = -1,
        .elementID = -1,
        .padding[4] = {0},
        .bounds = (Rectangle) {50,50,50,50},
        .text = "test",
        .type = TYPE_PANEL,
        .align = ALIGN_CENTER,
        .fill = FILL,
        .state = STATE_IDLE,
        .visibility = true,
        .size = 1.0,
        .resizable = false
    };
}
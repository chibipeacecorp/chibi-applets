#include "core.h"
#include "include/win/windows_base.h"
#include "include/win/file.h"
#define SET_ELEMENT_KEY(element, key, val) element.KEY_TO_MEMBER(key) = val

void debugToml(char* src, toml_datum_t toml)
{
    printf("From: %s\nType: %d\n", src, toml.type);
    printf("int: %lld\n", toml.u.int64);
    printf("bool: %s\n", toml.u.boolean ? "True" : "False");
    printf("Type: %s\n", toml.u.s);
}


/*    ENUM CONVERTORS
*******************************************************************************/
char* keyToString(e_ModuleKey key)
{
    char* temp = "0";
    
    switch(key)
    {
        case KEY_BOUNDS: {temp = "bounds";} break;
        case KEY_PADDING: {temp = "padding";} break;
        case KEY_TEXT: {temp = "text";} break;
        case KEY_TYPE: {temp = "type";} break;
        case KEY_ALIGN: {temp = "align";} break;
        case KEY_FILL: {temp = "fill";} break;
        case KEY_STATE: {temp = "state";} break;
        case KEY_VISIBILITY: {temp = "visibility";} break;
        case KEY_SIZE: {temp = "size";} break;
        case KEY_RESIZABLE: {temp = "resizable";} break;
        default: {/*do nothing*/}
    }
    
    return temp;
}

e_ModuleKey stringToKey(const char* string)
{
    e_ModuleKey key = MAX_KEY;
    
    if(strcmp(string, "bounds") == 0) {return KEY_BOUNDS;}
    if(strcmp(string, "padding") == 0) {return KEY_PADDING;}
    if(strcmp(string, "text") == 0) {return KEY_TEXT;}
    if(strcmp(string, "type") == 0) {return KEY_TYPE;}
    if(strcmp(string, "align") == 0) {return KEY_ALIGN;}
    if(strcmp(string, "fill") == 0) {return KEY_FILL;}
    if(strcmp(string, "state") == 0) {return KEY_STATE;}
    if(strcmp(string, "visibility") == 0) {return KEY_VISIBILITY;}
    if(strcmp(string, "size") == 0) {return KEY_SIZE;}
    if(strcmp(string, "resizable") == 0) {return KEY_RESIZABLE;}

    return key;
}

/*    TOML CONVERTORS
*******************************************************************************/
e_ElementType tomlToElementType(toml_datum_t toml)
{
    e_ElementType temp = MAX_TYPE;
    if(toml.type != TOML_STRING) {return temp;}

    if(strcmp(toml.u.s, "button") == 0) {return TYPE_BTTN;}
    if(strcmp(toml.u.s, "panel") == 0) {return TYPE_PANEL;}

    return temp;
}

e_ElementState tomlToElementState(toml_datum_t toml)
{
    e_ElementState temp = MAX_STATE;
    if(toml.type != TOML_STRING) {return temp;}

    if(strcmp(toml.u.s, "disabled") == 0) {return STATE_DISABLED;}
    if(strcmp(toml.u.s, "idle") == 0) {return STATE_IDLE;}
    if(strcmp(toml.u.s, "hovered") == 0) {return STATE_HOVERED;}
    if(strcmp(toml.u.s, "clicked") == 0) {return STATE_CLICKED;}
    if(strcmp(toml.u.s, "selected") == 0) {return STATE_SELECTED;}

    return temp;
}

e_ElementAlign tomlToElementAlign(toml_datum_t toml)
{
    e_ElementAlign temp = MAX_ALIGN;
    if(toml.type != TOML_STRING) {return temp;}

    if(strcmp(toml.u.s, "center") == 0) {return ALIGN_CENTER;}
    if(strcmp(toml.u.s, "left") == 0) {return ALIGN_LEFT;}
    if(strcmp(toml.u.s, "right") == 0) {return ALIGN_RIGHT;}
    if(strcmp(toml.u.s, "top") == 0) {return ALIGN_TOP;}
    if(strcmp(toml.u.s, "bottom") == 0) {return ALIGN_BOTTOM;}
    if(strcmp(toml.u.s, "absolute") == 0) {return ALIGN_ABS;}


    return temp;
}

e_ElementFill tomlToElementFill(toml_datum_t toml)
{
    e_ElementFill temp = MAX_FILL;
    if(toml.type != TOML_STRING) {return temp;}

    if(strcmp(toml.u.s, "fill") == 0) {return FILL;}
    if(strcmp(toml.u.s, "horizontal") == 0) {return FILL_X;}
    if(strcmp(toml.u.s, "vertical") == 0) {return FILL_Y;}
    if(strcmp(toml.u.s, "absolute") == 0) {return FILL_ABS;}

    return temp;
}

e_ElementSize tomlToElementSize(toml_datum_t toml)
{
    e_ElementSize temp = MAX_SIZE;
    if(toml.type != TOML_STRING) {return temp;}

    if(strcmp(toml.u.s, "h1") == 0) {return SIZE_H1;}
    if(strcmp(toml.u.s, "h2") == 0) {return SIZE_H2;}
    if(strcmp(toml.u.s, "h3") == 0) {return SIZE_H3;}
    if(strcmp(toml.u.s, "h4") == 0) {return SIZE_H4;}
    if(strcmp(toml.u.s, "h5") == 0) {return SIZE_H5;}
    if(strcmp(toml.u.s, "h6") == 0) {return SIZE_H6;}
    if(strcmp(toml.u.s, "p") == 0) {return SIZE_P;}

    return temp;
}

const char* tomlToString(toml_datum_t toml)
{
    const char* temp = "0";
    if(toml.type != TOML_STRING){return temp;}

    temp = toml.u.s;
    
    return temp;
}

IntVector2 tomlToIntVect2(toml_datum_t toml)
{
    IntVector2 temp = {0};
    if(toml.type != TOML_ARRAY) {return temp;}

    toml_datum_t x = toml.u.arr.elem[0];
    toml_datum_t y = toml.u.arr.elem[1];
    if(x.type != TOML_INT64 || y.type != TOML_INT64)
    {
        printf("array contains non-integer\n");
        return temp;
    }

    temp.x = (int)x.u.int64;
    temp.y = (int)y.u.int64;
    
    return temp;
}

Rectangle tomlToRect(toml_datum_t toml)
{
    Rectangle temp = {0};
    if(toml.type != TOML_ARRAY) {return temp;}

    toml_datum_t x = toml.u.arr.elem[0];
    toml_datum_t y = toml.u.arr.elem[1];
    toml_datum_t w = toml.u.arr.elem[2];
    toml_datum_t h = toml.u.arr.elem[3];

    if( x.type != TOML_INT64 || y.type != TOML_INT64 ||
        w.type != TOML_INT64 || h.type != TOML_INT64 )
    {
        printf("array contains non-integer\n");
        return temp;
    }

    temp.x = (int)x.u.int64;
    temp.y = (int)y.u.int64;
    temp.width = (int)w.u.int64;
    temp.height = (int)h.u.int64;

    return temp;
}

int tomlToInt(toml_datum_t toml)
{
    int temp = 0;
    if(toml.type != TOML_INT64){return temp;}

    temp = (int)toml.u.int64;
    
    return temp;
}

bool tomlToBool(toml_datum_t toml)
{
    bool temp = false;
    if(toml.type != TOML_BOOLEAN){return temp;}

    temp = toml.u.boolean;
    
    return temp;
}


/*    FILE HANDLING
*******************************************************************************/
FILE* modFile;
toml_result_t modFileResult;


void freeModFiles()
{
    fclose(modFile);
    toml_free(modFileResult);
}


int getModulesCount()
{
    int temp = -1;
    // WIN32_FIND_DATAW find_data;
    // HANDLE found = FindFirstFileW("modules\\*.toml", &find_data);
    // if (found == INVALID_HANDLE_VALUE) {return temp;} // no modules
    
    // printf("module: %s found", find_data.cFileName);
    // // while (FindNextFile(h_find, &find_data) != 0)
    // // {
    // //     printf("module: %s found", find_data.cFileName);
    // // }

    // temp = 1;
    return temp;
}

s_ModuleConfig getAppConfig()
{
    s_ModuleConfig tempConfig =
    {
        .size = (IntVector2) { .x = 600, .y = 600 },
        .resizable = false
    };
    
    return tempConfig;
}

s_ModuleConfig getModuleConfig()
{
    s_ModuleConfig tempConfig =
    {
        .size = (IntVector2) { .x = 600, .y = 600 },
        .resizable = false
    };


    FILE* configFile = fopen("app.toml", "r+");
    toml_result_t configResult = toml_parse_file(configFile);
    if (!configResult.ok)
    {
        printf("failed to parse module!\n");
        freeModFiles();
        return tempConfig;
    }
    
    toml_datum_t config = toml_get(configResult.toptab, "config");
    for (int i = 0; i < MAX_KEY; i++)
    {
        char* searchKey = keyToString(i);
        toml_datum_t searchResult = toml_get(config, searchKey);

        switch(i)
        {
            case KEY_SIZE:
            {
                tempConfig.size = tomlToIntVect2(searchResult);
            }break;
            case KEY_RESIZABLE:
            { 
                tempConfig.resizable = searchResult.u.boolean;
            } break;
            default: {}
        }
    }

    fclose(configFile);
    toml_free(configResult);
    return tempConfig;
}


toml_datum_t getModuleContent()
{
    toml_datum_t temp = {0};

    modFile = fopen("modules/test1.toml", "r+");
    modFileResult = toml_parse_file(modFile);
    if (!modFileResult.ok)
    {
        printf("failed to parse module!\n");
        freeModFiles();
        return temp;
    }

    toml_datum_t content = toml_seek(modFileResult.toptab, "content");
    if (content.type != TOML_TABLE)
    {
        printf("content returned as NOT a table"); return temp;
    }

    return content;
}


/*    ELEMENT PARSING
*******************************************************************************/
s_Element getElementFromToml(toml_datum_t toml)
{
    s_Element temp = {0};

    char* searchKey = "0";
    toml_datum_t searchResult = {0};

    // Check for every key possible for an element
    for(int i = 0; i < MAX_KEY; i++)
    {
        searchKey = keyToString(i); 
        searchResult = toml_get(toml, searchKey);

        switch(i)
        {   
            case KEY_PADDING:
            {
                if(searchResult.type != TOML_ARRAY) {continue;}
                temp.padding.key = KEY_PADDING;
                temp.padding.val.shape = tomlToRect(searchResult);
            }break;
            case KEY_BOUNDS:
            {
                if(searchResult.type != TOML_ARRAY) {continue;}
                temp.bounds.key = KEY_BOUNDS;
                temp.bounds.val.shape = tomlToRect(searchResult);
            } break;
            case KEY_TEXT:
            {
                if(searchResult.type != TOML_STRING) {continue;}
                temp.text.key = KEY_TEXT;
                temp.text.val.stringVal = tomlToString(searchResult);
            } break;
            case KEY_TYPE:
            {
                if(searchResult.type != TOML_STRING) {continue;}
                temp.type.key = KEY_TYPE;
                temp.type.val.intVal = tomlToElementType(searchResult);
            } break;
            case KEY_ALIGN:
            {
                if(searchResult.type != TOML_STRING) {continue;}
                temp.align.key = KEY_ALIGN;
                temp.align.val.intVal = tomlToElementAlign(searchResult);
            } break;
            case KEY_FILL:
            {
                if(searchResult.type != TOML_STRING) {continue;}
                temp.fill.key = KEY_FILL;
                temp.fill.val.intVal = tomlToElementFill(searchResult);
            } break;
            case KEY_STATE:
            {
                if(searchResult.type != TOML_STRING) {continue;}
                temp.state.key = KEY_STATE;
                temp.state.val.intVal = tomlToElementState(searchResult);
            } break;
            case KEY_VISIBILITY:
            {
                if(searchResult.type != TOML_BOOLEAN) {continue;}
                temp.visibility.key = KEY_VISIBILITY;
                temp.visibility.val.boolVal = tomlToBool(searchResult);
            } break;
            case KEY_SIZE:
            {
                if(searchResult.type != TOML_STRING) {continue;}
                temp.size.key = KEY_SIZE;
                temp.size.val.intVal = tomlToElementSize(searchResult);
            } break;
            case KEY_RESIZABLE:
            {
                if(searchResult.type != TOML_BOOLEAN) {continue;}
                temp.resizable.key = KEY_RESIZABLE;
                temp.resizable.val.boolVal = tomlToBool(searchResult);
            } break;
            default: { }
        }
    }
    
    return temp;  
}



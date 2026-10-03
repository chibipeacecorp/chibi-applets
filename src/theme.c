#include "core.h"

extern s_UIArena arena;

char* themeKeyToString(e_ElementType obj)
{
    char* temp = "0";
    
    switch(obj)
    {
        case TYPE_BTTN: {temp = "button";} break;
        case TYPE_PANEL: {temp = "panel";} break;
        default: {/*do nothing*/}
    }
    
    return temp;
}

char* colorTypeToString(e_ThemeColorType type)
{
    char* temp = "0";
    
    switch(type)
    {
        case baseColor: {temp = "base";} break;
        case outlineColor: {temp = "outline";} break;
        case textColor: {temp = "text";} break;
        default: {/*do nothing*/}
    }
    
    return temp;
}

Color getColorFromInt64(int64_t toml_value)
{
    Color temp = {0};
    
    temp.r = (toml_value >> 24) & 0xff;
    temp.g = (toml_value >> 16) & 0xff;
    temp.b = (toml_value >> 8) & 0xff;
    temp.a = (toml_value >> 0) & 0xff;

    return temp;
}

// gets passed the "*.color" key from whatever table is being checked 
s_ThemeColors getColorsFromToml(toml_datum_t toml)
{
    s_ThemeColors temp = {0};
    // select-hovered
    // need to update struct with new color options

    if(toml.type != TOML_TABLE) {return temp;}

    toml_datum_t disabled = toml_get(toml, "disabled");
    temp.disabled = getColorFromInt64(disabled.u.int64);

    toml_datum_t idle = toml_get(toml, "idle");
    toml_datum_t idleNormal = toml_get(idle, "normal");
    toml_datum_t idleHovered = toml_get(idle, "hovered");
    toml_datum_t idleClicked = toml_get(idle, "clicked");
    temp.idle = getColorFromInt64(idleNormal.u.int64);
    temp.hovered = getColorFromInt64(idleHovered.u.int64);
    temp.clicked = getColorFromInt64(idleClicked.u.int64);


    // toml_datum_t selected = toml_get(toml, "selected");
    // toml_datum_t selectedNormal = toml_get(selected, "normal");
    // toml_datum_t selectedHovered = toml_get(selected, "hovered");
    // toml_datum_t selectedClicked = toml_get(selected, "clicked");

    return temp;
}

// receives theme obj parent
s_ThemeData getThemeTypeFromToml(toml_datum_t obj)
{
    s_ThemeData defaultTheme = {0};

    toml_datum_t subObjTree = toml_get(obj, "base");
    s_ThemeColors color_Base = getColorsFromToml(toml_get(subObjTree, "color"));
    
    
    subObjTree = toml_get(obj, "outline");
    toml_datum_t width = toml_get(subObjTree, "width");
    printf("width type: %d\n", width.type);
    printf("width val: %lld\n", width.u.int64);


    s_ThemeColors color_Outline = getColorsFromToml(toml_get(subObjTree, "color"));
    
    //s_ThemeColors color_Text = getColorsFromToml(toml_get(baseTree, "color"));
    defaultTheme.base = color_Base;
    defaultTheme.outline = color_Outline;
    defaultTheme.outlineWidth = (float)width.u.int64;
    defaultTheme.text = color_Outline;
    defaultTheme.fontID = 1;
    defaultTheme.fontSize = 12;
    return defaultTheme;
}

// CURRENTLY RETURNING THE LOCAL DATA POINTER. collapse into a single theme getter
void getThemeData()
{
    FILE* themeFile = fopen("themes/style.toml", "r+");
    toml_result_t result = toml_parse_file(themeFile);
    if (!result.ok)
    { 
        printf("failed to parse file!\n");
        fclose(themeFile);
        toml_free(result);
    }

    toml_datum_t tree = result.toptab;
    toml_datum_t subTree;

    for (int i = 0; i < MAX_TYPE; i++)
    {
        subTree = toml_get(tree, themeKeyToString(i));
        arena.theme[i] = getThemeTypeFromToml(subTree); // passes in "button"
    }

    // FREE resources
    toml_free(result);
    fclose(themeFile);
}
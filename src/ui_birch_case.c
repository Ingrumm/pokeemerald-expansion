#include "global.h"
#include "ui_birch_case.h"
#include "strings.h"
#include "bg.h"
#include "data.h"
#include "decompress.h"
#include "event_data.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "item.h"
#include "item_menu.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "item_icon.h"
#include "item_use.h"
#include "international_string_util.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "scanline_effect.h"
#include "script.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text_window.h"
#include "overworld.h"
#include "event_data.h"
#include "constants/items.h"
#include "constants/field_weather.h"
#include "constants/songs.h"
#include "constants/rgb.h"
#include "trainer_pokemon_sprites.h"
#include "field_effect.h"
#include "pokemon.h"
#include "pokedex.h"
#include "script_pokemon_util.h"
#include "pokeball.h"
#include "constants/moves.h"
#include "naming_screen.h"
#include "tv.h"
#include "script_menu.h"
#include "random.h"
#include "move.h"

 /*
    9 Starter Selection Birch Case
    This UI was coded by Archie with Graphics by Mudskip.
 */

//==========DEFINES==========//
struct MenuResources
{
    MainCallback savedCallback;     // determines callback to run when we exit. e.g. where do we want to go after closing the menu
    u8 gfxLoadState;
    u16 monSpriteId;
    u16 itemSpriteId;
    u16 pokeballSpriteIds[9];
    u16 handSpriteId;
    u16 handPosition;
    u16 selector_x;
    u16 selector_y;
    u16 movingSelector;
};



enum WindowIds
{
    WINDOW_BOTTOM_BAR,
};

enum TextIds
{
    CHOOSE_MON,
    CONFIRM_SELECTION,
    RECIEVED_MON,
};

enum TextIds2
{
    OVERVIEW_MON,
    INSPECT_MON,
    OVERVIEW_ITEM,
    CONFIRM_REROLL,
    CONFIRM_MON,
    CONFIRM_ITEM,
    NAME_MON,
    OVERVIEW_SECRET,
    CONFIRM_SECRET,
    REVEAL_SECRET_MON,
    REVEAL_SECRET_ITEM,
};

enum Colors
{
    FONT_BLACK,
    FONT_WHITE,
    FONT_RED,
    FONT_BLUE,
    FONT_GREEN,
};
static const u8 sMenuWindowFontColors[][3] = 
{
    [FONT_BLACK]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GRAY,  TEXT_COLOR_LIGHT_GRAY},
    [FONT_WHITE]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_WHITE,  TEXT_COLOR_DARK_GRAY},
    [FONT_RED]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,        TEXT_COLOR_LIGHT_RED},
    [FONT_BLUE]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,       TEXT_COLOR_LIGHT_GRAY},
    [FONT_GREEN]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_GREEN,       TEXT_COLOR_LIGHT_GREEN},
};


struct SpriteCordsStruct {
    u8 x;
    u8 y;
};

enum BallPositions
{
    BALL_TOP_FIRST,
    BALL_TOP_SECOND,
    BALL_TOP_THIRD,
    BALL_TOP_FOURTH,
    BALL_MIDDLE_FIRST,
    BALL_MIDDLE_SECOND,
    BALL_MIDDLE_THIRD,
    BALL_BOTTOM_FIRST,
    BALL_BOTTOM_SECOND,
};

// struct MonChoiceData{ // This is the format used to define a mon, everything left out will default to 0 and be blank or use the in game defaults
//     u16 species; // Mon Species ID
//     u8 level;   // Mon Level 5
//     u16 item;   // Held item, just ITEM_POTION
//     u8 ball; // this ballid does not change the design of the ball in the case, only in summary/throwing out to battle 
//     u8 nature; // NATURE_JOLLY, NATURE_ETC...
//     u8 abilityNum; // this is either 0/1 in vanilla or 0/1/2 in Expansion, its the ability num your mon uses from its possible abilities, not the ability constant itself
//     u8 gender; // MON_MALE, MON_FEMALE, MON_GENDERLESS
//     u8 evs[6]; // use format {255, 255, 0, 0, 0, 0}
//     u8 ivs[6]; // use format {31, 31, 31, 31, 31, 31}
//     u16 moves[4]; // use format {MOVE_FIRE_BLAST, MOVE_SHEER_COLD, MOVE_NONE, MOVE_NONE}
//     bool8 ggMaxFactor;      // only work in Expansion set to 0 otherwise or leave blank
//     u8 teraType;            // only work in Expansion set to 0 otherwise or leave blank
//     bool8 isShinyExpansion; // only work in Expansion set to 0 otherwise or leave blank
//     bool8 isSecret;
// };

// struct ItemChoiceData {
//     u16 item;
//     u8 count;
//     bool8 isSecret;
// };

// struct MonHiddenInfo{
//     bool8 hpRevealed;
//     bool8 atkRevealed;
//     bool8 defRevealed;
//     bool8 spatkRevealed;
//     bool8 spdefRevealed;
//     bool8 speedRevealed;
//     bool8 abilityRevealed;
//     bool8 natureRevealed;
// };

struct MonHiddenInfo sHiddenInfo[9] =
{
    [BALL_TOP_FIRST]        = {0,0,0,0,0,0,0,0},
    [BALL_TOP_SECOND]       = {0,0,0,0,0,0,0,0},
    [BALL_MIDDLE_FIRST]     = {0,0,0,0,0,0,0,0},

    [BALL_TOP_THIRD]        = {0,0,0,0,0,0,0,0},
    [BALL_TOP_FOURTH]       = {0,0,0,0,0,0,0,0},
    [BALL_MIDDLE_THIRD]     = {0,0,0,0,0,0,0,0},

    [BALL_MIDDLE_SECOND]    = {0,0,0,0,0,0,0,0},
    [BALL_BOTTOM_FIRST]     = {0,0,0,0,0,0,0,0},
    [BALL_BOTTOM_SECOND]    = {0,0,0,0,0,0,0,0},
};

float get_stat(struct MonHiddenInfo info, int i) {
    switch(i){
        case 0: return info.hpRevealed;
        case 1: return info.atkRevealed;
        case 2: return info.defRevealed;
        case 3: return info.spatkRevealed;
        case 4: return info.spdefRevealed;
        case 5: return info.speedRevealed;
        case 6: return info.abilityRevealed;
        case 7: return info.natureRevealed;
    }
    return 0;
}

//==========EWRAM==========//
static EWRAM_DATA struct MenuResources *sBirchCaseDataPtr = NULL;
static EWRAM_DATA u8 *sBg1TilemapBuffer = NULL;
static EWRAM_DATA u8 *sBg2TilemapBuffer = NULL;

//==========STATIC=DEFINES==========//
static void BirchCaseRunSetup(void);
static bool8 BirchCaseDoGfxSetup(void);
static bool8 BirchCase_InitBgs(void);
static void BirchCaseFadeAndBail(void);
static bool8 BirchCaseLoadGraphics(void);
static void BirchCase_InitWindows(void);
static void PrintTextToBottomBar(u8 textId);
static void Task_BirchCaseWaitFadeIn(u8 taskId);
static void Task_BirchCaseMain(u8 taskId);
static void Task_BirchCaseSelect(u8 taskId);
static void SampleUi_DrawMonIcon(u16 speciesId);
static void SampleUi_DrawItemIcon(u16 itemId);
static void Task_DelayedSpriteLoad(u8 taskId);

//==========CONST=DATA==========//
static const struct BgTemplate sMenuBgTemplates[] =
{
    {
        .bg = 0,    // windows, etc
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .priority = 0
    }, 
    {
        .bg = 1,    // this bg loads the UI tilemap
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .priority = 2
    },
    {
        .bg = 2,    // this bg loads the UI tilemap
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .priority = 1
    }
};

static const struct WindowTemplate sMenuWindowTemplates[] = 
{
    [WINDOW_BOTTOM_BAR] = 
    {
        .bg = 0,            // which bg to print text on
        .tilemapLeft = 0,   // position from left (per 8 pixels)
        .tilemapTop = 0,    // position from top (per 8 pixels)
        .width = 30,        // width (per 8 pixels)
        .height = 20,        // height (per 8 pixels)
        .paletteNum = 15,   // palette index to use for text
        .baseBlock = 1,     // tile start in VRAM
    },
    DUMMY_WIN_TEMPLATE
};


//
//  Graphics Pointers to Tilemaps, Tilesets, Spritesheets, Palettes
//
static const u32 sCaseTiles[]   = INCBIN_U32("graphics/ui_birch_case/case_tiles.4bpp.lz");
static const u32 sCaseTilemap[] = INCBIN_U32("graphics/ui_birch_case/case_tiles.bin.lz");
static const u16 sCasePalette[] = INCBIN_U16("graphics/ui_birch_case/case_tiles.gbapal");

static const u32 sTextBgTiles[]   = INCBIN_U32("graphics/ui_birch_case/text_bg_tiles.4bpp.lz");
static const u32 sTextBgTilemap[] = INCBIN_U32("graphics/ui_birch_case/text_bg_tiles.bin.lz");
static const u16 sTextBgPalette[] = INCBIN_U16("graphics/ui_birch_case/text_bg_tiles.gbapal");

static const u32 sPokeballHand_Gfx[] = INCBIN_U32("graphics/ui_birch_case/pokeball_hand.4bpp.lz");
static const u16 sPokeballHand_Pal[] = INCBIN_U16("graphics/ui_birch_case/pokeball_hand.gbapal");

//
//  Sprite Data for Pokeball Hand Sprite
//
#define TAG_POKEBALL_CURSOR 20001
static const struct OamData sOamData_PokeballHand =
{
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 1,
};

static const struct CompressedSpriteSheet sSpriteSheet_PokeballHand =
{
    .data = sPokeballHand_Gfx,
    .size = 32*32*4/2,
    .tag = TAG_POKEBALL_CURSOR,
};

static const struct SpritePalette sSpritePal_PokeballHand =
{
    .data = sPokeballHand_Pal,
    .tag = TAG_POKEBALL_CURSOR
};

static const union AnimCmd sSpriteAnim_PokeballStatic[] =
{
    ANIMCMD_FRAME(0, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSpriteAnim_PokeballRockBackAndForth[] =
{
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(16, 16),
    ANIMCMD_FRAME(0, 16),
    ANIMCMD_FRAME(32, 16),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd sSpriteAnim_Hand[] =
{
    ANIMCMD_FRAME(48, 32),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sSpriteAnimTable_PokeballHand[] =
{
    sSpriteAnim_PokeballStatic,
    sSpriteAnim_PokeballRockBackAndForth,
    sSpriteAnim_Hand,
};

static const struct SpriteTemplate sSpriteTemplate_PokeballHandMap =
{
    .tileTag = TAG_POKEBALL_CURSOR,
    .paletteTag = TAG_POKEBALL_CURSOR,
    .oam = &sOamData_PokeballHand,
    .anims = sSpriteAnimTable_PokeballHand,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};


//
//  This is the Callback for the Hand Cursor that Updates its sprite position when moved by the input control functions
//
#define TOP_ROW_Y 36
#define MIDDLE_ROW_Y 58
#define BOTTOM_ROW_Y 80

static const struct SpriteCordsStruct sBallSpriteCords[3][4] = {
        {{40, TOP_ROW_Y}, {88, TOP_ROW_Y}, {152, TOP_ROW_Y}, {200, TOP_ROW_Y}},
        {{64, MIDDLE_ROW_Y}, {120, MIDDLE_ROW_Y}, {176, MIDDLE_ROW_Y}},
        {{96, BOTTOM_ROW_Y}, {144, BOTTOM_ROW_Y}},
};

static void CursorCallback(struct Sprite *sprite)
{
    struct SpriteCordsStruct current_position = {0,0};
    if(sBirchCaseDataPtr->handPosition <= 3)
        current_position = sBallSpriteCords[0][sBirchCaseDataPtr->handPosition];
    else if(sBirchCaseDataPtr->handPosition <= 6)  
        current_position = sBallSpriteCords[1][sBirchCaseDataPtr->handPosition - 4];
    else
        current_position = sBallSpriteCords[2][sBirchCaseDataPtr->handPosition - 7];

    sprite->x = current_position.x;
    sprite->y = current_position.y - 6;

    if(sBirchCaseDataPtr->movingSelector != TRUE)
    {
        if(sprite->data[5] <= 30)
        {
            sprite->y -= 1;
        }
        else if(sprite->data[5] > 30 && sprite->data[5] < 60)
        {
            sprite->y += 1;
        }
        else
        {
            sprite->y -= 1;
            sprite->data[5] = 0;
        }
        sprite->data[5]++;
    }

}

struct MonChoiceData sStarterChoices[9];
struct ItemChoiceData sItemChoices[9];
//
//  Create The Hande Cursor Sprite
//
static void CreateHandSprite()
{
    u16 i = 0;
    u16 x, y;
    struct SpriteCordsStruct current_position = {0,0};

    for(i=0; i<9; i++)
    {
        if(sStarterChoices[i].species == SPECIES_NONE) // Choose Non Empty Slot To Start In
            if(sItemChoices[i].item == ITEM_NONE)
                continue;

        if(sBirchCaseDataPtr->handPosition <= 3)
        {
            current_position = sBallSpriteCords[0][sBirchCaseDataPtr->handPosition];
            break;
        }
        else if(sBirchCaseDataPtr->handPosition <= 6)  
        {
            current_position = sBallSpriteCords[1][sBirchCaseDataPtr->handPosition - 4];
            break;
        }
        else
        {
            current_position = sBallSpriteCords[2][sBirchCaseDataPtr->handPosition - 7];
            break;
        }
    }

    x = current_position.x;
    y = current_position.y - 6;
    sBirchCaseDataPtr->handPosition = i;
    if (sBirchCaseDataPtr->handSpriteId == SPRITE_NONE)
        sBirchCaseDataPtr->handSpriteId = CreateSpriteAtEnd(&sSpriteTemplate_PokeballHandMap, x, y, 0);
    gSprites[sBirchCaseDataPtr->handSpriteId].invisible = FALSE;
    gSprites[sBirchCaseDataPtr->handSpriteId].callback = CursorCallback;
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->handSpriteId], 2);
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[sBirchCaseDataPtr->handPosition]], 1);
    if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE)
    {
        if(!sStarterChoices[sBirchCaseDataPtr->handPosition].isSecret)
        {
            SampleUi_DrawMonIcon(sStarterChoices[sBirchCaseDataPtr->handPosition].species);

        }
    }
    if(sItemChoices[sBirchCaseDataPtr->handPosition].item != ITEM_NONE)
    {
        if(!sItemChoices[sBirchCaseDataPtr->handPosition].isSecret)
        {
            SampleUi_DrawItemIcon(sItemChoices[sBirchCaseDataPtr->handPosition].item);
        }
    }

    return;
}

static void DestroyHandSprite()
{
    u8 i = 0;
    for(i = 0; i < 9; i++)
    {
        DestroySprite(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]]);
        sBirchCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
}

//
//  Create The Pokeball Sprites For Each Slot if Not SPECIES_NONE
//
static void CreatePokeballSprites()
{
    u16 i = 0;

    for(i=0; i<9; i++)
    {
        u16 x, y;
        if(sStarterChoices[i].species == SPECIES_NONE)
            if(sItemChoices[i].item == ITEM_NONE)
                continue;

        if(i <= 3)
        {
            x = sBallSpriteCords[0][i].x;
            y = sBallSpriteCords[0][i].y;
        }
        else if(i <= 6)
        {

            x = sBallSpriteCords[1][i - 4].x;
            y = sBallSpriteCords[1][i - 4].y;
        }
        else
        {
            x = sBallSpriteCords[2][i - 7].x;
            y = sBallSpriteCords[2][i - 7].y;
        }
        if (sBirchCaseDataPtr->pokeballSpriteIds[i] == SPRITE_NONE)
            sBirchCaseDataPtr->pokeballSpriteIds[i] = CreateSpriteAtEnd(&sSpriteTemplate_PokeballHandMap, x, y, 1);
        gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]].invisible = FALSE;
        StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]], 0);

    }   
    return;
}

static void DestroyPokeballSprites()
{
    u8 i = 0;
    for(i = 0; i < 9; i++)
    {
        DestroySprite(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[i]]);
        sBirchCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }
}


//
//  Draw The Pokemon Sprites
//
#define MON_ICON_X     208
#define MON_ICON_Y     104
#define TAG_MON_SPRITE 30003
static void SampleUi_DrawMonIcon(u16 speciesId)
{
    sBirchCaseDataPtr->monSpriteId = CreateMonPicSprite_Affine(speciesId, 0, 0x8000, TRUE, MON_ICON_X, MON_ICON_Y, 5, TAG_NONE);
    gSprites[sBirchCaseDataPtr->monSpriteId].oam.priority = 0;
}

static void SampleUi_DrawItemIcon(u16 itemId) 
{
    FreeSpriteTilesByTag(5109);
    FreeSpritePaletteByTag(5109);
    sBirchCaseDataPtr->itemSpriteId = AddItemIconSprite(5109, 5109, itemId);
    gSprites[sBirchCaseDataPtr->itemSpriteId].oam.priority = 0;
    gSprites[sBirchCaseDataPtr->itemSpriteId].x2 = MON_ICON_X+8;
    gSprites[sBirchCaseDataPtr->itemSpriteId].y2 = MON_ICON_Y+8;
}


static void ReloadNewPokemon(u8 taskId) // reload the pokeball after a 4 frame delay to prevent palette problems
{
    gSprites[sBirchCaseDataPtr->monSpriteId].invisible = TRUE;
    gSprites[sBirchCaseDataPtr->itemSpriteId].invisible = TRUE;
    FreeResourcesAndDestroySprite(&gSprites[sBirchCaseDataPtr->monSpriteId], sBirchCaseDataPtr->monSpriteId);
    FreeResourcesAndDestroySprite(&gSprites[sBirchCaseDataPtr->itemSpriteId], sBirchCaseDataPtr->itemSpriteId);
    sBirchCaseDataPtr->movingSelector = TRUE;
    gTasks[taskId].func = Task_DelayedSpriteLoad;
    gTasks[taskId].data[11] = 0;
}

static void ChangePositionUpdateSpriteAnims(u16 oldPosition, u8 taskId) // turn off Ball Shaking on old ball and start it on new ball, reload pokemon and text
{
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[oldPosition]], 0);
    StartSpriteAnim(&gSprites[sBirchCaseDataPtr->pokeballSpriteIds[sBirchCaseDataPtr->handPosition]], 1);
    ReloadNewPokemon(taskId);
    if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE)
    {
        if(sStarterChoices[sBirchCaseDataPtr->handPosition].isSecret) 
        {
            PrintTextToBottomBar(OVERVIEW_SECRET);
        }
        else
        {
            PrintTextToBottomBar(OVERVIEW_MON);

        }
    }
    if(sItemChoices[sBirchCaseDataPtr->handPosition].item != ITEM_NONE)
    {
        if(sItemChoices[sBirchCaseDataPtr->handPosition].isSecret) 
        {
            PrintTextToBottomBar(OVERVIEW_SECRET);
        }
        else
        {
            PrintTextToBottomBar(OVERVIEW_ITEM);
        }
    }
}

static void BirchCase_GiveMon() // Function that calls the GiveMon function pulled from Expansion by Lunos and Ghoulslash
{
    u8 *evs = (u8 *) sStarterChoices[sBirchCaseDataPtr->handPosition].evs;
    u8 *ivs = (u8 *) sStarterChoices[sBirchCaseDataPtr->handPosition].ivs;
    u16 *moves = (u16 *) sStarterChoices[sBirchCaseDataPtr->handPosition].moves;
    FlagSet(FLAG_SYS_POKEMON_GET);
    gSpecialVar_Result = BirchCase_GiveMonParameterized(sStarterChoices[sBirchCaseDataPtr->handPosition].species, sStarterChoices[sBirchCaseDataPtr->handPosition].level, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].item, sStarterChoices[sBirchCaseDataPtr->handPosition].ball, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].nature, sStarterChoices[sBirchCaseDataPtr->handPosition].abilityNum, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].gender, evs, ivs, moves, \
                sStarterChoices[sBirchCaseDataPtr->handPosition].ggMaxFactor, sStarterChoices[sBirchCaseDataPtr->handPosition].teraType,\
                sStarterChoices[sBirchCaseDataPtr->handPosition].isShinyExpansion);
}


bool8 isDupe(int speciesId)
{
    int familySize = sizeof(gSpeciesInfo[speciesId].familyMembers)/sizeof(gSpeciesInfo[speciesId].familyMembers[0]);
    for(int i=0; i<familySize; i++)
    {
        if(gSpeciesInfo[speciesId].familyMembers[i] == SPECIES_NONE) {
            continue;
        }
        else
        {
            if(GetSetPokedexFlag(SpeciesToNationalPokedexNum(gSpeciesInfo[speciesId].familyMembers[i]), FLAG_GET_CAUGHT))
            {
                DebugPrintf("dupeId: %d, dupeName: %S, familyDupeName: %S, familySize: %d", speciesId, gSpeciesInfo[speciesId].speciesName, gSpeciesInfo[gSpeciesInfo[speciesId].familyMembers[i]].speciesName, familySize);
                return TRUE;
            }
        }

    }
    return FALSE;
}


static void GenerateOptions(int speciesList[], int size)
{
    int potentialSpecies = 0;
    int speciesId = 0;
    int blowup;

    for(int i=4; i<=6; i++)
    {
        blowup = 0;
        do
        {
            if(blowup>=10)
            {
                break;
            }
            potentialSpecies = speciesList[Random() % size];
            if(potentialSpecies <= SPECIES_NONE || potentialSpecies >= NUM_SPECIES) {
                DebugPrintf("THIS SHOULD NOT HAPPEN");
                continue;
            }
            if(potentialSpecies == sStarterChoices[4].species || potentialSpecies == sStarterChoices[5].species)
            {
                blowup++;
                DebugPrintf("Failed to put %S in the %d slot. CHOICES: %S %S %S. Rerolling", gSpeciesInfo[potentialSpecies].speciesName, i-3, gSpeciesInfo[sStarterChoices[4].species].speciesName, gSpeciesInfo[sStarterChoices[5].species].speciesName, gSpeciesInfo[sStarterChoices[6].species].speciesName);
                speciesId = 0;
                continue;
            }
            // if(potentialSpecies == sStarterChoices[6].species)
            // {
            //     blowup++;
            //     DebugPrintf("6th slot duplicate %S, CHOICES: %S %S %S. Rerolling 6th", gSpeciesInfo[potentialSpecies].speciesName, gSpeciesInfo[sStarterChoices[4].species].speciesName, gSpeciesInfo[sStarterChoices[5].species].speciesName, gSpeciesInfo[sStarterChoices[6].species].speciesName);
            //     speciesId = 0;
            //     continue;
            // }
            if(isDupe(potentialSpecies)) {
                blowup++;
                DebugPrintf("Pokemon %S in the %d slot is a dupe. CHOICES: %S %S %S. Rerolling", gSpeciesInfo[potentialSpecies].speciesName, i-3, gSpeciesInfo[sStarterChoices[4].species].speciesName, gSpeciesInfo[sStarterChoices[5].species].speciesName, gSpeciesInfo[sStarterChoices[6].species].speciesName);
                speciesId = 0;
                continue;
            }
            speciesId = potentialSpecies;

        } while(speciesId == 0);

        sStarterChoices[i].species = speciesId;
        sStarterChoices[i].level = 5;
        sStarterChoices[i].abilityNum = Random() % 2;
        sStarterChoices[i].nature = Random() % 25;
        sStarterChoices[i].ivs[0] = Random() % 31;
        sStarterChoices[i].ivs[1] = Random() % 31;
        sStarterChoices[i].ivs[2] = Random() % 31;
        sStarterChoices[i].ivs[3] = Random() % 31;
        sStarterChoices[i].ivs[4] = Random() % 31;
        sStarterChoices[i].ivs[5] = Random() % 31;
        sStarterChoices[i].ball = BALL_POKE;
        sStarterChoices[i].isSecret = FALSE;

        sHiddenInfo[i].hpRevealed = 0;
        sHiddenInfo[i].atkRevealed = 0;
        sHiddenInfo[i].defRevealed = 0;
        sHiddenInfo[i].spatkRevealed = 0;
        sHiddenInfo[i].spdefRevealed = 0;
        sHiddenInfo[i].speedRevealed = 0;
        sHiddenInfo[i].abilityRevealed = 0;
        sHiddenInfo[i].natureRevealed = 0;
        DebugPrintf("1. %S, 2. %S, 3. %S", gSpeciesInfo[sStarterChoices[4].species].speciesName, gSpeciesInfo[sStarterChoices[5].species].speciesName, gSpeciesInfo[sStarterChoices[6].species].speciesName);
    }
    gSaveBlock3Ptr->revealsRemaining = 3;
}

static void GenerateBox()
{
    if(FlagGet(FLAG_BRIEFCASE_OPENED))
    {
        for(int i=0; i<9; i++)
        {
            sStarterChoices[i] = gSaveBlock3Ptr->briefcaseMons[i];
            sItemChoices[i] = gSaveBlock3Ptr->briefcaseItems[i];
            sHiddenInfo[i] = gSaveBlock3Ptr->briefcaseHidden[i];
        }
    }
    else
    {
        if(FlagGet(FLAG_SPLIT_4_DONE))
        {
            int speciesListSplit5[] = {SPECIES_IVYSAUR,SPECIES_CHARMELEON,SPECIES_WARTORTLE,SPECIES_BUTTERFREE,SPECIES_BEEDRILL,SPECIES_PIDGEOT,SPECIES_RATICATE,SPECIES_RATICATE_ALOLA,SPECIES_FEAROW,SPECIES_ARBOK,SPECIES_PIKACHU,SPECIES_SANDSLASH,SPECIES_SANDSLASH_ALOLA,SPECIES_NIDORINA,SPECIES_NIDORINO,SPECIES_CLEFAIRY,SPECIES_VULPIX,SPECIES_VULPIX_ALOLA,SPECIES_WIGGLYTUFF,SPECIES_GOLBAT,SPECIES_GLOOM,SPECIES_PARASECT,SPECIES_VENOMOTH,SPECIES_DUGTRIO,SPECIES_DUGTRIO_ALOLA,SPECIES_PERSIAN,SPECIES_PERSIAN_ALOLA,SPECIES_PERRSERKER,SPECIES_PSYDUCK,SPECIES_PRIMEAPE,SPECIES_GROWLITHE,SPECIES_GROWLITHE_HISUI,SPECIES_POLIWHIRL,SPECIES_KADABRA,SPECIES_MACHOKE,SPECIES_WEEPINBELL,SPECIES_TENTACOOL,SPECIES_GRAVELER,SPECIES_GRAVELER_ALOLA,SPECIES_PONYTA,SPECIES_PONYTA_GALAR,SPECIES_SLOWPOKE,SPECIES_SLOWPOKE_GALAR,SPECIES_MAGNETON,SPECIES_FARFETCHD,SPECIES_FARFETCHD_GALAR,SPECIES_DODRIO,SPECIES_DEWGONG,SPECIES_GRIMER,SPECIES_GRIMER_ALOLA,SPECIES_SHELLDER,SPECIES_HAUNTER,SPECIES_ONIX,SPECIES_DROWZEE,SPECIES_KINGLER,SPECIES_VOLTORB,SPECIES_VOLTORB_HISUI,SPECIES_EXEGGCUTE,SPECIES_MAROWAK,SPECIES_MAROWAK_ALOLA,SPECIES_HITMONLEE,SPECIES_HITMONCHAN,SPECIES_HITMONTOP,SPECIES_LICKITUNG,SPECIES_KOFFING,SPECIES_RHYHORN,SPECIES_HAPPINY,SPECIES_TANGELA,SPECIES_SEADRA,SPECIES_SEAKING,SPECIES_STARYU,SPECIES_MR_MIME,SPECIES_MR_MIME_GALAR,SPECIES_JYNX,SPECIES_ELEKID,SPECIES_MAGBY,SPECIES_MAGIKARP,SPECIES_DITTO,SPECIES_EEVEE,SPECIES_PORYGON,SPECIES_OMANYTE,SPECIES_KABUTO,SPECIES_MUNCHLAX,SPECIES_DRAGONAIR,SPECIES_BAYLEEF,SPECIES_QUILAVA,SPECIES_CROCONAW,SPECIES_FURRET,SPECIES_NOCTOWL,SPECIES_LEDIAN,SPECIES_ARIADOS,SPECIES_LANTURN,SPECIES_TOGETIC,SPECIES_XATU,SPECIES_FLAAFFY,SPECIES_AZUMARILL,SPECIES_SUDOWOODO,SPECIES_JUMPLUFF,SPECIES_AIPOM,SPECIES_SUNFLORA,SPECIES_YANMA,SPECIES_WOOPER,SPECIES_WOOPER_PALDEA,SPECIES_MURKROW,SPECIES_MISDREAVUS,SPECIES_WYNAUT,SPECIES_GIRAFARIG,SPECIES_FORRETRESS,SPECIES_DUNSPARCE,SPECIES_GLIGAR,SPECIES_GRANBULL,SPECIES_QWILFISH,SPECIES_QWILFISH_HISUI,SPECIES_SNEASEL,SPECIES_SNEASEL_HISUI,SPECIES_TEDDIURSA,SPECIES_MAGCARGO,SPECIES_PILOSWINE,SPECIES_CORSOLA,SPECIES_CORSOLA_GALAR,SPECIES_REMORAID,SPECIES_DELIBIRD,SPECIES_MANTYKE,SPECIES_SKARMORY,SPECIES_HOUNDOUR,SPECIES_PHANPY,SPECIES_STANTLER,SPECIES_PUPITAR,SPECIES_GROVYLE,SPECIES_COMBUSKEN,SPECIES_MARSHTOMP,SPECIES_MIGHTYENA,SPECIES_LINOONE,SPECIES_LINOONE_GALAR,SPECIES_BEAUTIFLY,SPECIES_DUSTOX,SPECIES_LOMBRE,SPECIES_NUZLEAF,SPECIES_SWELLOW,SPECIES_PELIPPER,SPECIES_KIRLIA,SPECIES_MASQUERAIN,SPECIES_BRELOOM,SPECIES_VIGOROTH,SPECIES_NINJASK,SPECIES_LOUDRED,SPECIES_HARIYAMA,SPECIES_NOSEPASS,SPECIES_DELCATTY,SPECIES_SABLEYE,SPECIES_MAWILE,SPECIES_LAIRON,SPECIES_MEDICHAM,SPECIES_MANECTRIC,SPECIES_PLUSLE,SPECIES_MINUN,SPECIES_VOLBEAT,SPECIES_ILLUMISE,SPECIES_ROSELIA,SPECIES_SWALOT,SPECIES_SHARPEDO,SPECIES_WAILMER,SPECIES_CAMERUPT,SPECIES_TORKOAL,SPECIES_GRUMPIG,SPECIES_SPINDA,SPECIES_VIBRAVA,SPECIES_CACTURNE,SPECIES_SWABLU,SPECIES_ZANGOOSE,SPECIES_SEVIPER,SPECIES_LUNATONE,SPECIES_SOLROCK,SPECIES_WHISCASH,SPECIES_CRAWDAUNT,SPECIES_BALTOY,SPECIES_LILEEP,SPECIES_ANORITH,SPECIES_FEEBAS,SPECIES_CASTFORM,SPECIES_KECLEON,SPECIES_BANETTE,SPECIES_DUSCLOPS,SPECIES_TROPIUS,SPECIES_CHIMECHO,SPECIES_ABSOL,SPECIES_SNORUNT,SPECIES_SEALEO,SPECIES_CLAMPERL,SPECIES_LUVDISC,SPECIES_SHELGON,SPECIES_METANG,SPECIES_GROTLE,SPECIES_MONFERNO,SPECIES_PRINPLUP,SPECIES_STARAVIA,SPECIES_BIBAREL,SPECIES_KRICKETUNE,SPECIES_LUXIO,SPECIES_RAMPARDOS,SPECIES_BASTIODON,SPECIES_VESPIQUEN,SPECIES_PACHIRISU,SPECIES_BUIZEL,SPECIES_CHERRIM,SPECIES_GASTRODON,SPECIES_DRIFLOON,SPECIES_BUNEARY,SPECIES_PURUGLY,SPECIES_SKUNTANK,SPECIES_BRONZOR,SPECIES_CHATOT,SPECIES_GABITE,SPECIES_RIOLU,SPECIES_HIPPOPOTAS,SPECIES_SKORUPI,SPECIES_CROAGUNK,SPECIES_CARNIVINE,SPECIES_LUMINEON,SPECIES_SNOVER,SPECIES_ROTOM,SPECIES_SERVINE,SPECIES_PIGNITE,SPECIES_DEWOTT,SPECIES_WATCHOG,SPECIES_HERDIER,SPECIES_LIEPARD,SPECIES_PANSAGE,SPECIES_PANSEAR,SPECIES_PANPOUR,SPECIES_MUNNA,SPECIES_UNFEZANT,SPECIES_BLITZLE,SPECIES_BOLDORE,SPECIES_SWOOBAT,SPECIES_DRILBUR,SPECIES_AUDINO,SPECIES_GURDURR,SPECIES_PALPITOAD,SPECIES_THROH,SPECIES_SAWK,SPECIES_SWADLOON,SPECIES_WHIRLIPEDE,SPECIES_COTTONEE,SPECIES_PETILIL,SPECIES_BASCULIN,SPECIES_BASCULIN_WHITE_STRIPED,SPECIES_KROKOROK,SPECIES_DARUMAKA,SPECIES_DARUMAKA_GALAR,SPECIES_MARACTUS,SPECIES_DWEBBLE,SPECIES_SCRAGGY,SPECIES_YAMASK,SPECIES_YAMASK_GALAR,SPECIES_TIRTOUGA,SPECIES_ARCHEN,SPECIES_GARBODOR,SPECIES_ZORUA,SPECIES_ZORUA_HISUI,SPECIES_CINCCINO,SPECIES_GOTHORITA,SPECIES_DUOSION,SPECIES_SWANNA,SPECIES_VANILLISH,SPECIES_SAWSBUCK,SPECIES_EMOLGA,SPECIES_KARRABLAST,SPECIES_FOONGUS,SPECIES_FRILLISH,SPECIES_GALVANTULA,SPECIES_FERROSEED,SPECIES_KLANG,SPECIES_EELEKTRIK,SPECIES_LAMPENT,SPECIES_FRAXURE,SPECIES_CUBCHOO,SPECIES_SHELMET,SPECIES_STUNFISK,SPECIES_STUNFISK_GALAR,SPECIES_MIENFOO,SPECIES_GOLETT,SPECIES_PAWNIARD,SPECIES_RUFFLET,SPECIES_VULLABY,SPECIES_ZWEILOUS,SPECIES_LARVESTA,SPECIES_QUILLADIN,SPECIES_BRAIXEN,SPECIES_FROGADIER,SPECIES_DIGGERSBY,SPECIES_FLETCHINDER,SPECIES_VIVILLON,SPECIES_LITLEO,SPECIES_FLOETTE,SPECIES_SKIDDO,SPECIES_PANCHAM,SPECIES_FURFROU,SPECIES_MEOWSTIC_M,SPECIES_MEOWSTIC_F,SPECIES_HONEDGE,SPECIES_AROMATISSE,SPECIES_SWIRLIX,SPECIES_INKAY,SPECIES_BINACLE,SPECIES_SKRELP,SPECIES_CLAUNCHER,SPECIES_HELIOPTILE,SPECIES_TYRUNT,SPECIES_AMAURA,SPECIES_DEDENNE,SPECIES_SLIGGOO,SPECIES_SLIGGOO_HISUI,SPECIES_KLEFKI,SPECIES_TREVENANT,SPECIES_PUMPKABOO,SPECIES_BERGMITE,SPECIES_NOIBAT,SPECIES_DARTRIX,SPECIES_TORRACAT,SPECIES_BRIONNE,SPECIES_TRUMBEAK,SPECIES_GUMSHOOS,SPECIES_CHARJABUG,SPECIES_CRABOMINABLE,SPECIES_ORICORIO,SPECIES_RIBOMBEE,SPECIES_ROCKRUFF,SPECIES_WISHIWASHI,SPECIES_MAREANIE,SPECIES_MUDBRAY,SPECIES_ARAQUANID,SPECIES_FOMANTIS,SPECIES_SHIINOTIC,SPECIES_SALANDIT,SPECIES_STUFFUL,SPECIES_STEENEE,SPECIES_WIMPOD,SPECIES_SANDYGAST,SPECIES_PYUKUMUKU,SPECIES_MINIOR,SPECIES_KOMALA,SPECIES_TURTONATOR,SPECIES_TOGEDEMARU,SPECIES_MIMIKYU,SPECIES_BRUXISH,SPECIES_DRAMPA,SPECIES_HAKAMO_O,SPECIES_THWACKEY,SPECIES_RABOOT,SPECIES_DRIZZILE,SPECIES_GREEDENT,SPECIES_CORVISQUIRE,SPECIES_DOTTLER,SPECIES_THIEVUL,SPECIES_ELDEGOSS,SPECIES_WOOLOO,SPECIES_DREDNAW,SPECIES_BOLTUND,SPECIES_CARKOL,SPECIES_APPLIN,SPECIES_SILICOBRA,SPECIES_CRAMORANT,SPECIES_ARROKUDA,SPECIES_TOXEL,SPECIES_SIZZLIPEDE,SPECIES_GRAPPLOCT,SPECIES_SINISTEA,SPECIES_HATTREM,SPECIES_MORGREM,SPECIES_MILCERY,SPECIES_FALINKS,SPECIES_PINCURCHIN,SPECIES_FROSMOTH,SPECIES_STONJOURNER,SPECIES_EISCUE,SPECIES_INDEEDEE_M,SPECIES_INDEEDEE_F,SPECIES_MORPEKO,SPECIES_CUFANT,SPECIES_DRAKLOAK,SPECIES_FLORAGATO,SPECIES_CROCALOR,SPECIES_QUAXWELL,SPECIES_OINKOLOGNE_M,SPECIES_OINKOLOGNE_F,SPECIES_SPIDOPS,SPECIES_LOKIX,SPECIES_PAWMO,SPECIES_MAUSHOLD_FOUR,SPECIES_DACHSBUN,SPECIES_DOLLIV,SPECIES_SQUAWKABILLY,SPECIES_NACLSTACK,SPECIES_CHARCADET,SPECIES_TADBULB,SPECIES_WATTREL,SPECIES_MASCHIFF,SPECIES_SHROODLE,SPECIES_BRAMBLIN,SPECIES_TOEDSCOOL,SPECIES_KLAWF,SPECIES_CAPSAKID,SPECIES_RABSCA,SPECIES_FLITTLE,SPECIES_TINKATUFF,SPECIES_WUGTRIO,SPECIES_BOMBIRDIER,SPECIES_FINIZEN,SPECIES_VAROOM,SPECIES_GLIMMET,SPECIES_GREAVARD,SPECIES_CETODDLE,SPECIES_VELUZA,SPECIES_TATSUGIRI,SPECIES_ARCTIBAX,SPECIES_GIMMIGHOUL,SPECIES_POLTCHAGEIST};
            GenerateOptions(speciesListSplit5, 418);
        }
        else if(FlagGet(FLAG_SPLIT_3_DONE))
        {
            int speciesListSplit4[] = {SPECIES_IVYSAUR,SPECIES_CHARMELEON,SPECIES_WARTORTLE,SPECIES_BUTTERFREE,SPECIES_BEEDRILL,SPECIES_PIDGEOTTO,SPECIES_RATICATE,SPECIES_RATICATE_ALOLA,SPECIES_FEAROW,SPECIES_ARBOK,SPECIES_PIKACHU,SPECIES_SANDSLASH,SPECIES_SANDSLASH_ALOLA,SPECIES_NIDORINA,SPECIES_NIDORINO,SPECIES_CLEFAIRY,SPECIES_VULPIX,SPECIES_VULPIX_ALOLA,SPECIES_WIGGLYTUFF,SPECIES_GOLBAT,SPECIES_GLOOM,SPECIES_PARASECT,SPECIES_VENOMOTH,SPECIES_DUGTRIO,SPECIES_DUGTRIO_ALOLA,SPECIES_PERSIAN,SPECIES_PERSIAN_ALOLA,SPECIES_PERRSERKER,SPECIES_PSYDUCK,SPECIES_PRIMEAPE,SPECIES_GROWLITHE,SPECIES_GROWLITHE_HISUI,SPECIES_POLIWHIRL,SPECIES_KADABRA,SPECIES_MACHOKE,SPECIES_WEEPINBELL,SPECIES_TENTACOOL,SPECIES_GRAVELER,SPECIES_GRAVELER_ALOLA,SPECIES_PONYTA,SPECIES_PONYTA_GALAR,SPECIES_SLOWPOKE,SPECIES_SLOWPOKE_GALAR,SPECIES_MAGNEMITE,SPECIES_FARFETCHD,SPECIES_FARFETCHD_GALAR,SPECIES_DODUO,SPECIES_SEEL,SPECIES_GRIMER,SPECIES_GRIMER_ALOLA,SPECIES_SHELLDER,SPECIES_HAUNTER,SPECIES_ONIX,SPECIES_DROWZEE,SPECIES_KRABBY,SPECIES_VOLTORB,SPECIES_VOLTORB_HISUI,SPECIES_EXEGGCUTE,SPECIES_MAROWAK,SPECIES_MAROWAK_ALOLA,SPECIES_HITMONLEE,SPECIES_HITMONCHAN,SPECIES_HITMONTOP,SPECIES_LICKITUNG,SPECIES_KOFFING,SPECIES_RHYHORN,SPECIES_HAPPINY,SPECIES_TANGELA,SPECIES_SEADRA,SPECIES_SEAKING,SPECIES_STARYU,SPECIES_MIME_JR,SPECIES_JYNX,SPECIES_ELEKID,SPECIES_MAGBY,SPECIES_MAGIKARP,SPECIES_DITTO,SPECIES_EEVEE,SPECIES_PORYGON,SPECIES_OMANYTE,SPECIES_KABUTO,SPECIES_MUNCHLAX,SPECIES_DRAGONAIR,SPECIES_BAYLEEF,SPECIES_QUILAVA,SPECIES_CROCONAW,SPECIES_FURRET,SPECIES_NOCTOWL,SPECIES_LEDIAN,SPECIES_ARIADOS,SPECIES_CHINCHOU,SPECIES_TOGETIC,SPECIES_NATU,SPECIES_FLAAFFY,SPECIES_MARILL,SPECIES_SUDOWOODO,SPECIES_SKIPLOOM,SPECIES_AIPOM,SPECIES_SUNFLORA,SPECIES_YANMA,SPECIES_WOOPER,SPECIES_WOOPER_PALDEA,SPECIES_MURKROW,SPECIES_MISDREAVUS,SPECIES_WYNAUT,SPECIES_GIRAFARIG,SPECIES_PINECO,SPECIES_DUNSPARCE,SPECIES_GLIGAR,SPECIES_GRANBULL,SPECIES_QWILFISH,SPECIES_QWILFISH_HISUI,SPECIES_SNEASEL,SPECIES_SNEASEL_HISUI,SPECIES_TEDDIURSA,SPECIES_MAGCARGO,SPECIES_PILOSWINE,SPECIES_CORSOLA,SPECIES_CORSOLA_GALAR,SPECIES_REMORAID,SPECIES_DELIBIRD,SPECIES_MANTYKE,SPECIES_HOUNDOUR,SPECIES_PHANPY,SPECIES_PUPITAR,SPECIES_GROVYLE,SPECIES_COMBUSKEN,SPECIES_MARSHTOMP,SPECIES_MIGHTYENA,SPECIES_LINOONE,SPECIES_LINOONE_GALAR,SPECIES_BEAUTIFLY,SPECIES_DUSTOX,SPECIES_LOMBRE,SPECIES_NUZLEAF,SPECIES_SWELLOW,SPECIES_PELIPPER,SPECIES_KIRLIA,SPECIES_MASQUERAIN,SPECIES_SHROOMISH,SPECIES_VIGOROTH,SPECIES_NINJASK,SPECIES_LOUDRED,SPECIES_MAKUHITA,SPECIES_NOSEPASS,SPECIES_DELCATTY,SPECIES_SABLEYE,SPECIES_MAWILE,SPECIES_LAIRON,SPECIES_MEDICHAM,SPECIES_ELECTRIKE,SPECIES_PLUSLE,SPECIES_MINUN,SPECIES_VOLBEAT,SPECIES_ILLUMISE,SPECIES_ROSELIA,SPECIES_GULPIN,SPECIES_CARVANHA,SPECIES_WAILMER,SPECIES_NUMEL,SPECIES_SPOINK,SPECIES_SPINDA,SPECIES_VIBRAVA,SPECIES_CACNEA,SPECIES_SWABLU,SPECIES_BARBOACH,SPECIES_CORPHISH,SPECIES_BALTOY,SPECIES_LILEEP,SPECIES_ANORITH,SPECIES_FEEBAS,SPECIES_CASTFORM,SPECIES_KECLEON,SPECIES_BANETTE,SPECIES_DUSKULL,SPECIES_CHIMECHO,SPECIES_SNORUNT,SPECIES_SEALEO,SPECIES_CLAMPERL,SPECIES_LUVDISC,SPECIES_SHELGON,SPECIES_METANG,SPECIES_GROTLE,SPECIES_MONFERNO,SPECIES_PRINPLUP,SPECIES_STARAVIA,SPECIES_BIBAREL,SPECIES_KRICKETUNE,SPECIES_LUXIO,SPECIES_CRANIDOS,SPECIES_SHIELDON,SPECIES_COMBEE,SPECIES_PACHIRISU,SPECIES_BUIZEL,SPECIES_CHERRIM,SPECIES_SHELLOS,SPECIES_DRIFLOON,SPECIES_BUNEARY,SPECIES_PURUGLY,SPECIES_STUNKY,SPECIES_BRONZOR,SPECIES_CHATOT,SPECIES_GABITE,SPECIES_RIOLU,SPECIES_HIPPOPOTAS,SPECIES_SKORUPI,SPECIES_CROAGUNK,SPECIES_CARNIVINE,SPECIES_FINNEON,SPECIES_SNOVER,SPECIES_ROTOM,SPECIES_SERVINE,SPECIES_PIGNITE,SPECIES_DEWOTT,SPECIES_WATCHOG,SPECIES_HERDIER,SPECIES_LIEPARD,SPECIES_PANSAGE,SPECIES_PANSEAR,SPECIES_PANPOUR,SPECIES_MUNNA,SPECIES_TRANQUILL,SPECIES_BLITZLE,SPECIES_BOLDORE,SPECIES_SWOOBAT,SPECIES_DRILBUR,SPECIES_AUDINO,SPECIES_GURDURR,SPECIES_PALPITOAD,SPECIES_SWADLOON,SPECIES_WHIRLIPEDE,SPECIES_COTTONEE,SPECIES_PETILIL,SPECIES_KROKOROK,SPECIES_DARUMAKA,SPECIES_DARUMAKA_GALAR,SPECIES_DWEBBLE,SPECIES_SCRAGGY,SPECIES_YAMASK,SPECIES_YAMASK_GALAR,SPECIES_TIRTOUGA,SPECIES_ARCHEN,SPECIES_TRUBBISH,SPECIES_ZORUA,SPECIES_ZORUA_HISUI,SPECIES_MINCCINO,SPECIES_GOTHORITA,SPECIES_DUOSION,SPECIES_DUCKLETT,SPECIES_VANILLISH,SPECIES_DEERLING,SPECIES_EMOLGA,SPECIES_KARRABLAST,SPECIES_FOONGUS,SPECIES_FRILLISH,SPECIES_JOLTIK,SPECIES_FERROSEED,SPECIES_KLANG,SPECIES_EELEKTRIK,SPECIES_ELGYEM,SPECIES_LAMPENT,SPECIES_FRAXURE,SPECIES_CUBCHOO,SPECIES_SHELMET,SPECIES_MIENFOO,SPECIES_GOLETT,SPECIES_PAWNIARD,SPECIES_RUFFLET,SPECIES_VULLABY,SPECIES_DEINO,SPECIES_LARVESTA,SPECIES_QUILLADIN,SPECIES_BRAIXEN,SPECIES_FROGADIER,SPECIES_BUNNELBY,SPECIES_FLETCHINDER,SPECIES_VIVILLON,SPECIES_LITLEO,SPECIES_FLOETTE,SPECIES_SKIDDO,SPECIES_PANCHAM,SPECIES_ESPURR,SPECIES_HONEDGE,SPECIES_SPRITZEE,SPECIES_SWIRLIX,SPECIES_INKAY,SPECIES_BINACLE,SPECIES_SKRELP,SPECIES_CLAUNCHER,SPECIES_HELIOPTILE,SPECIES_TYRUNT,SPECIES_AMAURA,SPECIES_DEDENNE,SPECIES_SLIGGOO,SPECIES_SLIGGOO_HISUI,SPECIES_PHANTUMP,SPECIES_PUMPKABOO,SPECIES_BERGMITE,SPECIES_NOIBAT,SPECIES_DARTRIX,SPECIES_TORRACAT,SPECIES_BRIONNE,SPECIES_TRUMBEAK,SPECIES_GUMSHOOS,SPECIES_CHARJABUG,SPECIES_CRABRAWLER,SPECIES_CUTIEFLY,SPECIES_ROCKRUFF,SPECIES_WISHIWASHI,SPECIES_MAREANIE,SPECIES_MUDBRAY,SPECIES_ARAQUANID,SPECIES_FOMANTIS,SPECIES_SHIINOTIC,SPECIES_SALANDIT,SPECIES_STUFFUL,SPECIES_STEENEE,SPECIES_WIMPOD,SPECIES_SANDYGAST,SPECIES_TOGEDEMARU,SPECIES_HAKAMO_O,SPECIES_THWACKEY,SPECIES_RABOOT,SPECIES_DRIZZILE,SPECIES_SKWOVET,SPECIES_CORVISQUIRE,SPECIES_DOTTLER,SPECIES_THIEVUL,SPECIES_GOSSIFLEUR,SPECIES_WOOLOO,SPECIES_DREDNAW,SPECIES_BOLTUND,SPECIES_CARKOL,SPECIES_APPLIN,SPECIES_SILICOBRA,SPECIES_ARROKUDA,SPECIES_TOXEL,SPECIES_SIZZLIPEDE,SPECIES_CLOBBOPUS,SPECIES_SINISTEA,SPECIES_HATTREM,SPECIES_MORGREM,SPECIES_MILCERY,SPECIES_PINCURCHIN,SPECIES_SNOM,SPECIES_MORPEKO,SPECIES_CUFANT,SPECIES_DRAKLOAK,SPECIES_FLORAGATO,SPECIES_CROCALOR,SPECIES_QUAXWELL,SPECIES_LECHONK,SPECIES_SPIDOPS,SPECIES_LOKIX,SPECIES_PAWMO,SPECIES_TANDEMAUS,SPECIES_FIDOUGH,SPECIES_DOLLIV,SPECIES_SQUAWKABILLY,SPECIES_NACLSTACK,SPECIES_CHARCADET,SPECIES_TADBULB,SPECIES_WATTREL,SPECIES_MASCHIFF,SPECIES_SHROODLE,SPECIES_BRAMBLIN,SPECIES_TOEDSCOOL,SPECIES_KLAWF,SPECIES_CAPSAKID,SPECIES_RELLOR,SPECIES_FLITTLE,SPECIES_TINKATUFF,SPECIES_WUGTRIO,SPECIES_FINIZEN,SPECIES_VAROOM,SPECIES_GLIMMET,SPECIES_GREAVARD,SPECIES_CETODDLE,SPECIES_ARCTIBAX,SPECIES_GIMMIGHOUL,SPECIES_POLTCHAGEIST};
            GenerateOptions(speciesListSplit4, 381);
        }
        else if(FlagGet(FLAG_SPLIT_2_DONE))
        {
            int speciesListSplit3[] = {SPECIES_IVYSAUR,SPECIES_CHARMELEON,SPECIES_WARTORTLE,SPECIES_BUTTERFREE,SPECIES_BEEDRILL,SPECIES_PIDGEOTTO,SPECIES_RATICATE,SPECIES_RATICATE_ALOLA,SPECIES_SPEAROW,SPECIES_EKANS,SPECIES_PIKACHU,SPECIES_SANDSHREW,SPECIES_SANDSHREW_ALOLA,SPECIES_NIDORINA,SPECIES_NIDORINO,SPECIES_CLEFAIRY,SPECIES_VULPIX,SPECIES_VULPIX_ALOLA,SPECIES_JIGGLYPUFF,SPECIES_ZUBAT,SPECIES_GLOOM,SPECIES_PARASECT,SPECIES_VENONAT,SPECIES_DIGLETT,SPECIES_DIGLETT_ALOLA,SPECIES_MEOWTH,SPECIES_MEOWTH_ALOLA,SPECIES_MEOWTH_GALAR,SPECIES_PSYDUCK,SPECIES_MANKEY,SPECIES_GROWLITHE,SPECIES_GROWLITHE_HISUI,SPECIES_POLIWHIRL,SPECIES_KADABRA,SPECIES_MACHOKE,SPECIES_WEEPINBELL,SPECIES_TENTACOOL,SPECIES_GRAVELER,SPECIES_GRAVELER_ALOLA,SPECIES_PONYTA,SPECIES_PONYTA_GALAR,SPECIES_SLOWPOKE,SPECIES_SLOWPOKE_GALAR,SPECIES_MAGNEMITE,SPECIES_FARFETCHD,SPECIES_FARFETCHD_GALAR,SPECIES_DODUO,SPECIES_SEEL,SPECIES_GRIMER,SPECIES_GRIMER_ALOLA,SPECIES_SHELLDER,SPECIES_HAUNTER,SPECIES_ONIX,SPECIES_DROWZEE,SPECIES_KRABBY,SPECIES_VOLTORB,SPECIES_VOLTORB_HISUI,SPECIES_EXEGGCUTE,SPECIES_CUBONE,SPECIES_TYROGUE,SPECIES_LICKITUNG,SPECIES_KOFFING,SPECIES_RHYHORN,SPECIES_HAPPINY,SPECIES_TANGELA,SPECIES_HORSEA,SPECIES_GOLDEEN,SPECIES_STARYU,SPECIES_MIME_JR,SPECIES_SMOOCHUM,SPECIES_ELEKID,SPECIES_MAGBY,SPECIES_MAGIKARP,SPECIES_DITTO,SPECIES_EEVEE,SPECIES_PORYGON,SPECIES_OMANYTE,SPECIES_KABUTO,SPECIES_MUNCHLAX,SPECIES_DRAGONAIR,SPECIES_BAYLEEF,SPECIES_QUILAVA,SPECIES_CROCONAW,SPECIES_FURRET,SPECIES_HOOTHOOT,SPECIES_LEDIAN,SPECIES_ARIADOS,SPECIES_CHINCHOU,SPECIES_TOGETIC,SPECIES_NATU,SPECIES_FLAAFFY,SPECIES_MARILL,SPECIES_SUDOWOODO,SPECIES_SKIPLOOM,SPECIES_AIPOM,SPECIES_SUNKERN,SPECIES_YANMA,SPECIES_WOOPER,SPECIES_WOOPER_PALDEA,SPECIES_MURKROW,SPECIES_WYNAUT,SPECIES_PINECO,SPECIES_DUNSPARCE,SPECIES_SNUBBULL,SPECIES_TEDDIURSA,SPECIES_SLUGMA,SPECIES_SWINUB,SPECIES_CORSOLA,SPECIES_CORSOLA_GALAR,SPECIES_REMORAID,SPECIES_DELIBIRD,SPECIES_MANTYKE,SPECIES_HOUNDOUR,SPECIES_PHANPY,SPECIES_PUPITAR,SPECIES_GROVYLE,SPECIES_COMBUSKEN,SPECIES_MARSHTOMP,SPECIES_MIGHTYENA,SPECIES_LINOONE,SPECIES_LINOONE_GALAR,SPECIES_BEAUTIFLY,SPECIES_DUSTOX,SPECIES_LOMBRE,SPECIES_NUZLEAF,SPECIES_TAILLOW,SPECIES_WINGULL,SPECIES_KIRLIA,SPECIES_SURSKIT,SPECIES_SHROOMISH,SPECIES_SLAKOTH,SPECIES_NINJASK,SPECIES_LOUDRED,SPECIES_MAKUHITA,SPECIES_NOSEPASS,SPECIES_DELCATTY,SPECIES_SABLEYE,SPECIES_MAWILE,SPECIES_ARON,SPECIES_MEDICHAM,SPECIES_ELECTRIKE,SPECIES_PLUSLE,SPECIES_MINUN,SPECIES_ROSELIA,SPECIES_GULPIN,SPECIES_CARVANHA,SPECIES_WAILMER,SPECIES_NUMEL,SPECIES_SPOINK,SPECIES_SPINDA,SPECIES_VIBRAVA,SPECIES_CACNEA,SPECIES_SWABLU,SPECIES_BARBOACH,SPECIES_CORPHISH,SPECIES_BALTOY,SPECIES_LILEEP,SPECIES_ANORITH,SPECIES_FEEBAS,SPECIES_CASTFORM,SPECIES_KECLEON,SPECIES_SHUPPET,SPECIES_DUSKULL,SPECIES_CHINGLING,SPECIES_SNORUNT,SPECIES_SEALEO,SPECIES_CLAMPERL,SPECIES_LUVDISC,SPECIES_BAGON,SPECIES_BELDUM,SPECIES_GROTLE,SPECIES_MONFERNO,SPECIES_PRINPLUP,SPECIES_STARAVIA,SPECIES_BIBAREL,SPECIES_KRICKETUNE,SPECIES_LUXIO,SPECIES_CRANIDOS,SPECIES_SHIELDON,SPECIES_COMBEE,SPECIES_PACHIRISU,SPECIES_BUIZEL,SPECIES_CHERUBI,SPECIES_SHELLOS,SPECIES_DRIFLOON,SPECIES_BUNEARY,SPECIES_GLAMEOW,SPECIES_STUNKY,SPECIES_BRONZOR,SPECIES_CHATOT,SPECIES_GABITE,SPECIES_RIOLU,SPECIES_HIPPOPOTAS,SPECIES_SKORUPI,SPECIES_CROAGUNK,SPECIES_FINNEON,SPECIES_SNOVER,SPECIES_SERVINE,SPECIES_PIGNITE,SPECIES_DEWOTT,SPECIES_WATCHOG,SPECIES_HERDIER,SPECIES_PURRLOIN,SPECIES_PANSAGE,SPECIES_PANSEAR,SPECIES_PANPOUR,SPECIES_MUNNA,SPECIES_TRANQUILL,SPECIES_BLITZLE,SPECIES_BOLDORE,SPECIES_WOOBAT,SPECIES_DRILBUR,SPECIES_GURDURR,SPECIES_PALPITOAD,SPECIES_SWADLOON,SPECIES_WHIRLIPEDE,SPECIES_COTTONEE,SPECIES_PETILIL,SPECIES_KROKOROK,SPECIES_DARUMAKA,SPECIES_DARUMAKA_GALAR,SPECIES_DWEBBLE,SPECIES_SCRAGGY,SPECIES_YAMASK,SPECIES_YAMASK_GALAR,SPECIES_TIRTOUGA,SPECIES_ARCHEN,SPECIES_TRUBBISH,SPECIES_ZORUA,SPECIES_ZORUA_HISUI,SPECIES_MINCCINO,SPECIES_GOTHORITA,SPECIES_DUOSION,SPECIES_DUCKLETT,SPECIES_VANILLISH,SPECIES_DEERLING,SPECIES_KARRABLAST,SPECIES_FOONGUS,SPECIES_FRILLISH,SPECIES_JOLTIK,SPECIES_KLINK,SPECIES_EELEKTRIK,SPECIES_ELGYEM,SPECIES_LITWICK,SPECIES_FRAXURE,SPECIES_CUBCHOO,SPECIES_SHELMET,SPECIES_MIENFOO,SPECIES_GOLETT,SPECIES_PAWNIARD,SPECIES_RUFFLET,SPECIES_VULLABY,SPECIES_DEINO,SPECIES_LARVESTA,SPECIES_QUILLADIN,SPECIES_BRAIXEN,SPECIES_FROGADIER,SPECIES_BUNNELBY,SPECIES_FLETCHINDER,SPECIES_VIVILLON,SPECIES_LITLEO,SPECIES_FLOETTE,SPECIES_SKIDDO,SPECIES_PANCHAM,SPECIES_ESPURR,SPECIES_HONEDGE,SPECIES_SPRITZEE,SPECIES_SWIRLIX,SPECIES_INKAY,SPECIES_BINACLE,SPECIES_SKRELP,SPECIES_CLAUNCHER,SPECIES_HELIOPTILE,SPECIES_TYRUNT,SPECIES_AMAURA,SPECIES_GOOMY,SPECIES_PHANTUMP,SPECIES_PUMPKABOO,SPECIES_BERGMITE,SPECIES_NOIBAT,SPECIES_DARTRIX,SPECIES_TORRACAT,SPECIES_BRIONNE,SPECIES_TRUMBEAK,SPECIES_GUMSHOOS,SPECIES_CHARJABUG,SPECIES_CRABRAWLER,SPECIES_CUTIEFLY,SPECIES_ROCKRUFF,SPECIES_WISHIWASHI,SPECIES_MAREANIE,SPECIES_MUDBRAY,SPECIES_DEWPIDER,SPECIES_FOMANTIS,SPECIES_SHIINOTIC,SPECIES_SALANDIT,SPECIES_STUFFUL,SPECIES_STEENEE,SPECIES_WIMPOD,SPECIES_SANDYGAST,SPECIES_JANGMO_O,SPECIES_THWACKEY,SPECIES_RABOOT,SPECIES_DRIZZILE,SPECIES_SKWOVET,SPECIES_CORVISQUIRE,SPECIES_DOTTLER,SPECIES_NICKIT,SPECIES_GOSSIFLEUR,SPECIES_WOOLOO,SPECIES_CHEWTLE,SPECIES_YAMPER,SPECIES_CARKOL,SPECIES_APPLIN,SPECIES_SILICOBRA,SPECIES_ARROKUDA,SPECIES_TOXEL,SPECIES_SIZZLIPEDE,SPECIES_CLOBBOPUS,SPECIES_SINISTEA,SPECIES_HATTREM,SPECIES_MORGREM,SPECIES_MILCERY,SPECIES_SNOM,SPECIES_CUFANT,SPECIES_DREEPY,SPECIES_FLORAGATO,SPECIES_CROCALOR,SPECIES_QUAXWELL,SPECIES_LECHONK,SPECIES_SPIDOPS,SPECIES_NYMBLE,SPECIES_PAWMO,SPECIES_TANDEMAUS,SPECIES_FIDOUGH,SPECIES_DOLLIV,SPECIES_SQUAWKABILLY,SPECIES_NACLSTACK,SPECIES_CHARCADET,SPECIES_TADBULB,SPECIES_WATTREL,SPECIES_MASCHIFF,SPECIES_SHROODLE,SPECIES_BRAMBLIN,SPECIES_TOEDSCOOL,SPECIES_CAPSAKID,SPECIES_RELLOR,SPECIES_FLITTLE,SPECIES_TINKATINK,SPECIES_WIGLETT,SPECIES_FINIZEN,SPECIES_VAROOM,SPECIES_GLIMMET,SPECIES_GREAVARD,SPECIES_CETODDLE,SPECIES_FRIGIBAX,SPECIES_GIMMIGHOUL,SPECIES_POLTCHAGEIST};
            GenerateOptions(speciesListSplit3, 358);
        }
        else if(FlagGet(FLAG_SPLIT_1_DONE))
        {
            int speciesListSplit2[] = {SPECIES_BULBASAUR,SPECIES_CHARMANDER,SPECIES_SQUIRTLE,SPECIES_BUTTERFREE,SPECIES_BEEDRILL,SPECIES_PIDGEOTTO,SPECIES_RATTATA,SPECIES_RATTATA_ALOLA,SPECIES_SPEAROW,SPECIES_EKANS,SPECIES_PIKACHU,SPECIES_SANDSHREW,SPECIES_SANDSHREW_ALOLA,SPECIES_NIDORINA,SPECIES_NIDORINO,SPECIES_CLEFAIRY,SPECIES_VULPIX,SPECIES_VULPIX_ALOLA,SPECIES_JIGGLYPUFF,SPECIES_ZUBAT,SPECIES_GLOOM,SPECIES_PARAS,SPECIES_VENONAT,SPECIES_DIGLETT,SPECIES_DIGLETT_ALOLA,SPECIES_MEOWTH,SPECIES_MEOWTH_ALOLA,SPECIES_MEOWTH_GALAR,SPECIES_PSYDUCK,SPECIES_MANKEY,SPECIES_GROWLITHE,SPECIES_GROWLITHE_HISUI,SPECIES_POLIWHIRL,SPECIES_ABRA,SPECIES_MACHOP,SPECIES_WEEPINBELL,SPECIES_TENTACOOL,SPECIES_GRAVELER,SPECIES_GRAVELER_ALOLA,SPECIES_PONYTA,SPECIES_PONYTA_GALAR,SPECIES_SLOWPOKE,SPECIES_SLOWPOKE_GALAR,SPECIES_MAGNEMITE,SPECIES_FARFETCHD,SPECIES_FARFETCHD_GALAR,SPECIES_DODUO,SPECIES_SEEL,SPECIES_GRIMER,SPECIES_GRIMER_ALOLA,SPECIES_SHELLDER,SPECIES_GASTLY,SPECIES_ONIX,SPECIES_DROWZEE,SPECIES_KRABBY,SPECIES_VOLTORB,SPECIES_VOLTORB_HISUI,SPECIES_EXEGGCUTE,SPECIES_CUBONE,SPECIES_TYROGUE,SPECIES_LICKITUNG,SPECIES_KOFFING,SPECIES_RHYHORN,SPECIES_HAPPINY,SPECIES_HORSEA,SPECIES_GOLDEEN,SPECIES_STARYU,SPECIES_MIME_JR,SPECIES_SMOOCHUM,SPECIES_ELEKID,SPECIES_MAGBY,SPECIES_MAGIKARP,SPECIES_DITTO,SPECIES_EEVEE,SPECIES_PORYGON,SPECIES_OMANYTE,SPECIES_KABUTO,SPECIES_MUNCHLAX,SPECIES_DRATINI,SPECIES_CHIKORITA,SPECIES_CYNDAQUIL,SPECIES_TOTODILE,SPECIES_SENTRET,SPECIES_HOOTHOOT,SPECIES_LEDIAN,SPECIES_SPINARAK,SPECIES_CHINCHOU,SPECIES_TOGEPI,SPECIES_NATU,SPECIES_FLAAFFY,SPECIES_MARILL,SPECIES_BONSLY,SPECIES_SKIPLOOM,SPECIES_AIPOM,SPECIES_SUNKERN,SPECIES_YANMA,SPECIES_WOOPER,SPECIES_WOOPER_PALDEA,SPECIES_WYNAUT,SPECIES_PINECO,SPECIES_SNUBBULL,SPECIES_TEDDIURSA,SPECIES_SLUGMA,SPECIES_SWINUB,SPECIES_REMORAID,SPECIES_DELIBIRD,SPECIES_MANTYKE,SPECIES_HOUNDOUR,SPECIES_PHANPY,SPECIES_LARVITAR,SPECIES_TREECKO,SPECIES_TORCHIC,SPECIES_MUDKIP,SPECIES_POOCHYENA,SPECIES_ZIGZAGOON,SPECIES_ZIGZAGOON_GALAR,SPECIES_BEAUTIFLY,SPECIES_DUSTOX,SPECIES_LOMBRE,SPECIES_NUZLEAF,SPECIES_TAILLOW,SPECIES_WINGULL,SPECIES_KIRLIA,SPECIES_SURSKIT,SPECIES_SHROOMISH,SPECIES_SLAKOTH,SPECIES_NINCADA,SPECIES_LOUDRED,SPECIES_MAKUHITA,SPECIES_NOSEPASS,SPECIES_SKITTY,SPECIES_SABLEYE,SPECIES_MAWILE,SPECIES_ARON,SPECIES_MEDITITE,SPECIES_ELECTRIKE,SPECIES_PLUSLE,SPECIES_MINUN,SPECIES_BUDEW,SPECIES_GULPIN,SPECIES_CARVANHA,SPECIES_NUMEL,SPECIES_SPOINK,SPECIES_SPINDA,SPECIES_VIBRAVA,SPECIES_CACNEA,SPECIES_SWABLU,SPECIES_BARBOACH,SPECIES_CORPHISH,SPECIES_BALTOY,SPECIES_LILEEP,SPECIES_ANORITH,SPECIES_FEEBAS,SPECIES_SHUPPET,SPECIES_DUSKULL,SPECIES_CHINGLING,SPECIES_SNORUNT,SPECIES_SPHEAL,SPECIES_CLAMPERL,SPECIES_LUVDISC,SPECIES_BAGON,SPECIES_BELDUM,SPECIES_TURTWIG,SPECIES_CHIMCHAR,SPECIES_PIPLUP,SPECIES_STARAVIA,SPECIES_BIDOOF,SPECIES_KRICKETUNE,SPECIES_LUXIO,SPECIES_CRANIDOS,SPECIES_SHIELDON,SPECIES_COMBEE,SPECIES_BUIZEL,SPECIES_CHERUBI,SPECIES_SHELLOS,SPECIES_DRIFLOON,SPECIES_BUNEARY,SPECIES_GLAMEOW,SPECIES_STUNKY,SPECIES_BRONZOR,SPECIES_GIBLE,SPECIES_RIOLU,SPECIES_HIPPOPOTAS,SPECIES_SKORUPI,SPECIES_CROAGUNK,SPECIES_FINNEON,SPECIES_SNOVER,SPECIES_SNIVY,SPECIES_TEPIG,SPECIES_OSHAWOTT,SPECIES_PATRAT,SPECIES_HERDIER,SPECIES_PURRLOIN,SPECIES_PANSAGE,SPECIES_PANSEAR,SPECIES_PANPOUR,SPECIES_MUNNA,SPECIES_TRANQUILL,SPECIES_BLITZLE,SPECIES_BOLDORE,SPECIES_WOOBAT,SPECIES_DRILBUR,SPECIES_TIMBURR,SPECIES_TYMPOLE,SPECIES_SWADLOON,SPECIES_WHIRLIPEDE,SPECIES_COTTONEE,SPECIES_PETILIL,SPECIES_SANDILE,SPECIES_DARUMAKA,SPECIES_DARUMAKA_GALAR,SPECIES_DWEBBLE,SPECIES_SCRAGGY,SPECIES_YAMASK,SPECIES_YAMASK_GALAR,SPECIES_TIRTOUGA,SPECIES_TRUBBISH,SPECIES_ZORUA,SPECIES_ZORUA_HISUI,SPECIES_MINCCINO,SPECIES_GOTHORITA,SPECIES_DUOSION,SPECIES_DUCKLETT,SPECIES_VANILLISH,SPECIES_DEERLING,SPECIES_KARRABLAST,SPECIES_FOONGUS,SPECIES_FRILLISH,SPECIES_JOLTIK,SPECIES_KLINK,SPECIES_TYNAMO,SPECIES_ELGYEM,SPECIES_LITWICK,SPECIES_AXEW,SPECIES_CUBCHOO,SPECIES_SHELMET,SPECIES_MIENFOO,SPECIES_GOLETT,SPECIES_PAWNIARD,SPECIES_RUFFLET,SPECIES_VULLABY,SPECIES_DEINO,SPECIES_LARVESTA,SPECIES_CHESPIN,SPECIES_FENNEKIN,SPECIES_FROAKIE,SPECIES_BUNNELBY,SPECIES_FLETCHINDER,SPECIES_SPEWPA,SPECIES_LITLEO,SPECIES_FLABEBE,SPECIES_SKIDDO,SPECIES_PANCHAM,SPECIES_ESPURR,SPECIES_HONEDGE,SPECIES_SPRITZEE,SPECIES_SWIRLIX,SPECIES_INKAY,SPECIES_BINACLE,SPECIES_SKRELP,SPECIES_CLAUNCHER,SPECIES_HELIOPTILE,SPECIES_TYRUNT,SPECIES_AMAURA,SPECIES_GOOMY,SPECIES_PHANTUMP,SPECIES_PUMPKABOO,SPECIES_BERGMITE,SPECIES_NOIBAT,SPECIES_ROWLET,SPECIES_LITTEN,SPECIES_POPPLIO,SPECIES_TRUMBEAK,SPECIES_YUNGOOS,SPECIES_GRUBBIN,SPECIES_CRABRAWLER,SPECIES_CUTIEFLY,SPECIES_ROCKRUFF,SPECIES_WISHIWASHI,SPECIES_MAREANIE,SPECIES_MUDBRAY,SPECIES_DEWPIDER,SPECIES_FOMANTIS,SPECIES_MORELULL,SPECIES_SALANDIT,SPECIES_STUFFUL,SPECIES_STEENEE,SPECIES_WIMPOD,SPECIES_SANDYGAST,SPECIES_JANGMO_O,SPECIES_GROOKEY,SPECIES_SCORBUNNY,SPECIES_SOBBLE,SPECIES_SKWOVET,SPECIES_CORVISQUIRE,SPECIES_DOTTLER,SPECIES_NICKIT,SPECIES_GOSSIFLEUR,SPECIES_WOOLOO,SPECIES_CHEWTLE,SPECIES_YAMPER,SPECIES_ROLYCOLY,SPECIES_APPLIN,SPECIES_SILICOBRA,SPECIES_ARROKUDA,SPECIES_TOXEL,SPECIES_SIZZLIPEDE,SPECIES_CLOBBOPUS,SPECIES_SINISTEA,SPECIES_HATTREM,SPECIES_MORGREM,SPECIES_MILCERY,SPECIES_SNOM,SPECIES_CUFANT,SPECIES_DREEPY,SPECIES_SPRIGATITO,SPECIES_FUECOCO,SPECIES_QUAXLY,SPECIES_LECHONK,SPECIES_TAROUNTULA,SPECIES_NYMBLE,SPECIES_PAWMO,SPECIES_TANDEMAUS,SPECIES_FIDOUGH,SPECIES_DOLLIV,SPECIES_NACLSTACK,SPECIES_CHARCADET,SPECIES_TADBULB,SPECIES_WATTREL,SPECIES_MASCHIFF,SPECIES_SHROODLE,SPECIES_BRAMBLIN,SPECIES_TOEDSCOOL,SPECIES_CAPSAKID,SPECIES_RELLOR,SPECIES_FLITTLE,SPECIES_TINKATINK,SPECIES_WIGLETT,SPECIES_FINIZEN,SPECIES_VAROOM,SPECIES_GLIMMET,SPECIES_GREAVARD,SPECIES_CETODDLE,SPECIES_FRIGIBAX,SPECIES_GIMMIGHOUL,SPECIES_POLTCHAGEIST};
            GenerateOptions(speciesListSplit2, 346);
        }
        else
        {
            int speciesListSplit1[] = {SPECIES_BULBASAUR,SPECIES_CHARMANDER,SPECIES_SQUIRTLE,SPECIES_BUTTERFREE,SPECIES_BEEDRILL,SPECIES_PIDGEOTTO,SPECIES_RATTATA,SPECIES_RATTATA_ALOLA,SPECIES_SPEAROW,SPECIES_EKANS,SPECIES_PIKACHU,SPECIES_SANDSHREW,SPECIES_SANDSHREW_ALOLA,SPECIES_NIDORAN_F,SPECIES_NIDORAN_M,SPECIES_CLEFAIRY,SPECIES_VULPIX,SPECIES_VULPIX_ALOLA,SPECIES_JIGGLYPUFF,SPECIES_ZUBAT,SPECIES_ODDISH,SPECIES_PARAS,SPECIES_VENONAT,SPECIES_DIGLETT,SPECIES_DIGLETT_ALOLA,SPECIES_MEOWTH,SPECIES_MEOWTH_ALOLA,SPECIES_MEOWTH_GALAR,SPECIES_PSYDUCK,SPECIES_MANKEY,SPECIES_GROWLITHE,SPECIES_GROWLITHE_HISUI,SPECIES_POLIWAG,SPECIES_ABRA,SPECIES_MACHOP,SPECIES_BELLSPROUT,SPECIES_TENTACOOL,SPECIES_GEODUDE,SPECIES_GEODUDE_ALOLA,SPECIES_SLOWPOKE,SPECIES_SLOWPOKE_GALAR,SPECIES_MAGNEMITE,SPECIES_FARFETCHD,SPECIES_DODUO,SPECIES_SEEL,SPECIES_GRIMER,SPECIES_GRIMER_ALOLA,SPECIES_SHELLDER,SPECIES_GASTLY,SPECIES_DROWZEE,SPECIES_KRABBY,SPECIES_VOLTORB,SPECIES_VOLTORB_HISUI,SPECIES_EXEGGCUTE,SPECIES_TYROGUE,SPECIES_LICKITUNG,SPECIES_KOFFING,SPECIES_HAPPINY,SPECIES_HORSEA,SPECIES_GOLDEEN,SPECIES_STARYU,SPECIES_MIME_JR,SPECIES_SMOOCHUM,SPECIES_ELEKID,SPECIES_MAGIKARP,SPECIES_DITTO,SPECIES_EEVEE,SPECIES_DRATINI,SPECIES_CHIKORITA,SPECIES_CYNDAQUIL,SPECIES_TOTODILE,SPECIES_SENTRET,SPECIES_HOOTHOOT,SPECIES_LEDYBA,SPECIES_SPINARAK,SPECIES_CHINCHOU,SPECIES_TOGEPI,SPECIES_NATU,SPECIES_MAREEP,SPECIES_AZURILL,SPECIES_BONSLY,SPECIES_HOPPIP,SPECIES_AIPOM,SPECIES_SUNKERN,SPECIES_WOOPER,SPECIES_WOOPER_PALDEA,SPECIES_WYNAUT,SPECIES_PINECO,SPECIES_SNUBBULL,SPECIES_TEDDIURSA,SPECIES_SLUGMA,SPECIES_SWINUB,SPECIES_REMORAID,SPECIES_DELIBIRD,SPECIES_MANTYKE,SPECIES_HOUNDOUR,SPECIES_PHANPY,SPECIES_LARVITAR,SPECIES_TREECKO,SPECIES_TORCHIC,SPECIES_MUDKIP,SPECIES_POOCHYENA,SPECIES_ZIGZAGOON,SPECIES_ZIGZAGOON_GALAR,SPECIES_BEAUTIFLY,SPECIES_DUSTOX,SPECIES_LOTAD,SPECIES_SEEDOT,SPECIES_TAILLOW,SPECIES_WINGULL,SPECIES_RALTS,SPECIES_SURSKIT,SPECIES_SHROOMISH,SPECIES_SLAKOTH,SPECIES_NINCADA,SPECIES_WHISMUR,SPECIES_MAKUHITA,SPECIES_SKITTY,SPECIES_ARON,SPECIES_MEDITITE,SPECIES_ELECTRIKE,SPECIES_PLUSLE,SPECIES_MINUN,SPECIES_BUDEW,SPECIES_GULPIN,SPECIES_CARVANHA,SPECIES_NUMEL,SPECIES_SPOINK,SPECIES_SPINDA,SPECIES_TRAPINCH,SPECIES_CACNEA,SPECIES_SWABLU,SPECIES_BARBOACH,SPECIES_CORPHISH,SPECIES_BALTOY,SPECIES_LILEEP,SPECIES_ANORITH,SPECIES_FEEBAS,SPECIES_SHUPPET,SPECIES_DUSKULL,SPECIES_CHINGLING,SPECIES_SNORUNT,SPECIES_SPHEAL,SPECIES_CLAMPERL,SPECIES_LUVDISC,SPECIES_BELDUM,SPECIES_TURTWIG,SPECIES_CHIMCHAR,SPECIES_PIPLUP,SPECIES_STARLY,SPECIES_BIDOOF,SPECIES_KRICKETOT,SPECIES_SHINX,SPECIES_CRANIDOS,SPECIES_SHIELDON,SPECIES_COMBEE,SPECIES_BUIZEL,SPECIES_CHERUBI,SPECIES_SHELLOS,SPECIES_DRIFLOON,SPECIES_BUNEARY,SPECIES_GLAMEOW,SPECIES_STUNKY,SPECIES_BRONZOR,SPECIES_GIBLE,SPECIES_RIOLU,SPECIES_HIPPOPOTAS,SPECIES_SKORUPI,SPECIES_CROAGUNK,SPECIES_FINNEON,SPECIES_SNOVER,SPECIES_SNIVY,SPECIES_TEPIG,SPECIES_OSHAWOTT,SPECIES_PATRAT,SPECIES_LILLIPUP,SPECIES_PURRLOIN,SPECIES_PANSAGE,SPECIES_PANSEAR,SPECIES_PANPOUR,SPECIES_MUNNA,SPECIES_PIDOVE,SPECIES_BLITZLE,SPECIES_ROGGENROLA,SPECIES_WOOBAT,SPECIES_DRILBUR,SPECIES_TIMBURR,SPECIES_TYMPOLE,SPECIES_SEWADDLE,SPECIES_VENIPEDE,SPECIES_COTTONEE,SPECIES_PETILIL,SPECIES_SANDILE,SPECIES_DARUMAKA,SPECIES_DARUMAKA_GALAR,SPECIES_DWEBBLE,SPECIES_SCRAGGY,SPECIES_YAMASK,SPECIES_YAMASK_GALAR,SPECIES_TIRTOUGA,SPECIES_TRUBBISH,SPECIES_ZORUA,SPECIES_ZORUA_HISUI,SPECIES_MINCCINO,SPECIES_GOTHITA,SPECIES_SOLOSIS,SPECIES_DUCKLETT,SPECIES_VANILLITE,SPECIES_DEERLING,SPECIES_KARRABLAST,SPECIES_FOONGUS,SPECIES_FRILLISH,SPECIES_JOLTIK,SPECIES_KLINK,SPECIES_TYNAMO,SPECIES_ELGYEM,SPECIES_LITWICK,SPECIES_AXEW,SPECIES_CUBCHOO,SPECIES_SHELMET,SPECIES_MIENFOO,SPECIES_GOLETT,SPECIES_RUFFLET,SPECIES_DEINO,SPECIES_LARVESTA,SPECIES_CHESPIN,SPECIES_FENNEKIN,SPECIES_FROAKIE,SPECIES_BUNNELBY,SPECIES_FLETCHLING,SPECIES_SPEWPA,SPECIES_FLABEBE,SPECIES_SKIDDO,SPECIES_PANCHAM,SPECIES_ESPURR,SPECIES_HONEDGE,SPECIES_SPRITZEE,SPECIES_SWIRLIX,SPECIES_INKAY,SPECIES_BINACLE,SPECIES_SKRELP,SPECIES_CLAUNCHER,SPECIES_HELIOPTILE,SPECIES_GOOMY,SPECIES_PHANTUMP,SPECIES_PUMPKABOO,SPECIES_BERGMITE,SPECIES_NOIBAT,SPECIES_ROWLET,SPECIES_LITTEN,SPECIES_POPPLIO,SPECIES_PIKIPEK,SPECIES_YUNGOOS,SPECIES_GRUBBIN,SPECIES_CRABRAWLER,SPECIES_CUTIEFLY,SPECIES_ROCKRUFF,SPECIES_WISHIWASHI,SPECIES_MAREANIE,SPECIES_DEWPIDER,SPECIES_FOMANTIS,SPECIES_MORELULL,SPECIES_SALANDIT,SPECIES_STUFFUL,SPECIES_BOUNSWEET,SPECIES_WIMPOD,SPECIES_SANDYGAST,SPECIES_JANGMO_O,SPECIES_GROOKEY,SPECIES_SCORBUNNY,SPECIES_SOBBLE,SPECIES_SKWOVET,SPECIES_ROOKIDEE,SPECIES_BLIPBUG,SPECIES_NICKIT,SPECIES_GOSSIFLEUR,SPECIES_WOOLOO,SPECIES_CHEWTLE,SPECIES_YAMPER,SPECIES_ROLYCOLY,SPECIES_APPLIN,SPECIES_SILICOBRA,SPECIES_ARROKUDA,SPECIES_TOXEL,SPECIES_SIZZLIPEDE,SPECIES_CLOBBOPUS,SPECIES_SINISTEA,SPECIES_HATENNA,SPECIES_IMPIDIMP,SPECIES_MILCERY,SPECIES_SNOM,SPECIES_CUFANT,SPECIES_DREEPY,SPECIES_SPRIGATITO,SPECIES_FUECOCO,SPECIES_QUAXLY,SPECIES_LECHONK,SPECIES_TAROUNTULA,SPECIES_NYMBLE,SPECIES_PAWMI,SPECIES_TANDEMAUS,SPECIES_FIDOUGH,SPECIES_SMOLIV,SPECIES_NACLI,SPECIES_CHARCADET,SPECIES_TADBULB,SPECIES_WATTREL,SPECIES_MASCHIFF,SPECIES_SHROODLE,SPECIES_BRAMBLIN,SPECIES_TOEDSCOOL,SPECIES_CAPSAKID,SPECIES_RELLOR,SPECIES_FLITTLE,SPECIES_TINKATINK,SPECIES_WIGLETT,SPECIES_FINIZEN,SPECIES_VAROOM,SPECIES_GLIMMET,SPECIES_GREAVARD,SPECIES_CETODDLE,SPECIES_FRIGIBAX,SPECIES_GIMMIGHOUL,SPECIES_POLTCHAGEIST};
            GenerateOptions(speciesListSplit1, 324);
        }
    }
}




//==========FUNCTIONS==========//
// UI loader template functions by Ghoulslash
void Task_OpenBirchCase(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        CleanupOverworldWindowsAndTilemaps();
        BirchCase_Init(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        DestroyTask(taskId);
    }
}

// This is our main initialization function if you want to call the menu from elsewhere
void BirchCase_Init(MainCallback callback)
{
    GenerateBox();
    u16 i = 0;
    if ((sBirchCaseDataPtr = AllocZeroed(sizeof(struct MenuResources))) == NULL)
    {
        SetMainCallback2(callback);
        return;
    }

    // initialize stuff
    sBirchCaseDataPtr->gfxLoadState = 0;
    sBirchCaseDataPtr->savedCallback = callback;

    sBirchCaseDataPtr->handSpriteId = SPRITE_NONE;

    for(i=0; i < 9; i++)
    {
        sBirchCaseDataPtr->pokeballSpriteIds[i] = SPRITE_NONE;
    }

    SetMainCallback2(BirchCaseRunSetup);
}

static void BirchCaseRunSetup(void)
{
    while (1)
    {
        if (BirchCaseDoGfxSetup() == TRUE)
            break;
    }
}

static void BirchCaseMainCB(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void BirchCaseVBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static bool8 BirchCaseDoGfxSetup(void)
{
    switch (gMain.state)
    {
    case 0:
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000)
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        ResetVramOamAndBgCntRegs();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        ResetSpriteData();
        ResetTasks();
        gMain.state++;
        break;
    case 2:
        if (BirchCase_InitBgs())
        {
            sBirchCaseDataPtr->gfxLoadState = 0;
            gMain.state++;
        }
        else
        {
            BirchCaseFadeAndBail();
            return TRUE;
        }
        break;
    case 3:
        if (BirchCaseLoadGraphics() == TRUE)
            gMain.state++;
        break;
    case 4:
        LoadMessageBoxAndBorderGfx();
        BirchCase_InitWindows();
        gMain.state++;
        break;
    case 5:
        CreatePokeballSprites(); // Create Sprites and Print Text
        CreateHandSprite();
        if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE)
        {
            if(sStarterChoices[sBirchCaseDataPtr->handPosition].isSecret)
            {
                PrintTextToBottomBar(OVERVIEW_SECRET);
            }
            else
            {
                PrintTextToBottomBar(OVERVIEW_MON);

            }
        }
        else
        {
            if(sItemChoices[sBirchCaseDataPtr->handPosition].isSecret)
            {
                PrintTextToBottomBar(OVERVIEW_SECRET);
            }
            else
            {
                PrintTextToBottomBar(OVERVIEW_ITEM);
            }
        }
        CreateTask(Task_BirchCaseWaitFadeIn, 0);
        BlendPalettes(0xFFFFFFFF, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 6:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(BirchCaseVBlankCB);
        SetMainCallback2(BirchCaseMainCB);
        return TRUE;
    }
    return FALSE;
}

#define try_free(ptr) ({        \
    void ** ptr__ = (void **)&(ptr);   \
    if (*ptr__ != NULL)                \
        Free(*ptr__);                  \
})

static void BirchCaseFreeResources(void)
{
    try_free(sBirchCaseDataPtr);
    try_free(sBg1TilemapBuffer);
    try_free(sBg2TilemapBuffer);
    FreeResourcesAndDestroySprite(&gSprites[sBirchCaseDataPtr->monSpriteId], sBirchCaseDataPtr->monSpriteId);
    DestroyPokeballSprites();
    DestroyHandSprite();
    FreeAllWindowBuffers();
}

static void Task_BirchCaseWaitFadeAndBail(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBirchCaseDataPtr->savedCallback);
        BirchCaseFreeResources();
        DestroyTask(taskId);
    }
}

static void BirchCaseFadeAndBail(void)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
    CreateTask(Task_BirchCaseWaitFadeAndBail, 0);
    SetVBlankCallback(BirchCaseVBlankCB);
    SetMainCallback2(BirchCaseMainCB);
}

static void Task_BirchCaseWaitFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_BirchCaseMain;
}

static void Task_BirchCaseTurnOff(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBirchCaseDataPtr->savedCallback);
        BirchCaseFreeResources();
        DestroyTask(taskId);
    }
}

static bool8 BirchCase_InitBgs(void) // Init the bgs and bg tilemap buffers and turn sprites on, also set the bgs to blend
{
    ResetAllBgsCoordinates();
    sBg1TilemapBuffer = Alloc(0x800);
    if (sBg1TilemapBuffer == NULL)
        return FALSE;
    memset(sBg1TilemapBuffer, 0, 0x800);

    sBg2TilemapBuffer = Alloc(0x800);
    if (sBg2TilemapBuffer == NULL)
        return FALSE;
    memset(sBg2TilemapBuffer, 0, 0x800);


    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sMenuBgTemplates, NELEMS(sMenuBgTemplates));
    SetBgTilemapBuffer(1, sBg1TilemapBuffer);
    SetBgTilemapBuffer(2, sBg2TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG2);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(1, 8));
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    return TRUE;
}

static bool8 BirchCaseLoadGraphics(void) // load tilesets, tilemaps, spritesheets, and palettes
{
    switch (sBirchCaseDataPtr->gfxLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sCaseTiles, 0, 0, 0);
        DecompressAndCopyTileDataToVram(2, sTextBgTiles, 0, 0, 0);
        sBirchCaseDataPtr->gfxLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sCaseTilemap, sBg1TilemapBuffer);
            LZDecompressWram(sTextBgTilemap, sBg2TilemapBuffer);
            sBirchCaseDataPtr->gfxLoadState++;
        }
        break;
    case 2:
        LoadCompressedSpriteSheet(&sSpriteSheet_PokeballHand);
        LoadSpritePalette(&sSpritePal_PokeballHand);
        LoadPalette(sCasePalette, 32, 32);
        LoadPalette(sTextBgPalette, 16, 16);
        sBirchCaseDataPtr->gfxLoadState++;
        break;
    default:
        sBirchCaseDataPtr->gfxLoadState = 0;
        return TRUE;
    }
    return FALSE;
}

static void BirchCase_InitWindows(void)
{
    AddBagItem(209,5);
    AddBagItem(102,10);
    InitWindows(sMenuWindowTemplates);
    DeactivateAllTextPrinters();
    ScheduleBgCopyTilemapToVram(0);
    FillWindowPixelBuffer(WINDOW_BOTTOM_BAR, 0);
    LoadUserWindowBorderGfx(WINDOW_BOTTOM_BAR, 720, 14 * 16);
    PutWindowTilemap(WINDOW_BOTTOM_BAR);
    CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);

    ScheduleBgCopyTilemapToVram(2);
}


//
//  Text Printing Function
//
static const u8 sText_ChoosePokemon[] = _("Choose your pokemon!");
static const u8 sText_InspectButton[] = _("{START_BUTTON}INSPECT");
static const u8 sText_RerollButton[] = _("{SELECT_BUTTON}REROLL");
static const u8 sText_RevealAStat[] = _("Reveal a stat! ");
static const u8 sText_RevealsRemaining[] = _("Reveals remaining: ");
static const u8 sText_RevealButtonBar[] = _("{A_BUTTON}REVEAL {B_BUTTON}CANCEL");
static const u8 sText_ItemOverviewTopLeft[] = _("Choose your item!");
static const u8 sText_RerollTopLeft[] = _("Reroll the briefcase?");
static const u8 sText_RerollWarning[] = _("Your D6 will be consumed on use!");
static const u8 sText_RerollButtonBar[] = _("{A_BUTTON}REROLL {B_BUTTON}CANCEL");
static const u8 sText_TakeThe[] = _("Take the ");
static const u8 sText_ConfirmButtonBar[] = _("{A_BUTTON}YES {B_BUTTON}NO");
static const u8 sText_Ability[] = _("Ability");
static const u8 sText_Nature[] = _("Nature");
static const u8 sText_HP[] = _("HP");
static const u8 sText_ATK[] = _("ATK");
static const u8 sText_DEF[] = _("DEF");
static const u8 sText_SPATK[] = _("SPATK");
static const u8 sText_SPDEF[] = _("SPDEF");
static const u8 sText_SPEED[] = _("SPEED");

static void ShowStats() {
    const u8* abilityText = sText_Ability;
    const u8* natureText = sText_Nature;
    const u8* hpText = sText_HP;
    const u8* atkText = sText_ATK;
    const u8* defText = sText_DEF;
    const u8* spatkText = sText_SPATK;
    const u8* spdefText = sText_SPDEF;
    const u8* speedText = sText_SPEED;
    u16 species = sStarterChoices[sBirchCaseDataPtr->handPosition].species;
    u8* ivs = (u8 *) sStarterChoices[sBirchCaseDataPtr->handPosition].ivs;
    u8* natureName = (u8 *) gNaturesInfo[sStarterChoices[sBirchCaseDataPtr->handPosition].nature].name;
    u16 ability = GetAbilityBySpecies(species,sStarterChoices[sBirchCaseDataPtr->handPosition].abilityNum);
    u8* abilityName = (u8 *) gAbilitiesInfo[ability].name;
    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].abilityRevealed == 1)
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 113, 120, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, abilityName);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 113, 120, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, abilityText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].natureRevealed == 1)
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 113, 136, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, natureName);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 113, 136, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, natureText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].hpRevealed == 1)
    {
        ConvertIntToDecimalStringN(gStringVar1, ivs[0], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar1, COMPOUND_STRING(" "));
        StringAppend(gStringVar1, hpText);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 114, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, gStringVar1);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 114, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, hpText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].atkRevealed == 1)
    {
        ConvertIntToDecimalStringN(gStringVar1, ivs[1], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar1, COMPOUND_STRING(" "));
        StringAppend(gStringVar1, atkText);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 128, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, gStringVar1);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 128, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, atkText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].defRevealed == 1)
    {
        ConvertIntToDecimalStringN(gStringVar1, ivs[2], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar1, COMPOUND_STRING(" "));
        StringAppend(gStringVar1, defText);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 142, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, gStringVar1);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 142, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, defText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].spatkRevealed == 1)
    {
        ConvertIntToDecimalStringN(gStringVar1, ivs[4], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar1, COMPOUND_STRING(" "));
        StringAppend(gStringVar1, spatkText);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 60, 114, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, gStringVar1);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 60, 114, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, spatkText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].spdefRevealed == 1)
    {
        ConvertIntToDecimalStringN(gStringVar1, ivs[5], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar1, COMPOUND_STRING(" "));
        StringAppend(gStringVar1, spdefText);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 60, 128, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, gStringVar1);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 60, 128, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, spdefText);
    }

    if(sHiddenInfo[sBirchCaseDataPtr->handPosition].speedRevealed == 1)
    {
        ConvertIntToDecimalStringN(gStringVar1, ivs[3], STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar1, COMPOUND_STRING(" "));
        StringAppend(gStringVar1, speedText);
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 60, 142, 0, 0, sMenuWindowFontColors[FONT_GREEN], 0xFF, gStringVar1);
    }
    else
    {
        AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 60, 142, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, speedText);
    }
}

static void PrintTextToBottomBar(u8 textId)
{
    u8 speciesNameArray[16];
    u8 itemNameArray[16];
    u16 species = sStarterChoices[sBirchCaseDataPtr->handPosition].species;
    u16 item = sItemChoices[sBirchCaseDataPtr->handPosition].item;
    u8 newFontColor;
    FillWindowPixelBuffer(WINDOW_BOTTOM_BAR, PIXEL_FILL(TEXT_COLOR_TRANSPARENT));
    switch(textId)
    {    
        // OVERVIEW_MON2,
        // INSPECT_MON2,
        // OVERVIEW_ITEM2,
        // CONFIRM_REROLL2,
        // CONFIRM_MON2,
        // CONFIRM_ITEM2,
        // NAME_MON2,
        case OVERVIEW_MON:
            StringCopy(gStringVar1, sText_ChoosePokemon);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_InspectButton);
            if(gSaveBlock3Ptr->revealsRemaining>0)
            {
                newFontColor = FONT_GREEN;
            }
            else
            {
                newFontColor = FONT_RED;
            }
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 124, 0, 0, 0, sMenuWindowFontColors[newFontColor], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_RerollButton);
            if(CountTotalItemQuantityInBag(209)>0)
            {
                newFontColor = FONT_GREEN;
            }
            else
            {
                newFontColor = FONT_RED;
            }
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 124+58, 0, 0, 0, sMenuWindowFontColors[newFontColor], 0xFF, gStringVar1);

            StringCopy(&speciesNameArray[0], GetSpeciesName(species)); //mon name
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 96, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, &speciesNameArray[0]);

            ShowStats();
            break;

        case INSPECT_MON:
            StringCopy(gStringVar1, sText_RevealAStat); // Top left
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_RevealsRemaining); // Above stats
            ConvertIntToDecimalStringN(gStringVar2, gSaveBlock3Ptr->revealsRemaining, STR_CONV_MODE_LEFT_ALIGN, 3);
            StringAppend(gStringVar1,gStringVar2);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 96, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_RevealButtonBar); // Button suggestions
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 157, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
            ShowStats();
            break;

        case OVERVIEW_ITEM:
            StringCopy(gStringVar1, sText_ItemOverviewTopLeft);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
            if(item>=582 && item<=681)
            {
                StringCopy(gStringVar1, gItemsInfo[item].name);
                StringAppend(gStringVar1, COMPOUND_STRING(" - "));
                StringAppend(gStringVar1, gMovesInfo[gItemsInfo[item].secondaryId].name);
            }
            else
            {
                StringCopy(gStringVar1, gItemsInfo[item].name);
            }
            StringCopy(&itemNameArray[0], gStringVar1); //item name
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 96, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, &itemNameArray[0]);

            StringCopy(gStringVar2, gItemsInfo[item].descriptionCase); //item description
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 15, 114, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar2);
            break;
        case CONFIRM_REROLL:
            StringCopy(gStringVar1, sText_RerollTopLeft);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_RerollButtonBar);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 157, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_RerollWarning);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 114, 0, 0, sMenuWindowFontColors[FONT_RED], 0xFF, gStringVar1);
            break;
        case CONFIRM_MON:
            StringCopy(gStringVar1, sText_TakeThe);
            StringAppend(gStringVar1, GetSpeciesName(species));
            StringAppend(gStringVar1, COMPOUND_STRING("?"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_ConfirmButtonBar);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 190, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_ConfirmButtonBar);
            ShowStats();
            break;
        case CONFIRM_ITEM:
            StringCopy(gStringVar1, COMPOUND_STRING("Take the item?"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
            StringCopy(gStringVar1, sText_ConfirmButtonBar);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 190, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_ConfirmButtonBar);
            
            if(item>=582 && item<=681)
            {
                StringCopy(gStringVar1, gItemsInfo[item].name);
                StringAppend(gStringVar1, COMPOUND_STRING(" - "));
                StringAppend(gStringVar1, gMovesInfo[gItemsInfo[item].secondaryId].name);
            }
            else
            {
                StringCopy(gStringVar1, gItemsInfo[item].name);
            }
            StringCopy(&itemNameArray[0], gStringVar1); //item name
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 96, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, &itemNameArray[0]);

            StringCopy(gStringVar2, gItemsInfo[item].descriptionCase); //item description
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 15, 114, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar2);
            break;
        case NAME_MON:
            StringCopy(gStringVar1, COMPOUND_STRING("Give the pokemon a name?"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
            StringCopy(gStringVar1, sText_ConfirmButtonBar);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 190, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_ConfirmButtonBar);
            break;
        case OVERVIEW_SECRET:
            StringCopy(gStringVar1, COMPOUND_STRING("Mystery Box"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 96, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar2, COMPOUND_STRING("Rewards include otherwise unobtainable \nitems and trained pokemon."));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 15, 114, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar2);
            break;
        case CONFIRM_SECRET:
            StringCopy(gStringVar1, COMPOUND_STRING("Confirm choice?"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, COMPOUND_STRING("Mystery Box"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 96, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);

            StringCopy(gStringVar1, sText_ConfirmButtonBar);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 190, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_ConfirmButtonBar);
            break;
        case REVEAL_SECRET_MON:
            StringCopy(gStringVar2, COMPOUND_STRING("Received "));
            StringAppend(gStringVar2, GetSpeciesName(species));
            StringAppend(gStringVar2, COMPOUND_STRING("!"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 114, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar2);
            StringCopy(gStringVar1, COMPOUND_STRING("Give the pokemon a name?"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 4, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);
            StringCopy(gStringVar1, sText_ConfirmButtonBar);
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 190, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_ConfirmButtonBar);
            break;
        case REVEAL_SECRET_ITEM:
            StringCopy(gStringVar2, COMPOUND_STRING("Received "));
            if(item>=582 && item<=681)
            {
                StringCopy(gStringVar1, gItemsInfo[item].name);
                StringAppend(gStringVar1, COMPOUND_STRING(" - "));
                StringAppend(gStringVar1, gMovesInfo[gItemsInfo[item].secondaryId].name);
            }
            else
            {
                StringCopy(gStringVar1, gItemsInfo[item].name);
            }
            StringAppend(gStringVar2, gStringVar1); //item name
            StringAppend(gStringVar2, COMPOUND_STRING("!"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_NORMAL, 15, 114, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar2);

            StringCopy(gStringVar1, COMPOUND_STRING("{A_BUTTON}CONFIRM"));
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL_NARROW, 190, 0, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, gStringVar1);


            break;
            
    }

    if(sStarterChoices[sBirchCaseDataPtr->handPosition].species == SPECIES_NONE && sItemChoices[sBirchCaseDataPtr->handPosition].item == ITEM_NONE)
    {
        PutWindowTilemap(WINDOW_BOTTOM_BAR);
        CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);
        return;
    }

    PutWindowTilemap(WINDOW_BOTTOM_BAR);
    CopyWindowToVram(WINDOW_BOTTOM_BAR, 3);
}

//
//  Control Flow Tasks for Switching Positions in the Grid and Confirming and Naming a Mon
//
static void Task_DelayedSpriteLoad(u8 taskId) // wait 4 frames after changing the mon you're editing so there are no palette problems
{   
    if (gTasks[taskId].data[11] >= 4)
    {
        if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE)
        {
            if(!sStarterChoices[sBirchCaseDataPtr->handPosition].isSecret)
            {
                SampleUi_DrawMonIcon(sStarterChoices[sBirchCaseDataPtr->handPosition].species);
            }
        }
        if(sItemChoices[sBirchCaseDataPtr->handPosition].item != ITEM_NONE)
        {
            if(!sItemChoices[sBirchCaseDataPtr->handPosition].isSecret)
            {
                SampleUi_DrawItemIcon(sItemChoices[sBirchCaseDataPtr->handPosition].item);
            }
        }
        gTasks[taskId].func = Task_BirchCaseMain;
        sBirchCaseDataPtr->movingSelector = FALSE;
        return;
    }
    else
    {
        gTasks[taskId].data[11]++;
    }
}

static void Task_WaitForFadeAndOpenNamingScreen(u8 taskId)
{   
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBirchCaseDataPtr->savedCallback);
        BirchCaseFreeResources();
        DestroyTask(taskId);
        VarSet(VAR_0x8004, gPlayerPartyCount - 1);
        if(gSpecialVar_Result == MON_GIVEN_TO_PC)
        {
            ChangeBoxPokemonNickname();
        }
        else
        {
            ChangePokemonNickname();
        }
    }
}

static void Task_BirchCaseRecievedMon(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_WaitForFadeAndOpenNamingScreen;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_BirchCaseTurnOff;
        return;
    }
}

static void Task_BirchCaseConfirmSelectionItem(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        AddBagItem(sItemChoices[sBirchCaseDataPtr->handPosition].item, sItemChoices[sBirchCaseDataPtr->handPosition].count);
        FlagClear(FLAG_BRIEFCASE_OPENED);
        for(int i=0; i<9; i++) {
            gSaveBlock3Ptr->briefcaseItems[i].item = ITEM_NONE;
            gSaveBlock3Ptr->briefcaseMons[i].species = SPECIES_NONE;
            gSaveBlock3Ptr->briefcaseHidden[i].abilityRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].natureRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].hpRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].atkRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].defRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].spatkRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].spdefRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].speedRevealed = 0;
        }
        gSaveBlock3Ptr->revealsRemaining = 0;
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_BirchCaseTurnOff;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(OVERVIEW_ITEM);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
}
static void Task_BirchCaseConfirmSelection(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(NAME_MON);
        BirchCase_GiveMon();
        FlagClear(FLAG_BRIEFCASE_OPENED);
        for(int i=0; i<9; i++) {
            gSaveBlock3Ptr->briefcaseItems[i].item = ITEM_NONE;
            gSaveBlock3Ptr->briefcaseMons[i].species = SPECIES_NONE;
            gSaveBlock3Ptr->briefcaseHidden[i].abilityRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].natureRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].hpRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].atkRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].defRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].spatkRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].spdefRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].speedRevealed = 0;
            sStarterChoices[i].species = SPECIES_NONE;
            sItemChoices[i].item = ITEM_NONE;
        }
        gSaveBlock3Ptr->revealsRemaining = 0;
        gTasks[taskId].func = Task_BirchCaseRecievedMon;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(OVERVIEW_MON);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
}

static void Task_BirchCaseAcknowledgeSecretItem(u8 taskId)
{
    if(IsFanfareTaskInactive()) {
        if(JOY_NEW(A_BUTTON)) {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
            gTasks[taskId].func = Task_BirchCaseTurnOff;
            return;
        }
    }
}

static void Task_BirchCaseConfirmSelectionSecret(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlayFanfare(MUS_OBTAIN_ITEM);
        gSaveBlock3Ptr->revealsRemaining = 0;
        if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE)
        {
            sStarterChoices[sBirchCaseDataPtr->handPosition].isSecret = FALSE;
            Task_DelayedSpriteLoad(taskId);
            PrintTextToBottomBar(REVEAL_SECRET_MON);
            BirchCase_GiveMon();
            gTasks[taskId].func = Task_BirchCaseRecievedMon;
            return;
        }
        if(sItemChoices[sBirchCaseDataPtr->handPosition].item != ITEM_NONE)
        {
            sItemChoices[sBirchCaseDataPtr->handPosition].isSecret = FALSE;
            Task_DelayedSpriteLoad(taskId);
            PrintTextToBottomBar(REVEAL_SECRET_ITEM);
            AddBagItem(sItemChoices[sBirchCaseDataPtr->handPosition].item, sItemChoices[sBirchCaseDataPtr->handPosition].count);
            gTasks[taskId].func = Task_BirchCaseAcknowledgeSecretItem;
            return;
        }
    }

    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(OVERVIEW_SECRET);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
}

static void Task_BirchCaseConfirmReroll(u8 taskId)
{
    if(JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        gSaveBlock3Ptr->revealsRemaining = 0;
        RemoveBagItem(209,1);
        FlagClear(FLAG_BRIEFCASE_OPENED);
        for(int i=0; i<9; i++) {
            gSaveBlock3Ptr->briefcaseItems[i].item = ITEM_NONE;
            gSaveBlock3Ptr->briefcaseMons[i].species = SPECIES_NONE;
            gSaveBlock3Ptr->briefcaseHidden[i].abilityRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].natureRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].hpRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].atkRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].defRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].spatkRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].spdefRevealed = 0;
            gSaveBlock3Ptr->briefcaseHidden[i].speedRevealed = 0;
        }
        gTasks[taskId].func = Task_BirchCaseTurnOff;
        return;
    }
    if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        PrintTextToBottomBar(OVERVIEW_MON);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
}


/* Main Grid Based Movement Control Flow*/
int selection = 0;
static const u8 sText_Selector[] = _("{RIGHT_ARROW}");
static void Task_BirchCaseMain(u8 taskId)
{
    u16 oldPosition = sBirchCaseDataPtr->handPosition;
    if(JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        if(sBirchCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move up
        {
            if(sBirchCaseDataPtr->handPosition < BALL_TOP_THIRD)
                sBirchCaseDataPtr->handPosition = BALL_BOTTOM_FIRST;
            else
                sBirchCaseDataPtr->handPosition = BALL_BOTTOM_SECOND;
        }
        else if(sBirchCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move up
        {
            if(sBirchCaseDataPtr->handPosition == BALL_MIDDLE_FIRST)
                sBirchCaseDataPtr->handPosition = BALL_TOP_FIRST;
            else if (sBirchCaseDataPtr->handPosition == BALL_MIDDLE_SECOND)
                sBirchCaseDataPtr->handPosition = BALL_TOP_SECOND;
            else
                sBirchCaseDataPtr->handPosition = BALL_TOP_THIRD;
        }
        else  // bottom row move up
        {
            sBirchCaseDataPtr->handPosition = BALL_MIDDLE_SECOND;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        if(sBirchCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move down
        {
            if(sBirchCaseDataPtr->handPosition < BALL_TOP_THIRD)
                sBirchCaseDataPtr->handPosition = BALL_MIDDLE_FIRST;
            else if(sBirchCaseDataPtr->handPosition == BALL_TOP_THIRD)
                sBirchCaseDataPtr->handPosition = BALL_MIDDLE_SECOND;
            else
                sBirchCaseDataPtr->handPosition = BALL_MIDDLE_THIRD;
        }
        else if(sBirchCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move down
        {
            if(sBirchCaseDataPtr->handPosition < BALL_MIDDLE_SECOND)
                sBirchCaseDataPtr->handPosition = BALL_BOTTOM_FIRST;
            else
                sBirchCaseDataPtr->handPosition = BALL_BOTTOM_SECOND;
        }
        else  // bottom row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_BOTTOM_FIRST)
                sBirchCaseDataPtr->handPosition = BALL_TOP_SECOND;
            else
                sBirchCaseDataPtr->handPosition = BALL_TOP_THIRD;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(DPAD_RIGHT))
    {
        PlaySE(SE_SELECT);
        if(sBirchCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_TOP_FOURTH) // top row move down
                sBirchCaseDataPtr->handPosition = BALL_TOP_FIRST;
            else
                sBirchCaseDataPtr->handPosition += 1;
        }
        else if(sBirchCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_MIDDLE_THIRD) // top row move down
                sBirchCaseDataPtr->handPosition = BALL_MIDDLE_FIRST;
            else
                sBirchCaseDataPtr->handPosition += 1;
        }
        else  // bottom row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_BOTTOM_SECOND) // top row move down
                sBirchCaseDataPtr->handPosition = BALL_BOTTOM_FIRST;
            else
                sBirchCaseDataPtr->handPosition += 1;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(DPAD_LEFT))
    {
        PlaySE(SE_SELECT);
        if(sBirchCaseDataPtr->handPosition <= BALL_TOP_FOURTH) // top row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_TOP_FIRST) // top row move down
                sBirchCaseDataPtr->handPosition = BALL_TOP_FOURTH;
            else
                sBirchCaseDataPtr->handPosition -= 1;
        }
        else if(sBirchCaseDataPtr->handPosition <= BALL_MIDDLE_THIRD)  // middle row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_MIDDLE_FIRST) // top row move down
                sBirchCaseDataPtr->handPosition = BALL_MIDDLE_THIRD;
            else
                sBirchCaseDataPtr->handPosition -= 1;
        }
        else  // bottom row move down
        {
            if(sBirchCaseDataPtr->handPosition == BALL_BOTTOM_FIRST) // top row move down
                sBirchCaseDataPtr->handPosition = BALL_BOTTOM_SECOND;
            else
                sBirchCaseDataPtr->handPosition -= 1;
        }
        ChangePositionUpdateSpriteAnims(oldPosition, taskId);
        return;
    }
    if(JOY_NEW(A_BUTTON))
    {
        if(sStarterChoices[sBirchCaseDataPtr->handPosition].species != SPECIES_NONE) // If spot empty don't go to next control flow state
        {
            if(sStarterChoices[sBirchCaseDataPtr->handPosition].isSecret)
            {
                PlaySE(SE_SELECT);
                PrintTextToBottomBar(CONFIRM_SECRET);
                gTasks[taskId].func = Task_BirchCaseConfirmSelectionSecret;
                return;
            }
            else
            {
                PlaySE(SE_SELECT);
                PrintTextToBottomBar(CONFIRM_MON);
                gTasks[taskId].func = Task_BirchCaseConfirmSelection;
                return;
            }
        }
        if(sItemChoices[sBirchCaseDataPtr->handPosition].item != ITEM_NONE) 
        {
            if(sItemChoices[sBirchCaseDataPtr->handPosition].isSecret)
            {
                PlaySE(SE_SELECT);
                PrintTextToBottomBar(CONFIRM_SECRET);
                gTasks[taskId].func = Task_BirchCaseConfirmSelectionSecret;
                return;
            }
            else
            {
                PlaySE(SE_SELECT);
                PrintTextToBottomBar(CONFIRM_MON);
                gTasks[taskId].func = Task_BirchCaseConfirmSelectionItem;
                return;
            }
        }
        else 
        {
            PlaySE(SE_BOO);
            return;
        }
    }
    if(JOY_NEW(START_BUTTON))
    {
        int counter = 0;
        for(int j=0; j<8; j++) {
            if(get_stat(sHiddenInfo[sBirchCaseDataPtr->handPosition], j) == 1){
                counter++;
            }
        }
        if(counter==8 || gSaveBlock3Ptr->revealsRemaining==0) {
            PlaySE(SE_BOO);
            gTasks[taskId].func = Task_BirchCaseMain;
            return;
        }
        int i = 0;
        for(i=0; i<8; i++) {
            if(get_stat(sHiddenInfo[sBirchCaseDataPtr->handPosition], i) == 0){
                PlaySE(SE_SELECT);
                selection = i;
                PrintTextToBottomBar(INSPECT_MON);
                if(selection>=0 && selection<=2)
                {
                    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 8, 114 + selection*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);

                }
                else if(selection>=3 && selection<=5)
                {
                    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 53, 114 + (selection-3)*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);

                }
                else if(selection>=6)
                {
                    AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 107, 120 + (selection-6)*16, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);

                }
                gTasks[taskId].func = Task_BirchCaseSelect;
                return;
            }
        }
    }
    if(JOY_NEW(SELECT_BUTTON))
    {
        if(CountTotalItemQuantityInBag(209)==0) {
            PlaySE(SE_BOO);
            gTasks[taskId].func = Task_BirchCaseMain;
            return;
        }
        else
        {
            PlaySE(SE_SELECT);
            PrintTextToBottomBar(CONFIRM_REROLL);
            gTasks[taskId].func = Task_BirchCaseConfirmReroll;
            return;
        }
    }
    if(JOY_NEW(B_BUTTON))
    {
        for(int i=0; i<9; i++) {
            gSaveBlock3Ptr->briefcaseMons[i] = sStarterChoices[i];
            gSaveBlock3Ptr->briefcaseItems[i] = sItemChoices[i];
            gSaveBlock3Ptr->briefcaseHidden[i] = sHiddenInfo[i];
        }
        FlagSet(FLAG_BRIEFCASE_OPENED);
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_BirchCaseTurnOff;
        return;

    }
}


static bool8 isSlotHidden(u8 slot)
{
    return get_stat(sHiddenInfo[sBirchCaseDataPtr->handPosition], slot) == 0;
}

static u8 checkSelectionSequence(u8 first, u8 second, u8 third, u8 fourth, u8 fifth, u8 sixth, u8 seventh)
{
    if(isSlotHidden(first))
    {
        return first;
    }
    else if(isSlotHidden(second))
    {
        return second;
    }
    else if(isSlotHidden(third))
    {
        return third;
    }
    else if(isSlotHidden(fourth))
    {
        return fourth;
    }
    else if(isSlotHidden(fifth))
    {
        return fifth;
    }
    else if(isSlotHidden(sixth))
    {
        return sixth;
    }
    else if(isSlotHidden(seventh))
    {
        return seventh;
    }
    return 8;
}

static void Task_BirchCaseSelect(u8 taskId)
{
    int newSelection;
    if(JOY_NEW(DPAD_UP))
    {
        newSelection = 8;
        switch(selection)
        {
            case 0:
                newSelection = checkSelectionSequence(2, 1, 5, 4, 3, 7, 6);
                break;
            case 1:
                newSelection = checkSelectionSequence(0, 2, 3, 5, 4, 6, 7);
                break;
            case 2:
                newSelection = checkSelectionSequence(1, 0, 4, 3, 5, 6, 7);
                break;
            case 3:
                newSelection = checkSelectionSequence(5, 4, 2, 1, 0, 7, 6);
                break;
            case 4:
                newSelection = checkSelectionSequence(3, 5, 0, 2, 1, 6, 7);
                break;
            case 5:
                newSelection = checkSelectionSequence(4, 3, 1, 0, 7, 2, 6);
                break;
            case 6:
                newSelection = checkSelectionSequence(7, 3, 4, 5, 0, 1, 2);
                break;
            case 7:
                newSelection = checkSelectionSequence(6, 4, 3, 1, 0, 5, 2);
                break;
        }
        PrintTextToBottomBar(INSPECT_MON);
        if(newSelection != 8) {
            PlaySE(SE_SELECT);
            selection = newSelection;
        }
        if(selection>=0 && selection<=2)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 8, 114 + selection*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=3 && selection<=5)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 53, 114 + (selection-3)*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=6 && selection<=7)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 107, 120 + (selection-6)*16, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        gTasks[taskId].func = Task_BirchCaseSelect;
        return;

    }
    if(JOY_NEW(DPAD_DOWN))
    {
        newSelection = 8;
        switch(selection)
        {
            case 0:
                newSelection = checkSelectionSequence(1, 2, 4, 5, 3, 6, 7);
                break;
            case 1:
                newSelection = checkSelectionSequence(2, 0, 5, 4, 3, 7, 6);
                break;
            case 2:
                newSelection = checkSelectionSequence(0, 1, 3, 4, 5, 6, 7);
                break;
            case 3:
                newSelection = checkSelectionSequence(4, 5, 2, 1, 0, 6, 7);
                break;
            case 4:
                newSelection = checkSelectionSequence(5, 3, 2, 7, 1, 0, 6);
                break;
            case 5:
                newSelection = checkSelectionSequence(3, 4, 0, 6, 1, 2, 7);
                break;
            case 6:
                newSelection = checkSelectionSequence(7, 4, 5, 0, 1, 2, 3);
                break;
            case 7:
                newSelection = checkSelectionSequence(6, 5, 3, 4, 0, 1, 2);
                break;
        }
        PrintTextToBottomBar(INSPECT_MON);
        if(newSelection != 8) {
            PlaySE(SE_SELECT);
            selection = newSelection;
        }
        if(selection>=0 && selection<=2)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 8, 114 + selection*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=3 && selection<=5)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 53, 114 + (selection-3)*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=6 && selection<=7)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 107, 120 + (selection-6)*16, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        gTasks[taskId].func = Task_BirchCaseSelect;
        return;


    }
    if(JOY_NEW(DPAD_LEFT))
    {
        newSelection = 8;
        switch(selection)
        {
            case 0:
                newSelection = checkSelectionSequence(6, 7, 3, 4, 5, 1, 2);
                break;
            case 1:
                newSelection = checkSelectionSequence(6, 7, 3, 4, 5, 0, 2);
                break;
            case 2:
                newSelection = checkSelectionSequence(7, 6, 4, 5, 3, 1, 0);
                break;
            case 3:
                newSelection = checkSelectionSequence(0, 1, 2, 6, 7, 4, 5);
                break;
            case 4:
                newSelection = checkSelectionSequence(1, 0, 2, 6, 7, 3, 5);
                break;
            case 5:
                newSelection = checkSelectionSequence(2, 1, 0, 6, 7, 3, 4);
                break;
            case 6:
                newSelection = checkSelectionSequence(3, 4, 5, 0,1,2,7);
                break;
            case 7:
                newSelection = checkSelectionSequence(4, 5, 3, 1, 2, 0, 6);
                break;
        }
        PrintTextToBottomBar(INSPECT_MON);
        if(newSelection != 8) {
            PlaySE(SE_SELECT);
            selection = newSelection;
        }
        if(selection>=0 && selection<=2)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 8, 114 + selection*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=3 && selection<=5)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 53, 114 + (selection-3)*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=6 && selection<=7)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 107, 120 + (selection-6)*16, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        gTasks[taskId].func = Task_BirchCaseSelect;
        return;


    }
    if(JOY_NEW(DPAD_RIGHT))
    {
        newSelection = 8;
        switch(selection)
        {
            case 0:
                newSelection = checkSelectionSequence(3, 4, 6, 5, 7, 1, 2);
                break;
            case 1:
                newSelection = checkSelectionSequence(4, 3, 5, 6, 7, 0, 2);
                break;
            case 2:
                newSelection = checkSelectionSequence(5, 4, 3, 6, 7, 1, 0);
                break;
            case 3:
                newSelection = checkSelectionSequence(6, 7, 0, 1, 2, 4, 5);
                break;
            case 4:
                newSelection = checkSelectionSequence(6, 7, 0, 1, 2, 3, 5);
                break;
            case 5:
                newSelection = checkSelectionSequence(7, 6, 1, 2, 0, 3, 4);
                break;
            case 6:
                newSelection = checkSelectionSequence(0, 1, 2, 3, 4, 5, 7);
                break;
            case 7:
                newSelection = checkSelectionSequence(1, 2, 0, 4, 5, 3, 6);
                break;
        }
        PrintTextToBottomBar(INSPECT_MON);
        if(newSelection != 8) {
            PlaySE(SE_SELECT);
            selection = newSelection;
        }
        if(selection>=0 && selection<=2)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 8, 114 + selection*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=3 && selection<=5)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 53, 114 + (selection-3)*14, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        else if(selection>=6 && selection<=7)
        {
            AddTextPrinterParameterized4(WINDOW_BOTTOM_BAR, FONT_SMALL, 107, 120 + (selection-6)*16, 0, 0, sMenuWindowFontColors[FONT_WHITE], 0xFF, sText_Selector);
        }
        gTasks[taskId].func = Task_BirchCaseSelect;
        return;

    }
    if(JOY_NEW(A_BUTTON))
    {
        switch(selection)
        {
            case 0:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].hpRevealed = 1;
                break;
            case 1:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].atkRevealed = 1;
                break;
            case 2:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].defRevealed = 1;
                break;
            case 3:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].spatkRevealed = 1;
                break;
            case 4:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].spdefRevealed = 1;
                break;
            case 5:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].speedRevealed = 1;
                break;
            case 6:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].abilityRevealed = 1;
                break;
            case 7:
                sHiddenInfo[sBirchCaseDataPtr->handPosition].natureRevealed = 1;
                break;
        }
        gSaveBlock3Ptr->revealsRemaining--;
        PrintTextToBottomBar(OVERVIEW_MON);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
    if(JOY_NEW(B_BUTTON))
    {
        PrintTextToBottomBar(OVERVIEW_MON);
        gTasks[taskId].func = Task_BirchCaseMain;
        return;
    }
}
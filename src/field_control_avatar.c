#include "global.h"
#include "battle_setup.h"
#include "bike.h"
#include "coord_event_weather.h"
#include "daycare.h"
//#include "faraway_island.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "event_scripts.h"
#include "fieldmap.h"
#include "field_control_avatar.h"
#include "field_player_avatar.h"
#include "field_poison.h"
#include "field_screen_effect.h"
#include "field_specials.h"
//#include "fldeff_misc.h"
#include "item_menu.h"
#include "link.h"
//#include "match_call.h"
#include "metatile_behavior.h"
#include "overworld.h"
#include "pokemon.h"
#include "safari_zone.h"
#include "script.h"
//#include "secret_base.h"
#include "sound.h"
#include "start_menu.h"
#include "trainer_see.h"
//#include "trainer_hill.h"
#include "wild_encounter.h"
#include "follow_me.h"
#include "constants/event_bg.h"
#include "constants/event_objects.h"
//#include "constants/field_poison.h"
#include "constants/map_types.h"
#include "constants/songs.h"
//#include "constants/trainer_hill.h"


//      más hooks


static const u8 *GetInteractedObjectEventScript(struct MapPosition *position, u8 metatileBehavior, u8 direction)
{
    u8 objectEventId;
    const u8 *script;
    objectEventId = GetObjectEventIdByPosition(position->x, position->y, position->elevation);
    if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
    {
        if (MetatileBehavior_IsCounter(metatileBehavior) != TRUE)
            return NULL;
        // Look for an object event on the other side of the counter.
        objectEventId = GetObjectEventIdByPosition(position->x + gDirectionToVectors[direction].x, position->y + gDirectionToVectors[direction].y, position->elevation);
        if (objectEventId == OBJECT_EVENTS_COUNT || gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_PLAYER)
            return NULL;
    }
    gSelectedObjectEvent = objectEventId;
    gSpecialVar_LastTalked = gObjectEvents[objectEventId].localId;
    gSpecialVar_Facing = direction;

    if (InTrainerHill() == TRUE)
        script = GetTrainerHillTrainerScript();
    else if (objectEventId == GetFollowerObjectId())//(gObjectEvents[objectEventId].localId == OBJ_EVENT_ID_FOLLOWER)
        script = GetFollowerScriptPointer();
    else
        script = GetObjectEventScriptPointerByObjectEventId(objectEventId);

    script = GetRamScript(gSpecialVar_LastTalked, script);
    return script;
}

static const u8 *GetInteractedWaterScript(struct MapPosition *unused1, u8 metatileBehavior, u8 direction)
{
    //if (FlagGet(FLAG_BADGE05_GET) == TRUE && PartyHasMonWithSurf() == TRUE && IsPlayerFacingSurfableFishableWater() == TRUE)
    if (FlagGet(FLAG_BADGE05_GET) == TRUE && PartyHasMonWithSurf() == TRUE && IsPlayerFacingSurfableFishableWater() == TRUE && CheckFollowerFlag(FOLLOWER_FLAG_CAN_SURF))
        return EventScript_UseSurf;

    //if (MetatileBehavior_IsWaterfall(metatileBehavior) == TRUE)
    if (MetatileBehavior_IsWaterfall(metatileBehavior) == TRUE && CheckFollowerFlag(FOLLOWER_FLAG_CAN_WATERFALL))
    {
        if (FlagGet(FLAG_BADGE08_GET) == TRUE && IsPlayerSurfingNorth() == TRUE)
            return EventScript_UseWaterfall;
        else
            return EventScript_CannotUseWaterfall;
    }
    return NULL;
}

static bool32 TrySetupDiveDownScript(void)
{
    if (!CheckFollowerFlag(FOLLOWER_FLAG_CAN_DIVE))
        return FALSE;

    if (FlagGet(FLAG_BADGE07_GET) && TrySetDiveWarp() == 2)
    {
        ScriptContext1_SetupScript(EventScript_UseDive);
        return TRUE;
    }
    return FALSE;
}

static bool32 TrySetupDiveEmergeScript(void)
{
    if (!CheckFollowerFlag(FOLLOWER_FLAG_CAN_DIVE))
        return FALSE;

    if (FlagGet(FLAG_BADGE07_GET) && gMapHeader.mapType == MAP_TYPE_UNDERWATER && TrySetDiveWarp() == 1)
    {
        ScriptContext1_SetupScript(EventScript_UseDiveUnderwater);
        return TRUE;
    }
    return FALSE;
}


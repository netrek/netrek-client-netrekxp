/******************************************************************************/
/***  File:  feature.c                                                      ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/*** March, 1994.    Joe Rumsey, Tedd Hadley                                ***/
/***                                                                        ***/
/*** most of the functions needed to handle SP_FEATURE/CP_FEATURE packets.  ***/
/*** fill in the features list below for your client,  handleFeature        ***/
/*** should just call checkFeature, which will search the list and set the  ***/
/*** appropriate variable.  features unknown to the server are set to the   ***/
/*** desired value for client features, and off for server/client features. ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    ssheldon - Cleaned up source code, added #include "proto.h"         ***/
/***               and function header comments                             ***/
/******************************************************************************/


/* feature packets look like: */

/* type is CP_FEATURE, which is 60.  feature_spacket is identical. */
#include "config.h"

#ifdef FEATURE_PACKETS

#include <stdio.h>
#include <sys/types.h>

#include "copyright.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "proto.h"

/* not the actual packets: this holds a list of features to report for this
 * client. */
struct feature
{
    char *name;
    int *var;                   /* holds allowed/enabled
                                 * status */
    char feature_type;          /* 'S' or 'C' for
                                 * server/client */
    int value;                  /* desired status */
    char *arg1, *arg2;          /* where to copy args, if
                                 * non-null */
};

static int _dummy;

struct feature features[] = {
/* also sent seperately, put here for checking later. should be ok that it's
 * sent twice. */
    {"FEATURE_PACKETS", &F_feature_packets, 'S', 1, 0, 0},

#ifdef MOTION_MOUSE
    {"CONTINUOUS_MOUSE", &motion_mouse_enablable, 'C', 1, 0, 0},
    {"CONTINUOUS_STEER", &motion_mouse_steering, 'C', 1, 0, 0},
#endif

    {"NEWMACRO", &UseNewMacro, 'C', 1, 0, 0},
    {"SMARTMACRO", &UseSmartMacro, 'C', 1, 0, 0},
    {"WHY_DEAD", &why_dead, 'S', 1, 0, 0},
    {"RC_DISTRESS", &gen_distress, 'S', 1, 0, 0},

#ifdef MULTILINE_MACROS
    {"MULTIMACROS", &multiline_enabled, 'S', 1, 0, 0},
#endif

    {"SBHOURS", &SBhours, 'S', 1, 0, 0},
    {"CLOAK_MAXWARP", &F_cloak_maxwarp, 'S', 1, 0, 0},
    {"SELF_8FLAGS", &F_self_8flags, 'S', 0, 0, 0},
    {"SELF_8FLAGS2", &F_self_8flags2, 'S', 0, 0, 0},
    {"19FLAGS", &F_self_19flags, 'S', 1, 0, 0},
    {"SHIP_CAP", &F_ship_cap, 'S', 1, 0, 0},
    {"SP_GENERIC_32", &F_sp_generic_32, 'S', 1, &A_sp_generic_32, 0},
    {"FULL_DIRECTION_RESOLUTION", &F_full_direction_resolution, 'S', 1, 0, 0},
    {"FULL_WEAPON_RESOLUTION", &F_full_weapon_resolution, 'S', 1, 0, 0},
    {"CHECK_PLANETS", &F_check_planets, 'S', 1, 0, 0},

#ifdef WARP_DEAD
    {"DEAD_WARP", &F_dead_warp, 'S', 1, 0, 0},
#endif

#ifdef BEEPLITE
    {"BEEPLITE", &_dummy, 'C', 1, &F_beeplite_flags, 0},
#endif

#ifdef RECORDGAME
    {"MANY_SELF", &F_many_self, 'S', 0, 0, 0},
#endif

    {"SHOW_ALL_TRACTORS", &F_show_all_tractors, 'S', 1, 0, 0},
    {"SHOW_ARMY_COUNT", &F_show_army_count, 'S', 1, 0, 0},
    {"SHOW_OTHER_SPEED", &F_show_other_speed, 'S', 1, 0, 0},
    {"SHOW_CLOAKERS", &F_show_cloakers, 'S', 1, 0, 0},
    {"TURN_KEYS", &F_turn_keys, 'S', 1, 0, 0},
    {"SHOW_VISIBILITY_RANGE", &F_show_visibility_range, 'S', 1, 0, 0},
    {"SP_FLAGS_ALL", &F_sp_flags_all, 'S', 1, 0, 0},
    {"WHY_DEAD_2", &F_why_dead_2, 'S', 1, 0, 0},
    /* paradise terrain features */
    {"TERRAIN", &F_terrain, 'S', 1, 0, 0},
    /* armies in paradise ship_cap_spacket */
    {"ARMIES_IN_SHIPCAP", &F_armies_shipcap, 'S', 1, 0, 0},
    {"SP_RANKS", &F_sp_ranks, 'S', 1, 0, 0},
    {0, 0, 0, 0, 0, 0}
};

/******************************************************************************/
/***  reportFeatures()                                                      ***/
/******************************************************************************/
static void
reportFeatures (void)
{
    struct feature *f;
    int value;
    char arg1, arg2;

    for (f = features; f->name != 0; f++)
    {
        if (strcmpi (f->name, "FEATURE_PACKETS") != 0)
        {
            value = f->value;
            arg1 = (f->arg1 ? *f->arg1 : 0);
            arg2 = (f->arg2 ? *f->arg2 : 0);
            if (!strcmp(f->name, "CHECK_PLANETS"))
                value = useCheckPlanets;
            else if (!strcmp(f->name, "FULL_DIRECTION_RESOLUTION"))
                value = useFullShipInfo;
            else if (!strcmp(f->name, "FULL_WEAPON_RESOLUTION"))
                value = useFullWeapInfo;
            else if (!strcmp(f->name, "SP_GENERIC_32"))
            {
                value = useGeneric32;
                arg1 = GENERIC_32_VERSION;
            }
            sendFeature (f->name, f->feature_type, value, arg1, arg2);
#ifdef DEBUG
            LineToConsole ("(C->S) %s (%c): %d (%d/%d)\n", f->name,
                           f->feature_type, value, arg1, arg2);
#endif
        }
    }
}

/******************************************************************************/
/***  checkFeature()                                                        ***/
/******************************************************************************/
static void
checkFeature (struct feature_cpacket *packet)
{

    int i;
    int value = (int) ntohl (packet->value);
    char buf[100];

#ifdef DEBUG
    if (packet->type != SP_FEATURE)
    {
        LineToConsole ("Packet type %d sent to checkFeature!\n", packet->type);
        return;
    }
#endif

    sprintf (buf, "%s: %s(%d) (%d/%d)", &packet->name[0],
             ((value == 1) ? "ON" : (value == 0) ? "OFF" : "UNKNOWN"), value,
             packet->arg1, packet->arg2);

#ifdef TOOLS
    W_WriteText (toolsWin, 0, 0, textColor, buf, strlen (buf), W_RegularFont);
#else
    LineToConsole ("%s\n", buf);
#endif

    for (i = 0; features[i].name != 0; i++)
    {
        if (strcmpi (packet->name, features[i].name) == 0)
        {
            /* if server returns unknown, set to off for server mods, desired
             * value for client mods. Otherwise,  set to value from server. */
            *features[i].var = (value == -1 ?
                                (features[i].feature_type ==
                                 'S' ? 0 : features[i].value) : value);
            if (features[i].arg1)
                *features[i].arg1 = packet->arg1;
            if (features[i].arg2)
                *features[i].arg2 = packet->arg2;
            break;
        }
    }
    if (strcmpi (packet->name, "FPS") == 0 && value != -1)
    {
        server_fps = updatesPerSec = server_ups = value;
        LineToConsole("Server capable of computing %d frames per second.\n", value);
        return;
    }
    if (strcmpi(packet->name, "UPS") == 0 && value != -1) {
        lastUpdateSpeed = updatesPerSec = server_ups = value;
#ifdef RECORDGAME
	pbdelay = 100 * 10 / server_ups;
#endif
        LineToConsole("Server actually sending %d updates per second.\n", value);
        return;
    }
    if (strcmpi (packet->name, "LAME_REFIT") == 0)
    {
        lame_refit = value;
        return;
    }
    if (strcmpi (packet->name, "LAME_BASE_REFIT") == 0)
    {
        lame_base_refit = value;
        return;
    }
    if (strcmpi (packet->name, "SP_GENERIC_32") == 0)
    {
        A_sp_generic_32 = packet->arg1;
        return;
    }
    if (features[i].name == 0)
    {
        LineToConsole ("Feature %s from server unknown to client!\n", packet->name);
    }
    /* special cases: */
    if (strcmpi (packet->name, "FEATURE_PACKETS") == 0)
        reportFeatures ();

    if ((strcmpi (packet->name, "RC_DISTRESS") == 0) && gen_distress)
        distmacro = dist_prefered;

#ifdef BEEPLITE
    if ((strcmpi(packet->name, "BEEPLITE") == 0))
    {
        switch (value)
	{
	case -1:				 /* Unknown, we can use all * 
						  * 
						  * * of the features! */

#ifdef STABLE
	    /* Stable release is absolutely non borgish */
	    F_beeplite_flags =
	        LITE_SOUNDS |
	        LITE_TTS;
#else
	    F_beeplite_flags = LITE_PLAYERS_MAP |
	        LITE_PLAYERS_LOCAL |
	        LITE_SELF |
	        LITE_PLANETS |
	        LITE_SOUNDS |
	        LITE_COLOR |
	        LITE_TTS;
#endif

	    break;
	case 1:
	    if (F_beeplite_flags == 0)
	    {					 /* Server says we can have * 
						  * 
						  * * beeplite, but no * *
						  * options??? must be * *
						  * configured wrong. */
	        F_beeplite_flags = LITE_PLAYERS_MAP |
		    LITE_PLAYERS_LOCAL |
		    LITE_SELF |
		    LITE_PLANETS |
		    LITE_SOUNDS |
		    LITE_COLOR |
		    LITE_TTS;
	    }
	    strcpy(buf, "  disabled:");
	    if (!(F_beeplite_flags & LITE_PLAYERS_MAP))
	        strcat(buf, " LITE_PLAYERS_MAP");
	    if (!(F_beeplite_flags & LITE_PLAYERS_LOCAL))
	        strcat(buf, " LITE_PLAYERS_LOCAL");
	    if (!(F_beeplite_flags & LITE_SELF))
	        strcat(buf, " LITE_SELF");
	    if (!(F_beeplite_flags & LITE_PLANETS))
	        strcat(buf, " LITE_PLANETS");
	    if (!(F_beeplite_flags & LITE_SOUNDS))
	        strcat(buf, " LITE_SOUNDS\n");
	    if (!(F_beeplite_flags & LITE_COLOR))
	        strcat(buf, " LITE_COLOR");
	    if (!(F_beeplite_flags & LITE_TTS))
	        strcat(buf, " LITE_TTS");

	    if (strcmp(buf, "  disabled:"))
	    {

#ifdef TOOLS
	        W_WriteText(toolsWin, 0, 0, textColor, buf, strlen(buf), W_RegularFont);
#else
	        printf("%s\n", buf);
#endif
	    }
	    break;
	case 0:
	    F_beeplite_flags = 0;
	    break;
	default:
	    break;
	}
    }
#endif /* BEEPLITE */


#if DEBUG
        /* Ignore these feature packets? */
        motion_mouse_steering = 1;
        F_show_army_count = 1;
        F_show_other_speed = 1;
        F_show_cloakers = 1;
        F_auto_weapons = 1;
        F_turn_keys = 1;
        F_show_visibility_range = 1;
#ifdef BEEPLITE
        F_beeplite_flags = LITE_PLAYERS_MAP |
            LITE_PLAYERS_LOCAL |
            LITE_SELF |
            LITE_PLANETS |
            LITE_SOUNDS |
            LITE_COLOR |
            LITE_TTS;
#endif
#endif
}

/******************************************************************************/
/***  sendFeature()                                                         ***/
/******************************************************************************/
void
sendFeature (char *name,
             char feature_type,
             int value,
             char arg1,
             char arg2)
{
    struct feature_cpacket packet;

    STRNCPY (packet.name, name, sizeof (packet.name));
    packet.type = CP_FEATURE;
    packet.name[sizeof (packet.name) - 1] = 0;
    packet.feature_type = feature_type;
    packet.value = htonl (value);
    packet.arg1 = arg1;
    packet.arg2 = arg2;
    sendServerPacket ((struct player_spacket *) &packet);
}

/******************************************************************************/
/***  handleFeature()                                                       ***/
/******************************************************************************/
void
handleFeature (struct feature_cpacket *packet)
{
    checkFeature (packet);
}

#endif /* FEATURE_PACKETS */

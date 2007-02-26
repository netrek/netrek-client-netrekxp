/******************************************************************************/
/***  File:  sound.c
/***
/***  Function: Sound stuff
/***
/***  Revisions:
/***
/******************************************************************************/

#include "config.h"

#ifdef SOUND
#include "SDL.h"
#include "SDL_mixer.h"

#include "copyright.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "audio.h"
#include "proto.h"

Mix_Chunk *newsounds[NUM_WAVES];
Mix_Music *newmusic[NUM_MUSIC];
/* Each sound has a priority which controls what can override what
   Currently these are set as follows:

   11: death sounds
   10: explosion
   9: phaser,plasma
   8: torp/plasma hit
   7: torp fire, cloak
   6: alert
   4: "informational" sounds (self destruct, enter ship, message, etc.)
   3: shield up/down, other people's sounds (phaser, torp, plasma, explode)
   0: background or ambient (engine?)
*/
static struct Sound sounds[NUM_SOUNDS + 1] = {
    {"", 0, 0},
    {"nt_fire_torp", 7, 1},
    {"nt_phaser", 9, 1},
    {"nt_fire_plasma", 9, 1},
    {"nt_explosion", 10, 1},
    {"nt_base_explosion", 10, 1},
    {"nt_cloaked", 7, 1},
    {"nt_uncloak", 7, 1},
    {"nt_shield_down", 3, 1},
    {"nt_shield_up", 3, 1},
    {"nt_torp_hit", 8, 1},
    {"nt_warning", 5, 1},
    {"nt_red_alert", 5, 1},
    {"nt_engine", 0, 0},
    {"nt_enter_ship", 4, 1},
    {"nt_self_destruct", 6, 1},
    {"nt_plasma_hit", 8, 1},
    {"nt_plasma_kill", 11, 1},
    {"nt_enter_warp", 4, 1},
    {"nt_exit_warp", 4, 1},
    {"nt_start_tmode", 6, 1},
    {"nt_stop_tmode", 6, 1},
    {"nt_message", 4, 1},
    {"nt_message1", 4, 1},
    {"nt_message2", 4, 1},
    {"nt_message3", 4, 1},
    {"nt_message4", 4, 1},
    {"nt_message5", 4, 1},
    {"nt_message6", 4, 1},
    {"nt_message7", 4, 1},
    {"nt_message8", 4, 1},
    {"nt_message9", 4, 1},
    {"nt_fire_torp_other", 3, 1},
    {"nt_phaser_other", 3, 1},
    {"nt_fire_plasma_other", 3, 1},
    {"nt_explosion_other", 10, 1}
};

static int current_sound = NO_SOUND;
static int sound_other = 1;     /* Play other ship's sounds? */

static char sound_prefix[PATH_MAX];

char *DATAFILE(const char* wav) {    
    strcpy(sound_prefix, sounddir);
    strcat(sound_prefix, "/");
    strcat(sound_prefix, wav);
    return sound_prefix;
} 

/*
 * Load the .wave files into the sounds array
 */
int loadSounds(void) {
  int i;

  newsounds[CLOAKED_WAV] = Mix_LoadWAV(DATAFILE("nt_cloaked.wav"));
  newsounds[ENGINE_WAV] = Mix_LoadWAV(DATAFILE("nt_engine.wav"));
  newsounds[ENTER_SHIP_WAV] = Mix_LoadWAV(DATAFILE("nt_enter_ship.wav"));
  newsounds[ENTER_WARP_WAV] = Mix_LoadWAV(DATAFILE("nt_enter_warp.wav"));
  newsounds[EXIT_WARP_WAV] = Mix_LoadWAV(DATAFILE("nt_exit_warp.wav"));
  newsounds[EXPLOSION_WAV] = Mix_LoadWAV(DATAFILE("nt_explosion.wav"));
  newsounds[BASE_EXPLOSION_WAV] = Mix_LoadWAV(DATAFILE("nt_base_explosion.wav"));
  newsounds[EXPLOSION_OTHER_WAV] = Mix_LoadWAV(DATAFILE("nt_explosion_other.wav"));
  newsounds[FIRE_PLASMA_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_plasma.wav"));
  newsounds[OTHER_FIRE_PLASMA_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_plasma_other.wav"));
  newsounds[FIRE_TORP_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_torp.wav"));
  newsounds[OTHER_FIRE_TORP_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_torp_other.wav"));
  newsounds[INTRO_WAV] = Mix_LoadWAV(DATAFILE("nt_intro.wav"));
  newsounds[MESSAGE_WAV] = Mix_LoadWAV(DATAFILE("nt_message.wav"));
  newsounds[MESSAGE1_WAV] = Mix_LoadWAV(DATAFILE("nt_message1.wav"));
  newsounds[MESSAGE2_WAV] = Mix_LoadWAV(DATAFILE("nt_message2.wav"));
  newsounds[MESSAGE3_WAV] = Mix_LoadWAV(DATAFILE("nt_message3.wav"));
  newsounds[MESSAGE4_WAV] = Mix_LoadWAV(DATAFILE("nt_message4.wav"));
  newsounds[MESSAGE5_WAV] = Mix_LoadWAV(DATAFILE("nt_message5.wav"));
  newsounds[MESSAGE6_WAV] = Mix_LoadWAV(DATAFILE("nt_message6.wav"));
  newsounds[MESSAGE7_WAV] = Mix_LoadWAV(DATAFILE("nt_message7.wav"));
  newsounds[PHASER_WAV] = Mix_LoadWAV(DATAFILE("nt_phaser.wav"));
  newsounds[PHASER_OTHER_WAV] = Mix_LoadWAV(DATAFILE("nt_phaser_other.wav"));
  newsounds[PLASMA_HIT_WAV] = Mix_LoadWAV(DATAFILE("nt_plasma_hit.wav"));
  newsounds[PLASMA_KILL_WAV] = Mix_LoadWAV(DATAFILE("nt_plasma_kill.wav"));
  newsounds[RED_ALERT_WAV] = Mix_LoadWAV(DATAFILE("nt_red_alert.wav"));
  newsounds[SELF_DESTRUCT_WAV] = Mix_LoadWAV(DATAFILE("nt_self_destruct.wav"));
  newsounds[SHIELD_DOWN_WAV] = Mix_LoadWAV(DATAFILE("nt_shield_down.wav"));
  newsounds[SHIELD_UP_WAV] = Mix_LoadWAV(DATAFILE("nt_shield_up.wav"));
  newsounds[START_TMODE_WAV] = Mix_LoadWAV(DATAFILE("nt_start_tmode.wav"));
  newsounds[STOP_TMODE_WAV] = Mix_LoadWAV(DATAFILE("nt_stop_tmode.wav"));
  newsounds[TORP_HIT_WAV] = Mix_LoadWAV(DATAFILE("nt_torp_hit.wav"));
  newsounds[UNCLOAK_WAV] = Mix_LoadWAV(DATAFILE("nt_uncloak.wav"));
  newsounds[WARNING_WAV] = Mix_LoadWAV(DATAFILE("nt_warning.wav"));

  for (i=0; i < NUM_WAVES; i++) {
    if (!newsounds[i]) {
      LineToConsole("Mix_LoadWAV newsound[%d] could not be loaded. Check soundDir in your .netrekrc: %s\n", i, Mix_GetError());
      return(-1);
    }
  }

  return(1);
}

/*
 * Load the .ogg files into the music arrays
 */
int loadMusic(void) {
  int i;

  newmusic[INTRO1_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_TOS.ogg"));
  newmusic[INTRO2_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_TNG.ogg"));
  newmusic[INTRO3_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_VOY.ogg"));
  newmusic[INTRO4_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_DS9.ogg"));
  newmusic[INTRO5_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_ST2.ogg"));
  newmusic[END1_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_TOS.ogg"));
  newmusic[END2_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_TNG.ogg"));
  newmusic[END3_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_VOY.ogg"));
  newmusic[END4_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_DS9.ogg"));

  for (i=0; i < NUM_MUSIC; i++) {
    if (!newmusic[i]) {
      LineToConsole("Mix_LoadMUS newmusic[%d] could not be loaded. Check soundDir in your .netrekrc: %s\n", i, Mix_GetError());
      return(-1);
    }
  }

  return(1);
}

extern void sound_cleanup (void)
{
    int i;
    
    //Free the sound effects
    for (i = 0; i < NUM_WAVES; i++)
    	Mix_FreeChunk(newsounds[i]);

    //Free the music
    for (i = 0; i < NUM_MUSIC; i++)
        Mix_FreeMusic(newmusic[i]);
    
    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

extern void Exit_Sound (void)
{
    if (sound_init)
        ExitSound ();
    sound_init = 0;
    sound_toggle = 0;
}


extern void Init_Sound (void)
{
    char buf[PATH_MAX];

#ifdef DEBUG
    LineToConsole ("Init_Sound\n");
#endif
    if (sound_init)
    {
        if ((sounddir = stringDefault ("sounddir")) == NULL)
        {
            if (getenv ("SOUNDDIR") != NULL)
            {
                sounddir = strdup (getenv ("SOUNDDIR"));
            }
            else
                sounddir = "./sounds";
        }
        
        if (newSound)
        {
            int i;

#ifdef DEBUG
	    LineToConsole ("Init_Sound using SDL\n");
#endif

    	    /* Initialize the SDL library */
    	    if (SDL_Init(SDL_INIT_AUDIO) < 0)
      		LineToConsole("Couldn't initialize SDL: %s\n",SDL_GetError());

    	    atexit(sound_cleanup);

    	    /* Open the audio device at 22050 Hz 8 bit Microsoft PCM with stereo */
    	    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024) < 0)
    	    {
      		LineToConsole("Mix_OpenAudio: %s\n", Mix_GetError());
      		sound_init = 0;
      		return;
    	    }

            /* Set sound and music volume to half (128 is MAXVOLUME) */
            Mix_Volume(-1, 64);
            Mix_VolumeMusic(64);

            /* Toggle on sound, and load sound files */
      	    sound_toggle = 1;
      	    loadSounds();
 
      	    /* Load music files, and play random intro music */
      	    loadMusic();
      	    i = RANDOM() % MUSIC_OFFSET;
      	    Play_Music(i);
      	    /* See if volume is adjustable */
      	    CheckVolumeSettings ();
      	    /* Default of 8 channels not enough */
       	    Mix_AllocateChannels(16);
        }
        else
        {
            if (InitSound () == -1)
            {
                sound_toggle = 0;
                sound_init = 0;
            }
            else
            {
                sound_init = 1;
                sound_toggle = 1;
                /* See if volume is adjustable */
                CheckVolumeSettings ();
            }

            strcpy (sound_prefix, sounddir);
            strcat (sound_prefix, "/");

            if (sound_toggle)
            {
                strcpy (buf, sounddir);
                strcat (buf, "/nt_intro");
                StartSound (buf);
            }
        }
    }
}

extern void Play_Sound (int type)
{
    if (newSound)
    {
    	int channel;
    	
        if (!sound_init || !newSoundEffects || !sound_toggle)
	    return;

        if ((type >= NUM_WAVES) || (type < 0))
        {
            LineToConsole("Invalid sound type %d\n", type);
            return;
        }

        if ((channel = Mix_PlayChannel(-1, newsounds[type], 0)) < 0)
        {
#ifdef DEBUG
            /* Frequent occurence is more than 16 channels playing sound */
            LineToConsole("Mix_PlayChannel: %s\n", Mix_GetError());
#endif
            return;
        }
            
        Group_Sound(type, channel);
    }
    else
    {
        char buf[PATH_MAX];

        /*  Don't play other ship's sounds if turned off */
        if (type > OTHER_SOUND_OFFSET && !sound_other)
            return;

        if (sound_toggle && sounds[type].flag &&
        ((sounds[type].priority >= sounds[current_sound].priority) ||
         !SoundPlaying ()))
        {
            STRNCPY (buf, sound_prefix, PATH_MAX);
            strcat (buf, sounds[type].name);
            StartSound (buf);
            current_sound = type;
        }

        if (!(sound_toggle))
            current_sound = NO_SOUND;
     }
}

extern void Play_Sound_Loc (int type, int angle, int distance)
{
    int channel;
    
    if (!sound_init || !newSoundEffects || !sound_toggle)
        return;

    if ((type >= NUM_WAVES) || (type < 0))
    {
        LineToConsole("Invalid sound type %d\n", type);
        return;
    }

    if ((channel = Mix_PlayChannel(-1, newsounds[type], 0)) < 0)
    {
#ifdef DEBUG
        /* Frequent occurence is more than 16 channels playing sound */
        LineToConsole("Mix_PlayChannel: %s\n", Mix_GetError());
#endif
        return;
    }
    /* Make sure distance in boundary range that function accepts */
    if (distance < 0)
    	distance = 0;
    if (distance > 255)
    	distance = 255;
    if (newSoundAngles)
    {
        // Adjust volume with distance and angle
        if (Mix_SetPosition(channel, angle, distance) == 0)
        {
            LineToConsole("Mix_SetPosition: %s\n", Mix_GetError());
            return;
        }
    }
    else
    {
        // Adjust volume with distance
        if (Mix_SetDistance(channel, distance) == 0)
        {
            LineToConsole("Mix_SetDistance: %s\n", Mix_GetError());
            return;
        }
    }
        
    Group_Sound(type, channel);
    return;
}

/* Only works with SDL, i.e. newSound */
extern void Play_Music (int type)
{
    if (!sound_init || !sound_toggle || !newSound || !newSoundMusic)
        return;

    if ((type >= NUM_MUSIC) || (type < 0))
    {
        LineToConsole("Invalid music type %d\n", type);
        return;
    }

    if (Mix_PlayMusic(newmusic[type], 1) < 0)
    {
        LineToConsole("Mix_PlayMusic: %s\n", Mix_GetError());
        return;
    }
}

/* Only works with SDL, i.e. newSound */
extern void Play_Music_Bkgd (void)
{
    int i;

    if (!sound_init || !sound_toggle || !newSound || !newSoundMusic || !newSoundMusicBkgd)
        return;

    if (Mix_PlayingMusic())
        return;

    i = RANDOM() % NUM_MUSIC;

    if (Mix_PlayMusic(newmusic[i], 1) < 0)
    {
        LineToConsole("Mix_PlayMusic: %s\n", Mix_GetError());
        return;
    }
    /* So we play another one when done */
    Mix_HookMusicFinished(Play_Music_Bkgd);
    
}

void Group_Sound (int type, int channel)
{
    // Add channel to group by type, useful for aborting specific sounds
    // at a later time
    // Current designations: 
    // group 1 = cloaked_wav
    // group 2 = warning_wav and red_alert_wav
    // group 3 = enter_warp_wav and exit_warp_wav
    // group 4 = shield_down_wav and shield_up_wav
    switch(type)
    {
    	case CLOAKED_WAV:
            if(!Mix_GroupChannel(channel,1))
                LineToConsole("Mix_GroupChannel: %s\n", Mix_GetError());
            break;
        case WARNING_WAV:
        case RED_ALERT_WAV:
            if(!Mix_GroupChannel(channel,2))
                LineToConsole("Mix_GroupChannel: %s\n", Mix_GetError());
            break;
        case ENTER_WARP_WAV:
        case EXIT_WARP_WAV:
            if(!Mix_GroupChannel(channel,3))
                LineToConsole("Mix_GroupChannel: %s\n", Mix_GetError());
            break;
        case SHIELD_UP_WAV:
        case SHIELD_DOWN_WAV:
            if(!Mix_GroupChannel(channel,4))
                LineToConsole("Mix_GroupChannel: %s\n", Mix_GetError());
            break;
        default: // Reset tag
            if(!Mix_GroupChannel(channel,-1))
                LineToConsole("Mix_GroupChannel: %s\n", Mix_GetError());
            break;
    }
}

/* Only works with SDL, i.e. newSound */
extern void ChangeNewsoundVolume (int vol)
{
    int mvolume, volume;
    
    // Get current average sound volume and music volume
    volume = Mix_Volume(-1, -1);
    mvolume = Mix_VolumeMusic(-1);

    // Change sound volume for all channels, range is 0 to 128
    volume = volume + 10*vol;
    if (volume < 0)
        volume = 0;
    Mix_Volume(-1,volume);

    // Change music volume, range is 0 to 128
    mvolume = mvolume + 10*vol;
    if (mvolume < 0)
        mvolume = 0;
    Mix_VolumeMusic(mvolume);
}

extern void Abort_Sound (int type)
{
    if ((current_sound != NO_SOUND) && (type == current_sound))
        StopSound ();
}


/* Sound options window stuff */

#define SOUND_TOGGLE  0
#define SOUND_OTHER   MESSAGE_SOUND + 1
#define SOUND_INIT    MESSAGE_SOUND + 2
#define SOUND_DONE    MESSAGE_SOUND + 3

#define SDL_EFFECTS_TOGGLE 1
#define SDL_MUSIC_TOGGLE 2
#define SDL_MUSIC_BKGD 3
#define SDL_SOUND_ANGLE 4
#define SDL_SOUND_DONE 5

static void soundrefresh (int i);
static void sdlsoundrefresh (int i);

extern void soundwindow (void)
{
    int i;
    if (newSound)
    {
        for (i = 0; i <= SDL_SOUND_DONE; i++)
            sdlsoundrefresh (i);
    }
    else
    {
        for (i = 0; i <= SOUND_DONE; i++)
            soundrefresh (i);
    }
    /* Map window */
    W_MapWindow (soundWin);
}


static void sdlsoundrefresh (int i)
{
    char buf[BUFSIZ];

    if (i == SOUND_TOGGLE)
    {
        sprintf (buf, "Sound is turned %s",
                 (sound_toggle == 1) ? "ON" : "OFF");
    }
    else if (i == SDL_EFFECTS_TOGGLE)
    {
        sprintf (buf, "Sound effects are turned %s",
                 (newSoundEffects == 1) ? "ON" : "OFF");
    }
    else if (i == SDL_MUSIC_TOGGLE)
    {
        sprintf (buf, "Music is turned %s",
                 (newSoundMusic == 1) ? "ON" : "OFF");
    }
    else if (i == SDL_MUSIC_BKGD)
    {
        sprintf (buf, "Background music is turned %s",
                 (newSoundMusicBkgd == 1) ? "ON" : "OFF");
    }
    else if (i == SDL_SOUND_ANGLE)
    {
        sprintf (buf, "Angular sound is turned %s",
                 (newSoundAngles) ? "ON" : "OFF");
    }
    else if (i == SDL_SOUND_DONE)
    {
        strcpy (buf, "Done");
    }
    else
    {
        LineToConsole ("Uh oh, bogus refresh number in sdlsoundrefresh\n");
    }

    W_WriteText (soundWin, 0, i, textColor, buf, strlen (buf), 0);
}

static void soundrefresh (int i)
{
    char buf[BUFSIZ], *flag;

    if (i == SOUND_TOGGLE)
    {
        sprintf (buf, "Sound is turned %s",
                 (sound_toggle == 1) ? "ON" : "OFF");
    }
    else if (i < SOUND_OTHER)
    {
        flag = ((sounds[i].flag == 1) ? "ON" : "OFF");
        switch (i)
        {
        case CLOAK_SOUND:
            sprintf (buf, "Cloak sound is %s", flag);
            break;
        case UNCLOAK_SOUND:
            sprintf (buf, "Uncloak sound is %s", flag);
            break;
        case ENGINE_SOUND:
            sprintf (buf, "Engine sound is not supported");
            /* sprintf (buf, "Engine sound is %s",  flag); */
            break;
        case ENTER_SHIP_SOUND:
            sprintf (buf, "Enter ship sound is %s", flag);
            break;
        case ENTER_WARP_SOUND:
            sprintf (buf, "Enter warp sound is %s", flag);
            break;
        case EXIT_WARP_SOUND:
            sprintf (buf, "Exit warp sound is %s", flag);
            break;
        case EXPLOSION_SOUND:
            sprintf (buf, "Explosion sound is %s", flag);
            break;
        case BASE_EXPLOSION_SOUND:
            sprintf (buf, "Base explosion sound is %s", flag);
            break;
        case FIRE_TORP_SOUND:
            sprintf (buf, "Fire torp sound is %s", flag);
            break;
        case MESSAGE_SOUND:
            sprintf (buf, "Message sound is %s", flag);
            break;
        case PHASER_SOUND:
            sprintf (buf, "Phaser sound is %s", flag);
            break;
        case WARNING_SOUND:
            sprintf (buf, "Warning sound is %s", flag);
            break;
        case RED_ALERT_SOUND:
            sprintf (buf, "Red alert sound is %s", flag);
            break;
        case SHIELD_DOWN_SOUND:
            sprintf (buf, "Shield down sound is %s", flag);
            break;
        case SHIELD_UP_SOUND:
            sprintf (buf, "Shield up sound is %s", flag);
            break;
        case START_TMODE_SOUND:
            sprintf (buf, "Start tmode sound is %s", flag);
            break;
        case STOP_TMODE_SOUND:
            sprintf (buf, "Stop tmode sound is %s", flag);
            break;
        case TORP_HIT_SOUND:
            sprintf (buf, "Torp hit sound is %s", flag);
            break;
        case SELF_DESTRUCT_SOUND:
            sprintf (buf, "Self destruct sound is %s", flag);
            break;
        case FIRE_PLASMA_SOUND:
            sprintf (buf, "Fire plasma sound is %s", flag);
            break;
        case PLASMA_HIT_SOUND:
            sprintf (buf, "Plasma hit sound is %s", flag);
            break;
        case PLASMA_KILL_SOUND:
            sprintf (buf, "Plasma kill sound is %s", flag);
            break;
        }
    }
    else if (i == SOUND_OTHER)
    {
        if (sound_other)
            strcpy (buf, "Other ship's sound is ON");
        else
            strcpy (buf, "Other ship's sound is OFF");
    }
    else if (i == SOUND_INIT)
    {
        if (sound_init)
            strcpy (buf, "Restart external sound player");
        else
            strcpy (buf, "Initialize external sound player");
    }
    else if (i == SOUND_DONE)
    {
        strcpy (buf, "Done");
    }
    else
    {
        LineToConsole ("Uh oh, bogus refresh number in soundrefresh\n");
    }

    W_WriteText (soundWin, 0, i, textColor, buf, strlen (buf), 0);
}

void sdlsoundaction (W_Event * data)
{
    int i;

    i = data->y;

    if (i == SOUND_TOGGLE)
    {
        if (sound_init)
            sound_toggle = (sound_toggle == 1) ? 0 : 1;
        // Halt all sounds if toggled off
        if (!sound_toggle)
        {
            Mix_HaltChannel(-1);
            Mix_HaltMusic();
        }
        sdlsoundrefresh (SOUND_TOGGLE);
    }
    else if (i == SDL_EFFECTS_TOGGLE)
    {
    	if (sound_init)
            newSoundEffects = (newSoundEffects) ? 0 : 1;
        if (!newSoundEffects)
            Mix_HaltChannel(-1);
    	sdlsoundrefresh (SDL_EFFECTS_TOGGLE);
    }
    else if (i == SDL_MUSIC_TOGGLE)
    {
    	if (sound_init)
            newSoundMusic = (newSoundMusic) ? 0 : 1;
        if (!newSoundMusic)
            Mix_HaltMusic();
    	sdlsoundrefresh (SDL_MUSIC_TOGGLE);
    }
    else if (i == SDL_MUSIC_BKGD)
    {
    	if (sound_init)
    	    newSoundMusicBkgd = (newSoundMusicBkgd) ? 0 : 1;
    	if (!newSoundMusicBkgd)
    	    Mix_HaltMusic();
    	else
    	    Play_Music_Bkgd();
    	sdlsoundrefresh (SDL_MUSIC_BKGD);
    }
    else if (i == SDL_SOUND_ANGLE)
    {
    	newSoundAngles = (newSoundAngles) ? 0 : 1;
    	sdlsoundrefresh (SDL_SOUND_ANGLE);
    }
    else
    {
        sounddone ();
    }
}


void soundaction (W_Event * data)
{
    int i, j;

    i = data->y;

    if (i == SOUND_TOGGLE)
    {
        if (sound_init)
            sound_toggle = (sound_toggle == 1) ? 0 : 1;
        soundrefresh (SOUND_TOGGLE);
        if (!sound_toggle)
        {
            Abort_Sound (ENGINE_SOUND);
        }
        else
        {
            Play_Sound (ENGINE_SOUND);
        }
    }
    else if (i < SOUND_OTHER)
    {
        sounds[i].flag = (sounds[i].flag == 1) ? 0 : 1;
        if (i + OTHER_SOUND_OFFSET <= NUM_SOUNDS)
            sounds[i + OTHER_SOUND_OFFSET].flag = sounds[i].flag;

        soundrefresh (i);
        Play_Sound (i);
        if (i == MESSAGE_SOUND)
        {
            for (j = MESSAGE1_SOUND; j <= MESSAGE9_SOUND; j++)
                sounds[j].flag = sounds[MESSAGE_SOUND].flag;
        }
        /* case ENGINE: st_engine = (st_engine == 1) ? 0 : 0; soundrefresh
         * (ENGINE); if (st_engine && sound_toggle) { Play_Sound
         * (ENGINE_SOUND); } else { Abort_Sound (ENGINE_SOUND); } break; */

    }
    else if (i == SOUND_OTHER)
    {
        sound_other = !sound_other;
        soundrefresh (SOUND_OTHER);
    }
    else if (i == SOUND_INIT)
    {
        Exit_Sound ();
        sound_init = 1;
        Init_Sound ();
        soundrefresh (SOUND_INIT);
        soundrefresh (SOUND_TOGGLE);
    }
    else
    {
        sounddone ();
    }
}

extern void sounddone (void)
{
    W_UnmapWindow (soundWin);
}

#endif /* SOUND */

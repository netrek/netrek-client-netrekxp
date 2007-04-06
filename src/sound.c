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
#include "proto.h"

static void soundrefresh (int i);
static void Init_sound_flags (void);
static int init_success = 0;

static Mix_Chunk *sounds[NUM_WAVES];
static Mix_Music *music[NUM_MUSIC];

static char sound_prefix[PATH_MAX];

/* For tracking current volume levels, new channels need to be
   told what volume to use */
static int mvolume = 0;
static int volume = 0;

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

  sounds[CLOAKED_WAV] = Mix_LoadWAV(DATAFILE("nt_cloaked.wav"));
  sounds[ENGINE_MELT_WAV] = Mix_LoadWAV(DATAFILE("nt_enginemelt.wav"));
  sounds[ENGINE_OK_WAV] = Mix_LoadWAV(DATAFILE("nt_engineok.wav"));
  sounds[ENTER_SHIP_WAV] = Mix_LoadWAV(DATAFILE("nt_enter_ship.wav"));
  sounds[ENTER_WARP_WAV] = Mix_LoadWAV(DATAFILE("nt_enter_warp.wav"));
  sounds[EXIT_WARP_WAV] = Mix_LoadWAV(DATAFILE("nt_exit_warp.wav"));
  sounds[EXPLOSION_WAV] = Mix_LoadWAV(DATAFILE("nt_explosion.wav"));
  sounds[BASE_EXPLOSION_WAV] = Mix_LoadWAV(DATAFILE("nt_base_explosion.wav"));
  sounds[FIRE_PLASMA_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_plasma.wav"));
  sounds[FIRE_TORP_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_torp.wav"));
  sounds[MESSAGE_WAV] = Mix_LoadWAV(DATAFILE("nt_message.wav"));
  sounds[MESSAGE1_WAV] = Mix_LoadWAV(DATAFILE("nt_message1.wav"));
  sounds[MESSAGE2_WAV] = Mix_LoadWAV(DATAFILE("nt_message2.wav"));
  sounds[MESSAGE3_WAV] = Mix_LoadWAV(DATAFILE("nt_message3.wav"));
  sounds[MESSAGE4_WAV] = Mix_LoadWAV(DATAFILE("nt_message4.wav"));
  sounds[MESSAGE5_WAV] = Mix_LoadWAV(DATAFILE("nt_message5.wav"));
  sounds[MESSAGE6_WAV] = Mix_LoadWAV(DATAFILE("nt_message6.wav"));
  sounds[MESSAGE7_WAV] = Mix_LoadWAV(DATAFILE("nt_message7.wav"));
  sounds[PHASER_WAV] = Mix_LoadWAV(DATAFILE("nt_phaser.wav"));
  sounds[PLASMA_HIT_WAV] = Mix_LoadWAV(DATAFILE("nt_plasma_hit.wav"));
  sounds[PLASMA_KILL_WAV] = Mix_LoadWAV(DATAFILE("nt_plasma_kill.wav"));
  sounds[RED_ALERT_WAV] = Mix_LoadWAV(DATAFILE("nt_red_alert.wav"));
  sounds[SELF_DESTRUCT_WAV] = Mix_LoadWAV(DATAFILE("nt_self_destruct.wav"));
  sounds[SHIELD_DOWN_WAV] = Mix_LoadWAV(DATAFILE("nt_shield_down.wav"));
  sounds[SHIELD_UP_WAV] = Mix_LoadWAV(DATAFILE("nt_shield_up.wav"));
  sounds[START_TMODE_WAV] = Mix_LoadWAV(DATAFILE("nt_start_tmode.wav"));
  sounds[STOP_TMODE_WAV] = Mix_LoadWAV(DATAFILE("nt_stop_tmode.wav"));
  sounds[TORP_HIT_WAV] = Mix_LoadWAV(DATAFILE("nt_torp_hit.wav"));
  sounds[UNCLOAK_WAV] = Mix_LoadWAV(DATAFILE("nt_uncloak.wav"));
  sounds[WARNING_WAV] = Mix_LoadWAV(DATAFILE("nt_warning.wav"));
  sounds[EXPLOSION_OTHER_WAV] = Mix_LoadWAV(DATAFILE("nt_explosion_other.wav"));
  sounds[FIRE_PLASMA_OTHER_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_plasma_other.wav"));
  sounds[FIRE_TORP_OTHER_WAV] = Mix_LoadWAV(DATAFILE("nt_fire_torp_other.wav"));
  sounds[PHASER_OTHER_WAV] = Mix_LoadWAV(DATAFILE("nt_phaser_other.wav"));

  for (i=0; i < NUM_WAVES; i++) {
    if (!sounds[i]) {
      LineToConsole("Mix_LoadWAV sounds[%d] could not be loaded. Check soundDir in your .netrekrc: %s\n", i, Mix_GetError());
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

  music[INTRO1_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_TOS.ogg"));
  music[INTRO2_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_TNG.ogg"));
  music[INTRO3_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_VOY.ogg"));
  music[INTRO4_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_DS9.ogg"));
  music[INTRO5_MUSIC] = Mix_LoadMUS(DATAFILE("intro_theme_ST2.ogg"));
  music[END1_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_TOS.ogg"));
  music[END2_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_TNG.ogg"));
  music[END3_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_VOY.ogg"));
  music[END4_MUSIC] = Mix_LoadMUS(DATAFILE("end_theme_DS9.ogg"));

  for (i=0; i < NUM_MUSIC; i++) {
    if (!music[i]) {
      LineToConsole("Mix_LoadMUS music[%d] could not be loaded. Check soundDir in your .netrekrc: %s\n", i, Mix_GetError());
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
    	Mix_FreeChunk(sounds[i]);

    //Free the music
    for (i = 0; i < NUM_MUSIC; i++)
        Mix_FreeMusic(music[i]);
    
    //Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

extern void Init_Sound (void)
{
    int i;

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

        /* Set sound and music volume */
        mvolume = volume = soundVolume;
        Mix_Volume(-1, volume);
        Mix_VolumeMusic(mvolume);

        /* Set sound flags */
        Init_sound_flags();

        /* Toggle on sound, and load sound files */
      	sound_toggle = 1;
      	loadSounds();
      	
      	/* Keep track of successful sound initialization */
      	init_success = 1;

      	/* Load music files, and play random intro music */
        loadMusic();
        i = RANDOM() % MUSIC_OFFSET;
        Play_Music(i);

      	/* Default of 8 channels not enough */
       	Mix_AllocateChannels(16);
    }
}

void Init_sound_flags(void)
{
    unsigned int i;
    char *tmp;

    /* Turn them all on first */
    sound_flags = SF_EXPLOSIONS|SF_WEAPONS|SF_ALERT|SF_MESSAGE|SF_INFO|SF_CLOAKING|SF_SHIELD|SF_OTHER;

    if ((tmp = stringDefault ("soundExclude")) != NULL)
    {
        for (i = 0; i < strlen (tmp); i++)
        {
            switch (tmp[i])
            {
            case 'E':
            case 'e':
                sound_flags &= ~(SF_EXPLOSIONS);
                break;
            case 'W':
            case 'w':
                sound_flags &= ~(SF_WEAPONS);
                break;
            case 'A':
            case 'a':
                sound_flags &= ~(SF_ALERT);
                break;
            case 'M':
            case 'm':
                sound_flags &= ~(SF_MESSAGE);
                break;
            case 'I':
            case 'i':
                sound_flags &= ~(SF_INFO);
                break;
            case 'C':
            case 'c':
                sound_flags &= ~(SF_CLOAKING);
                break;
            case 'S':
            case 's':
                sound_flags &= ~(SF_SHIELD);
                break;
            case 'O':
            case 'o':
                sound_flags &= ~(SF_OTHER);
                break;
            }
        }
    }
}

extern void Play_Sound (int type, u_int flag)
{
    int channel;

    if (!sound_init || !soundEffects || !sound_toggle)
        return;

    /* Don't play sound types you have turned off */
    if (flag != (sound_flags & flag))
        return;
 
    if ((type >= NUM_WAVES) || (type < 0))
    {
        LineToConsole("Invalid sound type %d\n", type);
        return;
    }

    if ((channel = Mix_PlayChannel(-1, sounds[type], 0)) < 0)
    {
#ifdef DEBUG
        /* Frequent occurence is more than 16 channels playing sound */
        LineToConsole("Mix_PlayChannel: %s\n", Mix_GetError());
#endif
        return;
    }
    /* Now that we know the channel, set the volume */
    Mix_Volume(channel, volume);

    Group_Sound(type, channel);
}

extern void Play_Sound_Loc (int type, u_int flag, int angle, int distance)
{
    int channel;
    
    if (!sound_init || !soundEffects || !sound_toggle)
        return;

    /* Don't play sound types you have turned off */
    if (flag != (sound_flags & flag))
        return;

    if ((type >= NUM_WAVES) || (type < 0))
    {
        LineToConsole("Invalid sound type %d\n", type);
        return;
    }

    if ((channel = Mix_PlayChannel(-1, sounds[type], 0)) < 0)
    {
#ifdef DEBUG
        /* Frequent occurence is more than 16 channels playing sound */
        LineToConsole("Mix_PlayChannel: %s\n", Mix_GetError());
#endif
        return;
    }
    /* Now that we know the channel, set the volume */
    Mix_Volume(channel, volume);

    /* Make sure distance in boundary range that function accepts */
    if (distance < 0)
    	distance = 0;
    if (distance > 255)
    	distance = 255;
    if (angle >= 0)
    {
        // Adjust volume with distance and angle
        if (Mix_SetPosition(channel, (char) angle, (char) distance) == 0)
        {
            LineToConsole("Mix_SetPosition: %s\n", Mix_GetError());
            return;
        }
    }
    else
    {
        // Adjust volume with distance
        if (Mix_SetDistance(channel, (char) distance) == 0)
        {
            LineToConsole("Mix_SetDistance: %s\n", Mix_GetError());
            return;
        }
    }
        
    Group_Sound(type, channel);
    return;
}

extern void Play_Music (int type)
{
    if (!sound_init || !sound_toggle || !soundMusic)
        return;

    if ((type >= NUM_MUSIC) || (type < 0))
    {
        LineToConsole("Invalid music type %d\n", type);
        return;
    }

    if (Mix_PlayMusic(music[type], 1) < 0)
    {
        LineToConsole("Mix_PlayMusic: %s\n", Mix_GetError());
        return;
    }
}

extern void Play_Music_Bkgd (void)
{
    int i;

    if (!sound_init || !sound_toggle || !soundMusic || !soundMusicBkgd)
        return;

    if (Mix_PlayingMusic())
        return;

    i = RANDOM() % NUM_MUSIC;

    if (Mix_PlayMusic(music[i], 1) < 0)
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

extern void ChangeVolume (int vol)
{
    // Get current average sound volume and music volume
    //volume = Mix_Volume(-1, -1);
    //mvolume = Mix_VolumeMusic(-1);

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

/* Sound options window stuff */

#define SOUND_TOGGLE  0
#define EFFECTS_TOGGLE 1
#define MUSIC_TOGGLE 2
#define MUSIC_BKGD 3
#define SOUND_ANGLE 4
#define SOUND_EXPLOSIONS 5
#define SOUND_WEAPONS 6
#define SOUND_ALERT 7
#define SOUND_MESSAGE 8
#define SOUND_INFO 9
#define SOUND_CLOAKING 10
#define SOUND_SHIELD 11
#define SOUND_OTHER 12
#define SOUND_DONE 13

extern void soundwindow (void)
{
    int i;

    for (i = 0; i <= SOUND_DONE; i++)
        soundrefresh (i);

    /* Map window */
    W_MapWindow (soundWin);
}

extern int sound_window_height (void)
{
    return SOUND_DONE + 1;
}

static void soundrefresh (int i)
{
    char buf[BUFSIZ];

    if (i == SOUND_TOGGLE)
    {
        sprintf (buf, "Sound is turned %s",
                 (sound_toggle == 1) ? "ON" : "OFF");
    }
    else if (i == EFFECTS_TOGGLE)
    {
        sprintf (buf, "Sound effects are turned %s",
                 (soundEffects == 1) ? "ON" : "OFF");
    }
    else if (i == MUSIC_TOGGLE)
    {
        sprintf (buf, "Music is turned %s",
                 (soundMusic == 1) ? "ON" : "OFF");
    }
    else if (i == MUSIC_BKGD)
    {
        sprintf (buf, "Background music is turned %s",
                 (soundMusicBkgd == 1) ? "ON" : "OFF");
    }
    else if (i == SOUND_ANGLE)
    {
        sprintf (buf, "Angular sound is turned %s",
                 (soundAngles) ? "ON" : "OFF");
    }
    else if (i == SOUND_EXPLOSIONS)
    {
        sprintf (buf, "Explosion sounds are turned %s",
                 (sound_flags & SF_EXPLOSIONS) ? "ON" : "OFF");
    }
    else if (i == SOUND_WEAPONS)
    {
        sprintf (buf, "Weapon sounds are turned %s",
                 (sound_flags & SF_WEAPONS) ? "ON" : "OFF");
    }
    else if (i == SOUND_ALERT)
    {
        sprintf (buf, "Alert sounds are turned %s",
                 (sound_flags & SF_ALERT) ? "ON" : "OFF");
    }
    else if (i == SOUND_MESSAGE)
    {
        sprintf (buf, "Message sounds are turned %s",
                 (sound_flags & SF_MESSAGE) ? "ON" : "OFF");
    }
    else if (i == SOUND_INFO)
    {
        sprintf (buf, "Info sounds are turned %s",
                 (sound_flags & SF_INFO) ? "ON" : "OFF");
    }
    else if (i == SOUND_CLOAKING)
    {
        sprintf (buf, "Cloaking sounds are turned %s",
                 (sound_flags & SF_CLOAKING) ? "ON" : "OFF");
    }
    else if (i == SOUND_SHIELD)
    {
        sprintf (buf, "Shield sounds are turned %s",
                 (sound_flags & SF_SHIELD) ? "ON" : "OFF");
    }
    else if (i == SOUND_OTHER)
    {
        sprintf (buf, "Other ship's sound is turned %s",
                 (sound_flags & SF_OTHER) ? "ON" : "OFF");
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

void soundaction (W_Event * data)
{
    int i;

    i = data->y;

    if (i == SOUND_TOGGLE)
    {
        if (!init_success)
        {
            sound_init = 1;
            Init_Sound();
        }
        else
            sound_toggle = (sound_toggle == 1) ? 0 : 1;
        // Halt all sounds if toggled off
        if (!sound_toggle)
        {
            Mix_HaltChannel(-1);
            Mix_HaltMusic();
        }
        soundrefresh (SOUND_TOGGLE);
    }
    else if (i == EFFECTS_TOGGLE)
    {
    	if (sound_init)
            soundEffects = (soundEffects) ? 0 : 1;
        if (!soundEffects)
            Mix_HaltChannel(-1);
    	soundrefresh (EFFECTS_TOGGLE);
    }
    else if (i == MUSIC_TOGGLE)
    {
    	if (sound_init)
            soundMusic = (soundMusic) ? 0 : 1;
        if (!soundMusic)
            Mix_HaltMusic();
    	soundrefresh (MUSIC_TOGGLE);
    }
    else if (i == MUSIC_BKGD)
    {
    	if (sound_init)
    	    soundMusicBkgd = (soundMusicBkgd) ? 0 : 1;
    	if (!soundMusicBkgd)
    	    Mix_HaltMusic();
    	else
    	    Play_Music_Bkgd();
    	soundrefresh (MUSIC_BKGD);
    }
    else if (i == SOUND_ANGLE)
    {
    	soundAngles = (soundAngles) ? 0 : 1;
    	soundrefresh (SOUND_ANGLE);
    }
    else if (i == SOUND_EXPLOSIONS)
    {
        sound_flags ^= SF_EXPLOSIONS;
        soundrefresh (SOUND_EXPLOSIONS);
    }
    else if (i == SOUND_WEAPONS)
    {
        sound_flags ^= SF_WEAPONS;
        soundrefresh (SOUND_WEAPONS);
    }
    else if (i == SOUND_ALERT)
    {
        sound_flags ^= SF_ALERT;
        soundrefresh (SOUND_ALERT);
    }
    else if (i == SOUND_MESSAGE)
    {
        sound_flags ^= SF_MESSAGE;
        soundrefresh (SOUND_MESSAGE);
    }
    else if (i == SOUND_INFO)
    {
        sound_flags ^= SF_INFO;
        soundrefresh (SOUND_INFO);
    }
    else if (i == SOUND_CLOAKING)
    {
        sound_flags ^= SF_CLOAKING;
        soundrefresh (SOUND_CLOAKING);
    }
    else if (i == SOUND_SHIELD)
    {
        sound_flags ^= SF_SHIELD;
        soundrefresh (SOUND_SHIELD);
    }
    else if (i == SOUND_OTHER)
    {
        sound_flags ^= SF_OTHER;
        soundrefresh (SOUND_OTHER);
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


/* Sound defines */

#ifndef __SOUND_H
#define __SOUND_H

#define NO_SOUND                0
#define FIRE_TORP_SOUND         1
#define PHASER_SOUND            2
#define FIRE_PLASMA_SOUND       3
#define EXPLOSION_SOUND         4
#define BASE_EXPLOSION_SOUND    5
#define CLOAK_SOUND             6
#define UNCLOAK_SOUND           7
#define SHIELD_DOWN_SOUND       8
#define SHIELD_UP_SOUND         9
#define TORP_HIT_SOUND          10
#define WARNING_SOUND           11
#define RED_ALERT_SOUND         12
#define ENGINE_SOUND            13
#define ENTER_SHIP_SOUND        14
#define SELF_DESTRUCT_SOUND     15
#define PLASMA_HIT_SOUND        16
#define PLASMA_KILL_SOUND       17
#define ENTER_WARP_SOUND        18
#define EXIT_WARP_SOUND         19
#define START_TMODE_SOUND       20
#define STOP_TMODE_SOUND        21
#define MESSAGE_SOUND           22
#define MESSAGE1_SOUND          23
#define MESSAGE2_SOUND          24
#define MESSAGE3_SOUND          25
#define MESSAGE4_SOUND          26
#define MESSAGE5_SOUND          27
#define MESSAGE6_SOUND          28
#define MESSAGE7_SOUND          29
#define MESSAGE8_SOUND          30
#define MESSAGE9_SOUND          31
#define OTHER_SOUND_OFFSET      31

/* Other people's sounds; not all of these are currently used */
#define OTHER_FIRE_TORP_SOUND   32
#define OTHER_PHASER_SOUND      33
#define OTHER_FIRE_PLASMA_SOUND 34
#define OTHER_EXPLOSION_SOUND   35


#define NUM_SOUNDS 35

struct Sound
{
    char *name;
    int priority;
    int flag;
};

extern void sounddone (void);
extern void soundwindow (void);

extern void Play_Sound (int type);
extern void Play_Sound_Loc (int type, int angle, int distance);
extern void Play_Music (int type);
extern void Play_Music_Bkgd (void);
extern void ChangeNewsoundVolume (int vol);
extern void Abort_Sound (int type);
extern void Init_Sound (void);
extern void Exit_Sound (void);

enum {
  CLOAKED_WAV,
  ENGINE_WAV,
  ENTER_SHIP_WAV,
  ENTER_WARP_WAV,
  EXIT_WARP_WAV,
  EXPLOSION_WAV,
  BASE_EXPLOSION_WAV,
  EXPLOSION_OTHER_WAV,
  FIRE_PLASMA_WAV,
  OTHER_FIRE_PLASMA_WAV,
  FIRE_TORP_WAV,
  OTHER_FIRE_TORP_WAV,
  INTRO_WAV,
  MESSAGE_WAV,
  MESSAGE1_WAV,
  MESSAGE2_WAV,
  MESSAGE3_WAV,
  MESSAGE4_WAV,
  MESSAGE5_WAV,
  MESSAGE6_WAV,
  MESSAGE7_WAV,
  PHASER_WAV,
  PHASER_OTHER_WAV,
  PLASMA_HIT_WAV,
  PLASMA_KILL_WAV,
  RED_ALERT_WAV,
  SELF_DESTRUCT_WAV,
  SHIELD_DOWN_WAV,
  SHIELD_UP_WAV,
  START_TMODE_WAV,
  STOP_TMODE_WAV,
  TORP_HIT_WAV,
  UNCLOAK_WAV,
  WARNING_WAV,
  NUM_WAVES
};

enum {
  INTRO1_MUSIC,
  INTRO2_MUSIC,
  INTRO3_MUSIC,
  INTRO4_MUSIC,
  INTRO5_MUSIC,
  END1_MUSIC,
  END2_MUSIC,
  END3_MUSIC,
  END4_MUSIC,
  NUM_MUSIC,
};
#define MUSIC_OFFSET	END1_MUSIC /* Offset for end music */

#endif /* __SOUND_H */

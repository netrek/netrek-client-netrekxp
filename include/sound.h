
/* Sound defines */

#ifndef __SOUND_H
#define __SOUND_H

#define NO_SOUND                0
#define FIRE_TORP_SOUND         1
#define PHASER_SOUND            2
#define FIRE_PLASMA_SOUND       3
#define EXPLOSION_SOUND         4
#define CLOAK_SOUND             5
#define UNCLOAK_SOUND           6
#define SHIELD_DOWN_SOUND       7
#define SHIELD_UP_SOUND         8
#define TORP_HIT_SOUND          9
#define WARNING_SOUND           10
#define RED_ALERT_SOUND         11
#define ENGINE_SOUND            12
#define ENTER_SHIP_SOUND        13
#define SELF_DESTRUCT_SOUND     14
#define PLASMA_HIT_SOUND        15
#define MESSAGE_SOUND           16
#define MESSAGE1_SOUND          17
#define MESSAGE2_SOUND          18
#define MESSAGE3_SOUND          19
#define MESSAGE4_SOUND          20
#define MESSAGE5_SOUND          21
#define MESSAGE6_SOUND          22
#define MESSAGE7_SOUND          23
#define MESSAGE8_SOUND          24
#define MESSAGE9_SOUND          25

#define OTHER_SOUND_OFFSET      26

/* Other people's sounds; not all of these are currently used */
#define OTHER_FIRE_TORP_SOUND   27
#define OTHER_PHASER_SOUND      28
#define OTHER_FIRE_PLASMA_SOUND 29
#define OTHER_EXPLOSION_SOUND   30


#define NUM_SOUNDS 30

struct Sound
{
    char *name;
    int priority;
    int flag;
};

extern void sounddone (void);
extern void soundwindow (void);

/* extern void soundaction (W_Event * data); */

extern void Play_Sound (int type);
extern void Play_Sound_Loc (int type, int angle, int distance);
extern void Abort_Sound (int type);
extern void Init_Sound (void);
extern void Exit_Sound (void);

enum {
  CLOAKED_WAV,
  ENGINE_WAV,
  ENTER_SHIP_WAV,
  EXPLOSION_WAV,
  EXPLOSION_OTHER_WAV,
  FIRE_PLASMA_WAV,
  OTHER_FIRE_PLASMA_WAV,
  FIRE_TORP_WAV,
  OTHER_FIRE_TORP_WAV,
  INTRO_WAV,
  MESSAGE_WAV,
  PHASER_WAV,
  PHASER_OTHER_WAV,
  PLASMA_HIT_WAV,
  RED_ALERT_WAV,
  SELF_DESTRUCT_WAV,
  SHIELD_DOWN_WAV,
  SHIELD_UP_WAV,
  TORP_HIT_WAV,
  UNCLOAK_WAV,
  WARNING_WAV,
  NUM_WAVES
};

#endif /* __SOUND_H */

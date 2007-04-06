
/* Sound defines */

#ifndef __SOUND_H
#define __SOUND_H

extern void sounddone (void);
extern void soundwindow (void);
extern void soundaction (W_Event * data);
extern void Play_Sound (int type, u_int flag);
extern void Play_Sound_Loc (int type, u_int flag, int angle, int distance);
extern void Play_Music (int type);
extern void Play_Music_Bkgd (void);
extern void Group_Sound (int type, int channel);
extern void ChangeVolume (int vol);
extern void Init_Sound (void);
extern int sound_window_height (void);

#define SF_EXPLOSIONS	0x001
#define SF_WEAPONS	0x002
#define SF_ALERT	0x004
#define SF_MESSAGE	0x008
#define SF_INFO 	0x010
#define SF_CLOAKING	0x020
#define SF_SHIELD	0x040
#define SF_OTHER	0x080

enum {
  CLOAKED_WAV,
  ENGINE_MELT_WAV,
  ENGINE_OK_WAV,
  ENTER_SHIP_WAV,
  ENTER_WARP_WAV,
  EXIT_WARP_WAV,
  EXPLOSION_WAV,
  BASE_EXPLOSION_WAV,
  FIRE_PLASMA_WAV,
  FIRE_TORP_WAV,
  MESSAGE_WAV,
  MESSAGE1_WAV,
  MESSAGE2_WAV,
  MESSAGE3_WAV,
  MESSAGE4_WAV,
  MESSAGE5_WAV,
  MESSAGE6_WAV,
  MESSAGE7_WAV,
  PHASER_WAV,
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
  EXPLOSION_OTHER_WAV,
  FIRE_PLASMA_OTHER_WAV,
  FIRE_TORP_OTHER_WAV,
  PHASER_OTHER_WAV,
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

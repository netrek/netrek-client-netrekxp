/* Portable Sound Library
 * 
 * Copyright 1993 by Kurt Siegl <siegl@risc.uni-linz.ac.at> Permission to use,
 * modify, copy and distribute this software without fee is hereby granted as
 * long as this notice is left here. */

#ifndef __SNDLIB_H
#define __SNDLIB_H


/* Background Sound player */

/* Starts the Background Sound player Returns:  0 on succes -1 in case of an
 * error */
extern int InitSound (void);

/* Terminate Sound player */
extern void ExitSound (void);

/* In WINBASE.H (at least in MS VC++) StartSound and StopSound are
   defined, even though they are supposed to be deleted functions.
   Being the lazy sort, I just edited WINBASE.H to remove the redef
   and incorrect def errors. -SAC 96-Jul-06
*/

/* Play a Soundfile */
extern int StartSound (char *name);

/* Stop the currently played sound */
extern void StopSound (void);

/* Is a sound currently playing? */
extern int SoundPlaying ();

#endif /* __SNDLIB_H */

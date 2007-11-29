List for the installer:
1) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.

Things not gonna happen, or server problems:
2) torp_other and plasma_other not working right due to lack of torp fuse info
3) twarping to base docked on base causes the twarp problem, but hard to reproduce
5) Triple mode blanks planet names, if you vote to dogfight or hockey, names are not restored
6) "The 0 0 20 minutes before empire collapses" - with surrenderstart = 1
7) mars - lose nb_robots
8) newbie/pret: monitor vs full_hostname
9) on daemon exit in newbie/pret, kill any hosed bot slots
10) end game bots are stupid, and on timericide don't act right
11) feature packet for rank names and requirements

Things that are sorta fixed, but could use improvement:
1) Border issue - quite complex problem, how to know the borders to obey after the bitmap
is rotated.  Only solution so far is to moderate border refresh with scaled bitmaps.
2) AddFontResourceEx caused problems with win 98 machines.  Commented out, and using
old AddFontResource function.
3) Metaping on a Unknown server is causing unseemly delay due to threading.  I believe
this is fixed.
4) sendto() error: 10049 on metaserver connect to a bad server, believe it's due
to hostname not resolving.  Currently fixed by using mulitple attempts to resolve bad
host names.

Things to do:
1) Get square planet bitmaps so I can turn them into mapped rotating globes.
2) Some sort of click action on player list brings up a menu with settings for that
player's candock, cantranswarp, ban, ignore, etc.
3) Out of bounds message window calcs don't work with taskbar or menu bar, basewin
rect doesn't take these into account.
4) With expanded tactical ..can now see ships at galactic coordinates (-500,-500),
if you fly into the NW corner of the galaxy.  this was never anticipated!
6) Tractor on/off and pressor on/off option, rather than just tractor on, pressor on.
Maybe with enforced state?

Paradise issues:
Motd paging system gutted for sake of simplicity
Autologin with name and password seems to break any motd pics
A bunch of gridline/zoom options should be made into netrekrc options.
The galactic zoom doesn't work
Some of the paradise packet types may need some work
Many many places in code where things are based on MAXSOMETHING whether it be players
torps planets plasmas.  Whereas paradise wants to use dynamic variables.  It's a
mess.  And paradise has nphasers, the entire concept of multiple phasers
per ship is not compliant with current code.
Still missing asteroids and (flashing) warpbeacons.
Info window not working right with paradise.
Need popup box that works, when connecting to wrong server type
CV army capacity is wrong
Login messages are being sent to review window (mapped for kills and phasers), not all!
Seems a commodore char doesn't have the proper rank info..player info is all blank.
Kill messages are being sent to all, not review, why?
Cambot playback is broken.  Packet handler needs updating.

Wish list:
1) Rewrite graphics engine using SDL

Stas's list:
- two column and/or scrollable playerlist.
- add "reset to defaults" to options menu
- add shiftMouse, controlMouse on/off
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
- parse Announcer messages in hockey games


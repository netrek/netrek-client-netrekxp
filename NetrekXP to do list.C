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


Things that are sorta fixed, but could use improvement:
1) Border issue - quite complex problem, how to know the borders to obey after the bitmap
is rotated.  Only solution so far is to moderate border refresh with scaled bitmaps.
2) AddFontResourceEx caused problems with win 98 machines.  Commented out, and using
old AddFontResource function.

Things to do:
1) Get square planet bitmaps so I can turn them into mapped rotating globes.
2) Some sort of click action on player list brings up a menu with settings for that
player's candock, cantranswarp, ban, ignore, etc.
3) Out of bounds message window calcs don't work with taskbar or menu bar, basewin
rect doesn't take these into account.
4) With expanded tactical ..can now see ships at galactic coordinates (-500,-500),
if you fly into the NW corner of the galaxy.  this was never anticipated!
5) Tractor on/off and pressor on/off option, rather than just tractor on, pressor on.
Maybe with enforced state?
6) Remapping RCD keys via dist.* lines in netrekrc.txt does not work.  Distress key is
changed, macro window in game confirms that key has been changed, but keymap is
apparently never updated.  Remapping dist keys via ckeymap does work.  Changing output
of distress message via dist.* line does work.
7) Remove mungscrollbars netrekrc option, causes problems in Windows 7.

Paradise issues:
1) Motd paging system gutted for sake of simplicity
2) Autologin with name and password seems to break any motd pics - race condition
during login sequence.  But this is a paradise server issue, occurs for paradise
client as well.
3) A bunch of gridline/zoom options should be made into netrekrc options.
4) The galactic zoom doesn't work
5) Some of the paradise packet types may need some work
6) Still missing asteroids.
7) Show ratings on full/wide playerlist.
9) Don't think there is gettarget support for astral bodies like nebulae and stars
and black holes.  Might be an issue with macros.
10) add POBSERV and PTQUEUE support

Wish list:
1) Rewrite graphics engine using SDL

Stas's list:
- two column and/or scrollable playerlist.
- add "reset to defaults" to options menu
- add shiftMouse, controlMouse on/off
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
- parse Announcer messages in hockey games

List for the installer:
1) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
2) fix netrekrc append so that it doesn't keep adding new lines to the netrekrc

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

Things to do:
1) Have font in windows adjust as window size increases.
2) Get square planet bitmaps so I can turn them into mapped rotating globes.
3) Metaping on a Unknown server is causing unseemly delay due to threading.  I believe
this is fixed.
4) Some sort of click action on player list brings up a menu with settings for that
player's candock, cantranswarp, ban, ignore, etc.
5) Out of bounds message window calcs don't work with taskbar or menu bar, basewin
rect doesn't take these into account.
6) sendto() error: 10049 on metaserver connect to a bad server, believe it's due
to hostname not resolving.  Currently fixed by using mulitple attempts to resolve bad
host names.
7) minor richtext issue with scrolling - extra line at the end
8) With expanded tactical ..can now see ships at galactic coordinates (-500,-500),
if you fly into the NW corner of the galaxy.  this was never anticipated!
9) if cursor is in window and it receives a message, it scrolls to bottom automatically,
bad if you are scrolling up reading history
10) check if scrolling is working ok with recent fix to setting position
11) things to fix with font size: dashboard, doc window, xtrekrc window,
macro window

Wish list:
1) Rewrite graphics engine using SDL

Stas's list:
- two column and/or scrollable playerlist.
- add "reset to defaults" to options menu
- add shiftMouse, controlMouse on/off
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
- parse Announcer messages in hockey games
- pasting multiline prints \r\n instead of splitting lines

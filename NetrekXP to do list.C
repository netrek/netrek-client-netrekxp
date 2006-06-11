List for the netrekrc/installer:
1) Change recommendation on metaserver from off to on!
2) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
3) Add chance for player to input their default login name - tired of seeing
new-be-nice!!

Things not gonna happen, or server problems:
1) Player list messaging doesn't work if you can't see player (cloaked, too far
away, etc).  Don't think this is fixable.
2) torp_other and plasma_other not working right due to lack of torp fuse info
3) Can't bomb enemy 3rd space planets in your T-mode opponent's space.
4) ATT not listed as valid ship even if it's in sysdef.
5) in orbit.c, pftranswarp strangeness.
6) look into swar/war in repair time, with obs and plr - doesn't seem to be any way
for an obs to know what the war decs are of the person he is observing. Best solution
is to just have server send repair time and planet orbit info.

Things that go wrong when short packets are on:
1) cloaking/shields at warp 0 doesn't update someone else's tactical on what you did if
there is no movement anywhere on map, and observer sound also messes up
2) Observers don't get any geno message at all (head to default which is error message) -
actually, they do get the whydead, but it's somehow after they are sent to quit screen.
When obs quits, their state is K_WINNER ..and they see the new geno bitmap.
3) The new smooth turning only working on self, not obs or others - short packets only
send headings of 16 positions.
4) Others speed only sent to 16..messed up for obs locked onto puck in hockey,
or twarpers
5) More detail on problem with whydead not updating with SP on.  It's not just an observer thing,
for players too, the first update where pstatus changes, whydead is not sent.  Only on 2nd update.
Now for regular players, not a problem, as they get several explosion updates on geno, so on 2nd explosion
frame they get the proper whydead.  But observers just go to quit screen.  So they don't have updated
whydead until next update.

Things that are sorta fixed, but could use improvement:
1) rewrite placement/refresh of new resource bitmaps to work better with a scaled tactical
2) Bug with waraction..was being called even when clicking on border (broke function)
..I added a default return to switch statement, but the underlying bug still remains.
3) Border issue - quite complex problem, how to know the borders to obey after the bitmap
is rotated.  Only solution so far is to ignore borders with scaled bitmaps.
4) AddFontResourceEx caused problems with win 98 machines.  Commented out, and using
old AddFontResource function.
5) planet resource placement and refresh (for new planet bitmaps) will need adjusting if local is scaled

Things to do:
1) Add warning hull.
2) resizeable message windows.  Perhaps memory of window positions if you choose
to save settings.  Text in windows should adjust to new window size.
3) flipping through options menus clears too large a path to the right on the galactic,
noticeable with viewbox disappearing at low speeds , depending on what length of menu you
flip to.  Also, closing hint window has same effect
4) Save options for observer/servernick/servertype need to be saved
5) A lot of annoying pop up windows (like macro window, default controls) should be
closeable by shift right click.
6) Workable windows installer would be nice
8) Is there a cpu leak with double buffering?
9) Use masking for things like player team/letter display on galactic.  Ship draw on tactical
(have it overwrite the planet/wrench/gas/army for example, not just combine colors).
10) Get square planet bitmaps so I can turn them into mapped rotating globes.
11) ships at warp 0 near newplanets blink in and out of galactic - may be fixed
12) problem with message windows, text getting garbled near top, goes away with
flushing with scroll button.  Doesn't show up initially, but do something like use
scroll button, alt-enter, change to review all..and it shows up.
13) review all scrolling messes up text, if bottom of window is below the visible
bottom border of netrek window.  Actually will happen to whatever window is mapped 
down there at bottom of screen

Stas's list:
- color coded playerlist.
- check inl server for behavior (observers and tractor/pressor packets)
! server: ita won't block RCDs (why not ?)
- add usage on metaserver window (well, what would you write there ?)
! after res if there cloaker around he will be seen briefly (speed 0 bug)
- whydead dies when out of range (not confirmed)
- add "reset to defaults" to options menu
- add bitmap themes option (change default bitmap naming and processing)
- add shiftMouse, controlMouse on/off
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
- add copy/paste possibility from message windows (richtext ?)
- add clickable urls from message windows (richtext ?)
- add "mKeyOnlyToStartMessages: on/off" to ignore keys [0-9][a-z][ATFOKRG] in message window when the messageon=0
- scrollable player list
- parse Announcer messages in hockey games
- resize documentation window
- pasting multiline prints \r\n instead of splitting lines

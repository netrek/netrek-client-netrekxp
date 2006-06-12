Netrek Configurator v1.0 - written by Joe Evango

Program notes from Joe 4/30/06-
Code contains some comments, pretty easy to follow the logic.
I am currently using a config directory that contains two netrekrc files, netrek.one and netrek.sep.
This is done for the two messaging window configs.  This should be easy enough to implement into the code
in order to eliminate the extra sub directory and files if someone has time.

This utility writes to the netrekrc file and assumes it is in located the same folder as the utility so if the
netrekrc file does not exist the utility will not run properly and error out.  When it is run it will also save
your old netrekrc file as netrek.sav.

Since Netrek code has always been freely shared in the community I will do the same with this config utility.
Please feel free to help improve the utility and/or include the utility as an add-on if you are working on
a Windows client.  Also feel free to use the logic in this code to create a config utility for another OS.  It
took a while to put the keymap portion of this together but it works well. I only ask that if any modifications
are made you also make your code changes available to the community.  It would also be nice if I was mentioned as
a contributor in your code comments.
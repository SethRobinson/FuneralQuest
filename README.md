## Funeral Quest source code release

Funeral Quest is kind of an attempt to create a web-based "daily turn" BBS style game.  It allows real humans to partake in the cut throat world
of the death care industry by running competing funeral parlors.

I'm primarly releasing this because someone wants to look at the source for his own clone (Hi Richard!), but also so maybe it can live on in its own way.
Feel free to port this to anything.  See license.txt for the license, it's Proton SDK's license, which is basically "do whatever you want" with attribution.

For more info on setting it up, see:  https://www.codedojo.com/?p=2189

If you just want to play, a list of active games is here: https://rtsoft.com/pages/funeral_quest.php

### WARNING:

This is a home-built http server that runs a game, so assume it could be hacked or exploited in some way, so you probably shouldn't run it
publicly on a box with sensitive info on it, just in case.  It's old code from a different time, I mean, it requires Flash on a webbrowser to work.

Changes made for this source release:

* Upgraded projects to work with Visual Studio 2019 (free versions works fine)
* Removed dependency on StringEx class, it no longer works due to CString changes that Microsoft has made
* Removed dependency on some DirectX 7(?) math stuff by removing Vector3 type from the scripting engine, it was leftover from TeenageLawnmower
and totally not needed for FQ.
* Stripped out drm/protection code
* Removed sub projects like FQStress (stress tester), drm server system

### Notes:

- The "Play local" button that uses localhost worked, but only if I ran the server on port 80.  82 didn't work, not sure why
- The multiplayer/communication is 100% http polling based, no special sockets or cors stuff to worry about.  However, not
being https capable freak out browsers.  It should be possible to run it through an https proxy if you really had to though,
I've done this with similar projects.
- Flash/graphics are included so things should work out the box.
- User passwords are not encrypted and visible to the admin running the game
- The sounds/music used in the Flash files are properly licensed only for FQ and are not part of the license.
- The artwork (Flash vector art, etc) is done by Akiko Robinson and IS released under the Proton SDK license, so feel free to use it, but please credit her.
- The Flash source is the .fla files in the dist dir.  The dist/public dir is what the built in webserver allows the public to see and contains the .swf
(the compiled flash) versions
- The .c files in dist/script are written in a scripting language similar to C.  (Same one used in Teenage Lawnmower)  They are directly processed by the server
and not compiled.  (they are not distributed to the player, which only sees the Flash files)
- The scripting system and a few other things require linking with Compressed File Library library (CFL3 written by Jari Komppa, zlib license) so I'm including that as well.
- Including SAPI lib to link against, this is Microsoft's text to speech system, it's probably part of WinSDK10 now but I was too lazy to check
- This uses MFC (Microsoft Foundation Class library) for the GUI, so that's why the GUI source looks weird and insane and why the server isn't
easily portable to linux.  Could be done though...

### How to compile:

- Install Visual Studio 2019. Make sure you've included C++ and the latest Windows 10 SDK install options.
- Open source/fq.sln and compile.  Both Debug or Release should create dist/fq.exe.   Running the file should start the server
- Clicking the "Logon Channel 1" button should allow you to play locally via your browser.  (Chrome worked for me, although you have to enable Flash)

### Credits:

- Seth A. Robinson - Programming, design
- Akiko Robinson - Art
- Greg Smith - Additional design

Funeral Quest is Copyright 2020 Robinson Technologies


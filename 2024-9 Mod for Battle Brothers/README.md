## Introduction
I made this mod right before I started my master's programme. The mod adds female playable characters to the game. Check it out on Nexus Mods here: https://www.nexusmods.com/battlebrothers/mods/771?tab=description
It is admittedly not without bugs, but I would say it works 95% of the time and whenever I play the game I certainly use it. If you look at the comments/bug reports you can see that I made _some_
effort to iron out the bugs with user feedback, but unfortunately I do not have as much free time as I did when I made the mod. I do plan on finishing it off in the next half year though!
## Implementation
Battle Brothers is a game that uses scrips rather than compiled code, and the scripts are stored in zip files within the game data. After the zip files are extracted, most script files will be .cnut files. These seem to be... semi-compiled? My guess is they are optimized for performance to maximum extent possible while still being an exact copy of the source code once they are decompiled (minus comments).

If two files exist in the data directory (even if in different zip files!) with the same name but different extensions, .cnut and .nut, .nut is loaded AFTER .cnut and overwrites .cnut entirely. This means modding is just a matter of creating a modified copy of one or more .nut files and placing them in a zip folder in the data directory. 

My mod is slightly more complicated, as it messes with texture and sound files, but the overall concept is the same.

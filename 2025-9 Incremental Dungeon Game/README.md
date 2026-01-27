# Incremental Dungeon Game
After finishing my graduate dissertation, I started working on a game using the Godot engine and GDScript. This game is 100% finished and playable on itch.io or Steam\* </br>
https://nauphy.itch.io/incremental-dungeon-game </br>
https://partner.steamgames.com/dashboard </br>
\*Game will release on both itch.io and Steam when the store page for the DLC is approved. </br> </br>

This was my first time making a game of publishable quality. As is typical the first time you do anything, it could have gone better, and I learned a lot. 
My original vision for the game was an incremental game with something to set it apart from the zillion Cookie Clicker clones out there. I liked the 
auto-battler system in the games "Your Chronicle", "Idling to Rule the Gods", and "Untitled Space Sim", and I wanted to make that mechanic the core of the
game, such that the primary way to progress is to win combats. </br> </br>

The actual combat system was really quite easy to implement, but designing and balancing the enemies was an enormous time investment. I ended up instead procedurally generating
enemies such that I only had to balance each enemy to conform to one standard, then just make sure that standard scales properly. This approach required a greater variety of enemies and drops
than would have been necessary with custom-designed encounters, but it definitely saved time, and I think the increased variety was a beneficial side effect. </br>
I also had much more difficulty than expected implementing the front end of the game- all the panels, containers, sprites, buttons, etc. This may have been just because I'm no 
graphic designer, but regardless it further reduced the extent of the scope I could work with. </br> </br>

Overall the game took me about 550 hours to create, and I'm quite happy with the result. I wish it had more mechanics to empower your player, but I think the depth of the equipment
system makes up for the lack of mechanics. If I were to do it again I would rely more on singletons and inheritance to avoid duplicating so much code, as at the start of the 
project I wasn't really prepared for how loosely coupled a node-based system needs to be. Still, most of my development time was spent struggling with the front end and testing/balancing,
so that optimisation wouldn't really save much time. 
**SPACE SLIMES
Abdel Ngouloure and Erica Chou**

For our final project of the semester, we made a top down shooter/dungeon crawler named Space Slimes. The main goal of this game is to eliminate all the slimes that are in your space shuttle. However, there is one slime, the queen slime, that cannot be killed by you permanently until you kill all the other slimes. She will spawn in the main room once you clear the middle room. Slimes will also respawn in rooms you visited unless you kill them all (this is because the slimes divide again). 

**CONTROLS:**

UP KEY: UP
LEFT KEY: LEFT
DOWN KEY:DOWN
RIGHT KEY:RIGHT 

SPACE: SHOOT

**OBJECTIVE:**
KILL THE QUEEN SLIME AFTER ELIMINATING ALL THE OTHER SLIMS

**SUCCESS:** 
YOU KILL THE FINAL SLIME IN THE MIDDLE ROOM AFTER CLEARING ALL OTHER ROOMS

**FAIL:**
YOU LOSE ALL THREE LIVES

**BUGS**
-Game will randomly crash due to break points. This is most likely due to the transfer of information in main and the fact that we do not have a copy constructor, delete, or = operator 

  -Take time when entering rooms, it seems like code will crash due to things not loading in fast enough 

  -Tried adding memory allocation deletes to free up space and lessen the amount of memory leaks I will get. Seems to have aliveated the problem a tiny bit, but I do not think this is the complete fix. 
  
  -There might also be problems due to the passing of variables between main and level...probably need a copy constructor but I'm not sure.
  
  **I would really like advice on the above bug if you have the time proffesor :(**



-Player may or may not glitch through walls if they walk right next to them. This is most likely because of how the collision detection works in this game.

  -As long as you dont stand near a wall you probably will not clip, however this bug is also quite random as the player will sometimes clip and sometimes not clip. 

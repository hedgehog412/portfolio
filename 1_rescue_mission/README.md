05/17 Monday morning:
    Who: Philippe
    What: Obstacle Implementation, defining overall structure of abstractions for obstacles and other objects of the game.
    Bugs: No major bug at the time but did not try running anything
    Resources: None

05/17 Tuesday 4:00pm - 7:00pm:
    Who: Victor, Riti
    What: Player Element Implementation, List abstraction, drawing player
    Bugs: Not moving as expected
    Resources: None

05/17 Tuesday 9:00pm - 11:15pm:
    Who: Victor, Riti
    What: Implementing Player Shape
    Bugs: Not Looking as Expected
    Resources: None

05/18 Wednesday 1:00pm - 3:34pm:
    Who: Victor
    Updating Body Abstraction
    Bugs: Memory Leaks, issues with backwards compatability
    Resources: None

 05/17 Wednesday 10:00pm-1:30am:
    Who: Philippe
    What: Built the shape of zappers, and worked on the random component of how they spawn
    Bugs: Findinf a way to create the semi circles at the ends of the zappers by using the rotate function based on (0, 0) and then translating the semi circles through the vec_add function
    Resources: Used the random function from Stack Overflow that we have been using so far to generate a number between a min and a max.

 05/18 Thursday 2:00am-4:00am:
    Who: Philippe
    What: Worked on making the zappers spawn out of the window and creating an element detection function when objects are out of the window on the left based on their x-component to free them and remove them from the scene. Implemented the collision detection with the player.
    Bugs: Problem that the obstacles did not spawn correctly on other computers in the group. The issue was that a vector was malloced but never defined.
    Resources: None

05/19 Thursday 4:00pm - 9:30pm
    Who: Jihyun
    What: Abstraction for elements, editing demo (cleaning up code)
    Bugs: No major bugs
    Resources: None

 05/19 Thursday 10pm-0:00am:
    Who: Philippe
    What: Worked on the rotation of the zappers to not have all of them perpedicular to the floor. Might edit the percentages of each format later but right now they are all 1/4 chances on the position. Also solved some bugs in the demo that prevented it from running. Did not manage to finish and ge the rotation working.
    Bugs: non void functions not returning anything, undefined variable names, unused variables and impicit function declarations.
    Resources: None

05/20 Friday 10:00am-12:00am:
   Who: Victor
   What: Zappers not working due to bugs in the abscraction, Fixed these bugs
   Bugs: Sdl Wrapper render scene was not rendering the correct thing
   Resources: None

 05/22 Sunday 12pm - 2:30pm:
    Who: Riti 
    What: Researched how to implement text. Had a lot of trouble with where to start and how to start and what to do. 
    Resources: Google, https://wiki.libsdl.sorg/

 05/23 Monday 12pm - 1pm:
    Who: Riti 
    What: Researched how to implement text. Tried doing it a lot of different ways, looked at multiple sdl libraries. Went down many rabbit holes looking for somewhere to start. 
    Resources: Google, https://wiki.libsdl.org/

 05/24 Tuesday 7pm - 8pm:
    Who: Riti 
    What: Got help on what libraries to use and the concepts behind the text implementation.  
    Resources: Office hours

 05/24 Tuesday 10pm - 11pm:
    Who: Riti 
    What: Implemented the basic text abstraction with the concepts from office hours.  
    Resources: Office hours, https://wiki.libsdl.org/SDL_Color, https://wiki.libsdl.org/SDL_MapRGB, https://wiki.libsdl.org/SDL_Rect, http://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_32.html, https://wiki.libsdl.org/SDL_RenderCopy, https://fonts.google.com/specimen/Open+Sans#standard-styles .  

05/24 Tuesday 1pm-4:30pm:
    Who: Philippe
    What: Worked on the rotation of zappers when they are placed
    Bugs: still had problems on my end but later reallized it was due to an abstraction created by other members of the group.
    Resources: None

 05/24 Wednesday 4pm - 5pm:
    Who: Riti 
    What: Fixed small bugs in the text abstractions with help from the TAs. 
    Resources: Office hours. 

 05/25 Wednesday 8pm-10pm:
    Who: Jihyun
    What: Created the shape of the laser
    Bugs: No major bugs
    Resources: None

05/24 Thursday 12pm-5pm:
    Who: Philippe
    What: Created the missile shape as well as the abstraction and added the function to the main game on a timer base. Started working on a warning sign that would be a extra if we have time.
    Bugs: the missile would not appear but would be present, found out it was because a helper function was not returning what it was supposed to.
    Resources: None

05/24 Thursday 3pm - 4pm:
    Who: Riti 
    What: Tried to implement text on top of the polygons. Was not able to render text on top of polygons. 
    Resources: None. 

05/24 Thursday 6:00pm - 7:50pm:
    Who: Victor 
    What: Started Getting head around text, attended office hours to ask about bugs. 
    Resources: TA assistance

05/26 Thursday 7:50pm-9:45pm:
    Who: Philippe & Victor
    What: Fixed the missile abstraction as well as some other errors from the initial abstraction. Implemented all the changes to the main game in regards to the timers and the fact that the laser should not happen at the same time as zappers or missiles.
    Bugs: multiple bugs and a lot of debugging.
    Resources: None

 05/26 Thursday 8pm-0:00am:
    Who: Jihyun
    What: Implemented laser operations, started implementing power ups
    Bugs: Problems with positioning laser (random function)
    Resources: TAs help

05/27 Friday 10:00am-1:00pm:
    Who: Philippe
    What: Created the coin shape as well as its implementation in the overall game. Created the random spawn component and added the semi destrutive collision.
    Bugs: player would move to the left even though the elasticity of the semi destructive collision was set to 0.
    Resources: None

 05/27 Friday 11am-4pm:
    Who: Jihyun
    What: Fixed bugs with demo, Implemented power ups
    Bugs: Problems with coin counter
    Resources: None

05/31 Tuesday 10pm-11pm:
    Who: Jihyun
    What: Implemented coin counter through collision handler, Edited powerup activation
    Bugs: None
    Resources: None

06/01 Wednesday 11:00am-2:00pm:
   Who: Philippe
   What: resolved bugs regarding timing, objects not freeing, power ups.
   Resources: None.

06/01 Wednesday 6pm-11pm:
   Who: Philippe and Victor
   What: Transitions between scenes
   Bugs: Accessing empty memory, miscelaneous typos
   Resources: none

06/01 Wednesday 5pm-6pm:
   Who: Riti 
   What: debugging text
   Bugs: text not displaying with background
   Resources: None

06/01 Wednesday 7:30pm-11pm:
   Who: Riti 
   What: implementing text and making sure it works with an abstraction. Worked on the abstraction
   Resources: None

06/01 Wednesday 11pm-00:05am:
   Who: Riti and Victor
   What: implementing text and creating new abstraction of text.c and changin sdl_wrappper accordingly
   Resources: None

06/01 Wednesday 10pm-00:00am:
    Who: Jihyun
    What: Changed state_t and powerups so uses general list and structure
    Bugs: None
    Resources: None

06/02 Thurday 00:05am-3:40am:
   Who: Victor
   What: implementing moving background, finishing start and end screen with text, improving death transition.
   Resources: https://cboard.cprogramming.com/c-programming/38507-double-string-conversion.html used to see how to convert doubles to strings in c

06/02 Thurday 11:00am-2:00pm:
   Who: Philippe
   What: implementing the start screen and final screen through implemtation of text, completed the storytelling EXTRA PRIORITY of the game
   Resources: None.

06/02 Thurday 12:00pm-1:00pm:
   Who: Riti 
   What: Adding more functionality to text abstraction to make it render only for a specific amount of time. Also going through the new_game and looking for bugs. 
   Resources: None. 

06/02 Thursday 1:00pm - 9pm:
    Who: Jihyun
    What: Highscore counter implementation, editing powerups, Updating laser bounds
    Bugs: Problems with fopen and updating highscore
    Resources: Adam

06/02 Thurday 10:00am-1:00pm:
   Who: Victor
   What: Improving coin magnet force
   Resources: None
   Bugs: Coins not behaving as expected

06/02 Thursday 3:40pm-4:55pm:
   Who: Victor, Riti, Philippe 
   What: Fixing small bugs about freeing our objects.
   Resources: None. 

06/02 Thursday 7:40pm-10:30pm:
   Who: Victor, Riti, Philippe
   What: Implementing Blue coin for controlling power ups
   Resources: None
   Bugs: Miscelaneous Typos

06/03 Friday 11:00am-1:00pm:
   Who: Philippe
   What: EXTRA PRIORITY implementing the "get harder" component with the initial speed of the obstacles and coins increasing linearly during the game
   Resources: https://stackoverflow.com/questions/6792930/how-do-i-share-a-global-variable-between-c-files

06/03 Friday 12:00pm - 3:00pm:
   Who: Riti 
   What: Implemented background sound when playing the game
   Resources: google and lazyfoo.net. 

06/03 Friday 12:00pm - 3:00pm:
   Who: Jihyun Philippe Victor
   What: Finishing Touches to Game
   Resources: none

06/03 Friday 3:00pm - 6:00pm:
   Who: Jihyun Philippe Victor Riti
   What: Finishing Touches to Game
   Resources: none
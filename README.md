# lowrezjam2019
LowRezJam2019 entry

Game Flow

New Game - Player starts with 10,000g - enough for 10 x 1 minutes in small boat, basic rod, basic line, 1 depth weight, simple bait

Continue - Load saved game

Credits - Arcade Badgers...

Tackle Shop
-> Boat			- how much time you get on lake
-> Rod			- how far you can cast
-> Line			- tension
-> Weight		- depth
-> Bait			- diff fish have diff tastes
-> Fishtionary

Fishtionary
List of all fish seen, caught and got away.
Also all Rods, Lines, Weight and Bait.
Smallest, Biggest landed of each type.
Cancel goes back to Tackle Shop

Lake
Randomly generates a lake, displays it on screen, player can place boat anywhere to start.
Might be nicer to have pre-generated lakes? a dozen or so, and a "permit" for each one to add to progression?
Permits would then come from the tackle shop
Maybe add piers to this so player doesn't need a boat?

Boat
Player can move boat around ( drives like a car... )
White spot on radar is boat.
In boat mode, action button enters cast mode, cancel enters leave boat
In cast mode, action button casts, cancel button cancels.
In leave boat mode, action leaves boat, cancel goes back to boat.
If time runs out, player charged 100g for each minute overdue.

Cast
In cast mode, action starts swinging, cancel goes back to boat
In swing mode, cancel cancels swinging. Press action for back swing, then again for front swing, goes to line mode

Line
In line mode, action yanks rod, rotate pulls line back in.
Cancel gives option to continue ( action ) or release fish

As fish nibble, the float bobs.
If fish bites, float disappears, player should yank rod.
If player not quick enough, fish eats bait and gets away.
Otherwise, fish gets caught and player needs to reel in without snapping line.
Line snaps if too much tension ( tension = (rodTension * weight) / lineTension ... where rodTension increases as player reels in, and decreases as they stop/give out line .. snap if tension >= 1 )
If player lands fish - gets added to bucket and back to boat screen.

If player leaves Boat mode before time runs out, bucket gets tallied up and 1c for each g of fish.. some fish have modifiers to add/decrease this (TINY * 0.6, SMALL * 0.8, NORMAL = 1.0, BIG * 1.2, MASSIVE * 1.5)

GOAL:
Make money, complete the Fishtionary


Rods: 
Basic - 1sq
Standard - 2sq
Good - 3sq
Excellent - 4sq
Outstanding - 5sq

Immediate Tasks:

[x] Day 1 - Thursday
Create a 64x64 surface, but allow window to be resized.
Copy over the emscripten build.
Start data structures for:
	Fish
	Rod
	Bait
	SaveGame
	Area
	
Also decide on whether actually doing this in C or using GMS2
Remember that glesgae2 has no sound.. though can probably rip that from glesgae1 without too much issue.
And of course, most of glesgae does not exist!

[x] Day 2 - Friday
	Javascript work
[x] Day 3 - Saturday
	Graphics/mockup work
[x] Day 4 - Sunday
	Graphics/mockup work
[x] Day 5 - Monday
	Nope
[x] Day 6 - Tuesday
	Spritesheet implementation
[ ] Day 7 - Wednesday
	States - splash + timers ( plus emscripten fixes ) plus moved lake to own state to begin game flow
[ ] Day 8 - Thursday
[ ] Day 9 - Friday
[ ] Day 10 - Saturday
[ ] Day 11 - Sunday
[ ] Day 12 - Monday
[ ] Day 13 - Tuesday - JS13k kicks off
[ ] Day 14 - Wednesday
[ ] Day 15 - Thursday
[ ] Day 16 - Friday - last submission day

Remaining Tasks:
Audio System
GUI System ( Button, Menu Tree )
Game Flow ( Splash, Menu, Game )
"Fish" database
Equipment database
Load/Save System
Lake module
Boat module
Cast module
Reel module
Shop module
Music
Sound Effects

SWFoteCustom CHANGELOG

This changelog documents changes done per commit to the code base. It will note whether or not a compilation was attempted and whether or not it was successful.

Commit from 4/15/2014
Compile Successful. Ran Successfully. Logged in successfully.

-	Fixed up some tax rate code in shops.c.

-	Finished Placevendor code. VNUMs 32667 - 32767 are reserved for player vendors. Please don't use them.

-	Added a new field to SHOP_DATA to track whether or not a vendor is a player vendor. Fixed all pre-existing area files to conform to this change and changed Load/save shops to load/save this variable.

-	Added an array of ints to CHAR_DATA to track owned vendor vnums.

- 	A room flag dubbed Airless existed in the defined room flags in build.c but was not defined in mud.h. It was listed as flag 11. I added it to mud.h as flag 11.

-	Increased the chances of rolling a forcer. Before the range was -1000, 20 easily making it a very low percentage chance of being a forcer. Now the range is -40, 20.

-	If a forcer rolls 10+, they also start with the meditate skill trained to 1%. 10+ being considered a forcer able to manifest effects without training. Though they still can't learn force skills without a master. Maybe later.

-	Newbies get 30,000 credits now.

-	Max level per ability is now stored on the player object and calculated at creation. The max level function was changed to return this.

-	The Max Level for any skill is calculated using the function calc_max_level in comm.c during character creation. It's based on stats and race.

-	Literally fixed every warning and error that came up when trying to compile clean. (Except IMC. IMC is still throwing errors, don't care enough to fix it. Makefile has IMC disabled.)

-	Fixed a bug where it would read the entire entry for a shop for trading flags when there were only 5. Set MAX_TRADE to 5 which is what is in the files.

Commit from 4/14/2014
Compile not attempted

-	Added Industry Droid Race for an Engineering Droid Race (Idea in part from LOTJ's industry Droids)

-	Added an ability_bonus struct to mud.h for holding ability bonuses per race. These are added to the base ability as bonus levels. (Loosely based on LOTJ)
	
- 	Secondary Skills are no longer chosen. The system assigns them.
	| To facilitate this, see the Companion_Class struct in mud.h
	
-	Added Companion_Class struct to mud.h and defined in const.c

-	Removed redundant Droid race and added a droid race for each ability

-	Droid races start with 200 in their primary skill and 150 in their secondary. All droids have 0 in every other skill except Piloting. They get 25 levels in Piloting so they are not limited in movement. A droid race is very specialized and powerful for a single task, but they're only programmed for that single task. Perhaps later I'll add in the ability to learn other skills or have a skilled slicer reprogram them to different skills. 

-	Went back and standardized all races to get 50 bonus levels. One skill at 25, One at 15 and 2 at 5. Some races get all 50 in a single skill but these are mostly reserved to Droids because of their specialized programming or races which literally have no other way of life.

-	Shops now list the type of the item they're selling next to the reference and before the short description. (Idea from LOTJ)

-	Clans can set Tax Rates on planets. Every transaction on that planet is charged tax and that tax goes into the clan's funds. (Idea from LOTJ)

-	Changed sell dialogs to indicate when tax is being charged and to remove that amount from their quote automatically.

-	Players can purchase deeds to open a vendor at any other vendor using buyvendor. Added hasDeed flag to Char Data and Pfiles. Player Vendors not yet fully implemented.
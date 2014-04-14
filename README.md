SWFoteCustom CHANGELOG

This changelog documents changes done per commit to the code base. It will note whether or not a compilation was attempted and whether or not it was successful.

Commit from 4//2014
Compile not attempted
-	Added Industry Droid Race for an Engineering Droid Race (Idea in part from LOTJ's industry Droids)

-	Added an ability_bonus struct to mud.h for holding ability bonuses per race. These are added to the base ability as bonus levels. (Loosely based on LOTJ)
	| Humans are an exception. They receive no bonuses per ability, but instead they cannot start with less than 150 in their main class and 130 in their secondary. The rest are calculated as being 10 points less than the 40 others start at. In other words, if a human's primary skill is less than 150 after bonuses it will be set to 150. Secondary will be set to 130 if not higher or equal. All others start at 30 and receive stat bonuses as usual.
	| Hapans are considered Humans but different enough to justify their own Race. All other humanoid races for now are rolled into Human.
	
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
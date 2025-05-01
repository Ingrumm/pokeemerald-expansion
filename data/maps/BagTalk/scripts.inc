# 1 "data/maps/BagTalk/scripts.pory"

# 2 "data/maps/BagTalk/scripts.pory"
    BagTalk_MapScripts::
# 3 "data/maps/BagTalk/scripts.pory"
	    .byte 0

BagTalk_EventScript_BagCheck::
# 7 "data/maps/BagTalk/scripts.pory"
	lockall
# 8 "data/maps/BagTalk/scripts.pory"
	callnative StartNewPokeballCaseUI
# 9 "data/maps/BagTalk/scripts.pory"
	waitstate
# 11 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 12 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_ADVENTURE_STARTED
# 13 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 14 "data/maps/BagTalk/scripts.pory"
	special SetUnlockedPokedexFlags
# 15 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
# 16 "data/maps/BagTalk/scripts.pory"
	releaseall
	return


BagTalk_EventScript_RoxanneTest::
# 21 "data/maps/BagTalk/scripts.pory"
	lockall
# 22 "data/maps/BagTalk/scripts.pory"
	showteampreview 265
# 23 "data/maps/BagTalk/scripts.pory"
	releaseall
	return


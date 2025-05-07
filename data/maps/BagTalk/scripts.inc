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
# 10 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 11 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_ADVENTURE_STARTED
# 12 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 13 "data/maps/BagTalk/scripts.pory"
	special SetUnlockedPokedexFlags
# 14 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
# 15 "data/maps/BagTalk/scripts.pory"
	releaseall
	return


BagTalk_EventScript_RoxanneTest::
# 20 "data/maps/BagTalk/scripts.pory"
	lockall
# 21 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 1
# 22 "data/maps/BagTalk/scripts.pory"
	releaseall
	return


BagTalk_EventScript_Split1Done::
# 26 "data/maps/BagTalk/scripts.pory"
	lock
# 27 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_1_DONE
# 28 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 2
# 29 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split1Done_Text_0
# 30 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split2Done::
# 35 "data/maps/BagTalk/scripts.pory"
	lock
# 36 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_2_DONE
# 37 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 3
# 38 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split2Done_Text_0
# 39 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split3Done::
# 44 "data/maps/BagTalk/scripts.pory"
	lock
# 45 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_3_DONE
# 46 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 4
# 47 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split3Done_Text_0
# 48 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split4Done::
# 53 "data/maps/BagTalk/scripts.pory"
	lock
# 54 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_4_DONE
# 55 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 5
# 56 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split4Done_Text_0
# 57 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split5Done::
# 62 "data/maps/BagTalk/scripts.pory"
	lock
# 63 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_5_DONE
# 64 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 6
# 65 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split5Done_Text_0
# 66 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split6Done::
# 71 "data/maps/BagTalk/scripts.pory"
	lock
# 72 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_6_DONE
# 73 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split6Done_Text_0
# 74 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split7Done::
# 79 "data/maps/BagTalk/scripts.pory"
	lock
# 80 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_7_DONE
# 81 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split7Done_Text_0
# 82 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split8Done::
# 87 "data/maps/BagTalk/scripts.pory"
	lock
# 88 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_8_DONE
# 89 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split8Done_Text_0
# 90 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split9Done::
# 95 "data/maps/BagTalk/scripts.pory"
	lock
# 96 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_9_DONE
# 97 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split9Done_Text_0
# 98 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split10Done::
# 103 "data/maps/BagTalk/scripts.pory"
	lock
# 104 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_10_DONE
# 105 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split10Done_Text_0
# 106 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split11Done::
# 111 "data/maps/BagTalk/scripts.pory"
	lock
# 112 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_11_DONE
# 113 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split11Done_Text_0
# 114 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split12Done::
# 119 "data/maps/BagTalk/scripts.pory"
	lock
# 120 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_12_DONE
# 121 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split12Done_Text_0
# 122 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split13Done::
# 127 "data/maps/BagTalk/scripts.pory"
	lock
# 128 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_13_DONE
# 129 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split13Done_Text_0
# 130 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split14Done::
# 135 "data/maps/BagTalk/scripts.pory"
	lock
# 136 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_14_DONE
# 137 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split14Done_Text_0
# 138 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split15Done::
# 143 "data/maps/BagTalk/scripts.pory"
	lock
# 144 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_15_DONE
# 145 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split15Done_Text_0
# 146 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split16Done::
# 151 "data/maps/BagTalk/scripts.pory"
	lock
# 152 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_16_DONE
# 153 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split16Done_Text_0
# 154 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split17Done::
# 159 "data/maps/BagTalk/scripts.pory"
	lock
# 160 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_17_DONE
# 161 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split17Done_Text_0
# 162 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split18Done::
# 167 "data/maps/BagTalk/scripts.pory"
	lock
# 168 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_18_DONE
# 169 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split18Done_Text_0
# 170 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split19Done::
# 175 "data/maps/BagTalk/scripts.pory"
	lock
# 176 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_19_DONE
# 177 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split19Done_Text_0
# 178 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split1Done_Text_0:
# 29 "data/maps/BagTalk/scripts.pory"
	.string "Split 1 completed!$"

BagTalk_EventScript_Split2Done_Text_0:
# 38 "data/maps/BagTalk/scripts.pory"
	.string "Split 2 completed!$"

BagTalk_EventScript_Split3Done_Text_0:
# 47 "data/maps/BagTalk/scripts.pory"
	.string "Split 3 completed!$"

BagTalk_EventScript_Split4Done_Text_0:
# 56 "data/maps/BagTalk/scripts.pory"
	.string "Split 4 completed!$"

BagTalk_EventScript_Split5Done_Text_0:
# 65 "data/maps/BagTalk/scripts.pory"
	.string "Split 5 completed!$"

BagTalk_EventScript_Split6Done_Text_0:
# 73 "data/maps/BagTalk/scripts.pory"
	.string "Split 6 completed!$"

BagTalk_EventScript_Split7Done_Text_0:
# 81 "data/maps/BagTalk/scripts.pory"
	.string "Split 7 completed!$"

BagTalk_EventScript_Split8Done_Text_0:
# 89 "data/maps/BagTalk/scripts.pory"
	.string "Split 8 completed!$"

BagTalk_EventScript_Split9Done_Text_0:
# 97 "data/maps/BagTalk/scripts.pory"
	.string "Split 9 completed!$"

BagTalk_EventScript_Split10Done_Text_0:
# 105 "data/maps/BagTalk/scripts.pory"
	.string "Split 10 completed!$"

BagTalk_EventScript_Split11Done_Text_0:
# 113 "data/maps/BagTalk/scripts.pory"
	.string "Split 11 completed!$"

BagTalk_EventScript_Split12Done_Text_0:
# 121 "data/maps/BagTalk/scripts.pory"
	.string "Split 12 completed!$"

BagTalk_EventScript_Split13Done_Text_0:
# 129 "data/maps/BagTalk/scripts.pory"
	.string "Split 13 completed!$"

BagTalk_EventScript_Split14Done_Text_0:
# 137 "data/maps/BagTalk/scripts.pory"
	.string "Split 14 completed!$"

BagTalk_EventScript_Split15Done_Text_0:
# 145 "data/maps/BagTalk/scripts.pory"
	.string "Split 15 completed!$"

BagTalk_EventScript_Split16Done_Text_0:
# 153 "data/maps/BagTalk/scripts.pory"
	.string "Split 16 completed!$"

BagTalk_EventScript_Split17Done_Text_0:
# 161 "data/maps/BagTalk/scripts.pory"
	.string "Split 17 completed!$"

BagTalk_EventScript_Split18Done_Text_0:
# 169 "data/maps/BagTalk/scripts.pory"
	.string "Split 18 completed!$"

BagTalk_EventScript_Split19Done_Text_0:
# 177 "data/maps/BagTalk/scripts.pory"
	.string "Split 19 completed!$"

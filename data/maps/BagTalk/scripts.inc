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
	setflag FLAG_SYS_NATIONAL_DEX
# 16 "data/maps/BagTalk/scripts.pory"
	special EnableNationalPokedex
# 18 "data/maps/BagTalk/scripts.pory"
	releaseall
	return


BagTalk_EventScript_RoxanneTest::
# 23 "data/maps/BagTalk/scripts.pory"
	lockall
# 24 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 1
# 25 "data/maps/BagTalk/scripts.pory"
	releaseall
	return


BagTalk_EventScript_Split1Done::
# 29 "data/maps/BagTalk/scripts.pory"
	lock
# 30 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_1_DONE
# 31 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 2
# 32 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split1Done_Text_0
# 33 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split2Done::
# 38 "data/maps/BagTalk/scripts.pory"
	lock
# 39 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_2_DONE
# 40 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 3
# 41 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split2Done_Text_0
# 42 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split3Done::
# 47 "data/maps/BagTalk/scripts.pory"
	lock
# 48 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_3_DONE
# 49 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 4
# 50 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split3Done_Text_0
# 51 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split4Done::
# 56 "data/maps/BagTalk/scripts.pory"
	lock
# 57 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_4_DONE
# 58 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 5
# 59 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split4Done_Text_0
# 60 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split5Done::
# 65 "data/maps/BagTalk/scripts.pory"
	lock
# 66 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_5_DONE
# 67 "data/maps/BagTalk/scripts.pory"
	setvar VAR_CURRENT_SPLIT, 6
# 68 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split5Done_Text_0
# 69 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split6Done::
# 74 "data/maps/BagTalk/scripts.pory"
	lock
# 75 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_6_DONE
# 76 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split6Done_Text_0
# 77 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split7Done::
# 82 "data/maps/BagTalk/scripts.pory"
	lock
# 83 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_7_DONE
# 84 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split7Done_Text_0
# 85 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split8Done::
# 90 "data/maps/BagTalk/scripts.pory"
	lock
# 91 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_8_DONE
# 92 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split8Done_Text_0
# 93 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split9Done::
# 98 "data/maps/BagTalk/scripts.pory"
	lock
# 99 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_9_DONE
# 100 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split9Done_Text_0
# 101 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split10Done::
# 106 "data/maps/BagTalk/scripts.pory"
	lock
# 107 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_10_DONE
# 108 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split10Done_Text_0
# 109 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split11Done::
# 114 "data/maps/BagTalk/scripts.pory"
	lock
# 115 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_11_DONE
# 116 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split11Done_Text_0
# 117 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split12Done::
# 122 "data/maps/BagTalk/scripts.pory"
	lock
# 123 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_12_DONE
# 124 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split12Done_Text_0
# 125 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split13Done::
# 130 "data/maps/BagTalk/scripts.pory"
	lock
# 131 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_13_DONE
# 132 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split13Done_Text_0
# 133 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split14Done::
# 138 "data/maps/BagTalk/scripts.pory"
	lock
# 139 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_14_DONE
# 140 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split14Done_Text_0
# 141 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split15Done::
# 146 "data/maps/BagTalk/scripts.pory"
	lock
# 147 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_15_DONE
# 148 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split15Done_Text_0
# 149 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split16Done::
# 154 "data/maps/BagTalk/scripts.pory"
	lock
# 155 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_16_DONE
# 156 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split16Done_Text_0
# 157 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split17Done::
# 162 "data/maps/BagTalk/scripts.pory"
	lock
# 163 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_17_DONE
# 164 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split17Done_Text_0
# 165 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split18Done::
# 170 "data/maps/BagTalk/scripts.pory"
	lock
# 171 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_18_DONE
# 172 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split18Done_Text_0
# 173 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split19Done::
# 178 "data/maps/BagTalk/scripts.pory"
	lock
# 179 "data/maps/BagTalk/scripts.pory"
	setflag FLAG_SPLIT_19_DONE
# 180 "data/maps/BagTalk/scripts.pory"
	msgbox BagTalk_EventScript_Split19Done_Text_0
# 181 "data/maps/BagTalk/scripts.pory"
	release
	end


BagTalk_EventScript_Split1Done_Text_0:
# 32 "data/maps/BagTalk/scripts.pory"
	.string "Split 1 completed!$"

BagTalk_EventScript_Split2Done_Text_0:
# 41 "data/maps/BagTalk/scripts.pory"
	.string "Split 2 completed!$"

BagTalk_EventScript_Split3Done_Text_0:
# 50 "data/maps/BagTalk/scripts.pory"
	.string "Split 3 completed!$"

BagTalk_EventScript_Split4Done_Text_0:
# 59 "data/maps/BagTalk/scripts.pory"
	.string "Split 4 completed!$"

BagTalk_EventScript_Split5Done_Text_0:
# 68 "data/maps/BagTalk/scripts.pory"
	.string "Split 5 completed!$"

BagTalk_EventScript_Split6Done_Text_0:
# 76 "data/maps/BagTalk/scripts.pory"
	.string "Split 6 completed!$"

BagTalk_EventScript_Split7Done_Text_0:
# 84 "data/maps/BagTalk/scripts.pory"
	.string "Split 7 completed!$"

BagTalk_EventScript_Split8Done_Text_0:
# 92 "data/maps/BagTalk/scripts.pory"
	.string "Split 8 completed!$"

BagTalk_EventScript_Split9Done_Text_0:
# 100 "data/maps/BagTalk/scripts.pory"
	.string "Split 9 completed!$"

BagTalk_EventScript_Split10Done_Text_0:
# 108 "data/maps/BagTalk/scripts.pory"
	.string "Split 10 completed!$"

BagTalk_EventScript_Split11Done_Text_0:
# 116 "data/maps/BagTalk/scripts.pory"
	.string "Split 11 completed!$"

BagTalk_EventScript_Split12Done_Text_0:
# 124 "data/maps/BagTalk/scripts.pory"
	.string "Split 12 completed!$"

BagTalk_EventScript_Split13Done_Text_0:
# 132 "data/maps/BagTalk/scripts.pory"
	.string "Split 13 completed!$"

BagTalk_EventScript_Split14Done_Text_0:
# 140 "data/maps/BagTalk/scripts.pory"
	.string "Split 14 completed!$"

BagTalk_EventScript_Split15Done_Text_0:
# 148 "data/maps/BagTalk/scripts.pory"
	.string "Split 15 completed!$"

BagTalk_EventScript_Split16Done_Text_0:
# 156 "data/maps/BagTalk/scripts.pory"
	.string "Split 16 completed!$"

BagTalk_EventScript_Split17Done_Text_0:
# 164 "data/maps/BagTalk/scripts.pory"
	.string "Split 17 completed!$"

BagTalk_EventScript_Split18Done_Text_0:
# 172 "data/maps/BagTalk/scripts.pory"
	.string "Split 18 completed!$"

BagTalk_EventScript_Split19Done_Text_0:
# 180 "data/maps/BagTalk/scripts.pory"
	.string "Split 19 completed!$"

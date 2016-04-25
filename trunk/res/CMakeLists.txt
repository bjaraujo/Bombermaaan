
PROJECT(BomberMaan32)

SET(RES_HEADERS
res.h
StdAfx.h
)

SET(RES_SOURCES
res.cpp
StdAfx.cpp
resource.rc
)

SET(RES_RESOURCES_IMAGES
image/arena_arrows.bmp
image/arena_bomb.bmp
image/arena_bomber_death.bmp
image/arena_bomber_lift.bmp
image/arena_bomber_punch.bmp
image/arena_bomber_stunt.bmp
image/arena_bomber_throw.bmp
image/arena_bomber_walk.bmp
image/arena_bomber_walk_hold.bmp
image/arena_fire.bmp
image/arena_flame.bmp
image/arena_floor.bmp
image/arena_fly.bmp
image/arena_fumes.bmp
image/arena_hurry.bmp
image/arena_item.bmp
image/arena_pause.bmp
image/arena_remote_bomb.bmp
image/arena_wall.bmp
image/board_background.bmp
image/board_clock_bottom.bmp
image/board_clock_top.bmp
image/board_drawgame.bmp
image/board_heads.bmp
image/board_score.bmp
image/board_time.bmp
image/controls_background_1.bmp
image/controls_background_2.bmp
image/controls_background_3.bmp
image/drawgame_flag.bmp
image/drawgame_fumes.bmp
image/drawgame_main.bmp
image/global_font.bmp
image/level_mini_bombers.bmp
image/level_mini_tiles.bmp
image/menu_background_1.bmp
image/menu_background_2.bmp
image/menu_background_3.bmp
image/menu_bomber.bmp
image/menu_frame_1.bmp
image/menu_frame_2.bmp
image/menu_hand.bmp
image/menu_hand_title.bmp
image/title_background.bmp
image/title_bombers.bmp
image/title_cloud_1.bmp
image/title_cloud_2.bmp
image/title_cloud_3.bmp
image/title_menu_items.bmp
image/title_title.bmp
image/victory_bomber.bmp
image/victory_confettis_large.bmp
image/victory_confettis_medium.bmp
image/victory_confettis_small.bmp
image/victory_crowd.bmp
image/victory_title.bmp
image/victory_wall.bmp
image/winner_background_1.bmp
image/winner_background_2.bmp
image/winner_background_3.bmp
image/winner_bomber.bmp
image/winner_coin.bmp
image/winner_cross.bmp
image/winner_lights.bmp
image/winner_sparks.bmp
image/winner_title.bmp
)

SET(RES_RESOURCES_SOUNDS
sound/BOMB_BOUNCE.ogg
sound/BOMB_DROP.ogg
sound/BOMBER_DEATH.ogg
sound/BOMBER_LOSE_ITEM.ogg
sound/BOMBER_PUNCH.ogg
sound/BOMBER_THROW.ogg
sound/BREAK_1.ogg
sound/BREAK_2.ogg
sound/CONTROLS_MUSIC.S3M
sound/DRAW_GAME.ogg
sound/EXPLOSION_01_1.ogg
sound/EXPLOSION_01_2.ogg
sound/EXPLOSION_02_1.ogg
sound/EXPLOSION_02_2.ogg
sound/EXPLOSION_03_1.ogg
sound/EXPLOSION_03_2.ogg
sound/EXPLOSION_04_1.ogg
sound/EXPLOSION_04_2.ogg
sound/EXPLOSION_05_1.ogg
sound/EXPLOSION_05_2.ogg
sound/EXPLOSION_06_1.ogg
sound/EXPLOSION_06_2.ogg
sound/EXPLOSION_07_1.ogg
sound/EXPLOSION_07_2.ogg
sound/EXPLOSION_08_1.ogg
sound/EXPLOSION_08_2.ogg
sound/EXPLOSION_09_1.ogg
sound/EXPLOSION_09_2.ogg
sound/EXPLOSION_10_1.ogg
sound/EXPLOSION_10_2.ogg
sound/HURRY.ogg
sound/ITEM_FUMES.ogg
sound/MATCH_MUSIC_1_FAST.MOD
sound/MATCH_MUSIC_1_NORMAL.MOD
sound/MENU_BEEP.ogg
sound/MENU_ERROR.ogg
sound/MENU_MUSIC.S3M
sound/MENU_NEXT.ogg
sound/MENU_PREVIOUS.ogg
sound/PAUSE.ogg
sound/PICK_ITEM_1.ogg
sound/PICK_ITEM_2.ogg
sound/RING_DING.ogg
sound/SICK_1.ogg
sound/SICK_2.ogg
sound/SICK_3.ogg
sound/TITLE_MUSIC.S3M
sound/VICTORY.ogg
sound/WALL_CLAP_1.ogg
sound/WALL_CLAP_2.ogg
sound/WINNER.ogg
)

SOURCE_GROUP("Header Files" FILES ${RES_HEADERS})
SOURCE_GROUP("Source Files" FILES ${RES_SOURCES})
SOURCE_GROUP("Resource Files" FILES ${RES_RESOURCES_IMAGES} ${RES_RESOURCES_SOUNDS})

ADD_LIBRARY(BomberMaan32 SHARED ${RES_HEADERS} ${RES_SOURCES} ${RES_RESOURCES_IMAGES} ${RES_RESOURCES_SOUNDS})

ADD_CUSTOM_COMMAND(TARGET BomberMaan32 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/\$\(Configuration\)/BomberMaan32.dll ${PROJECT_BINARY_DIR}/\$\(Configuration\)/../src/BomberMaan32.dll)



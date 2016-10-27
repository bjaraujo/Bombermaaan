
PROJECT(Bombermaaan32)

SET(RES_HEADERS
res.h
)

SET(RES_SOURCES
res.cpp
)

SET(RES_RESOURCES
res.rc
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
sound/bomb_bounce.ogg
sound/bomb_drop.ogg
sound/bomber_death.ogg
sound/bomber_lose_item.ogg
sound/bomber_punch.ogg
sound/bomber_throw.ogg
sound/break_1.ogg
sound/break_2.ogg
sound/controls_music.s3m
sound/draw_game.ogg
sound/draw_game_voice.ogg
sound/explosion_01_1.ogg
sound/explosion_01_2.ogg
sound/explosion_02_1.ogg
sound/explosion_02_2.ogg
sound/explosion_03_1.ogg
sound/explosion_03_2.ogg
sound/explosion_04_1.ogg
sound/explosion_04_2.ogg
sound/explosion_05_1.ogg
sound/explosion_05_2.ogg
sound/explosion_06_1.ogg
sound/explosion_06_2.ogg
sound/explosion_07_1.ogg
sound/explosion_07_2.ogg
sound/explosion_08_1.ogg
sound/explosion_08_2.ogg
sound/explosion_09_1.ogg
sound/explosion_09_2.ogg
sound/explosion_10_1.ogg
sound/explosion_10_2.ogg
sound/hurry.ogg
sound/item_fumes.ogg
sound/match_music_1_fast.mod
sound/match_music_1_normal.mod
sound/menu_beep.ogg
sound/menu_error.ogg
sound/menu_music.s3m
sound/menu_next.ogg
sound/menu_previous.ogg
sound/pause.ogg
sound/pick_item_1.ogg
sound/pick_item_2.ogg
sound/ring_ding.ogg
sound/sick_1.ogg
sound/sick_2.ogg
sound/sick_3.ogg
sound/title_music.s3m
sound/victory.ogg
sound/victory_voice.ogg
sound/wall_clap_1.ogg
sound/wall_clap_2.ogg
sound/winner.ogg
)

SOURCE_GROUP("Header Files" FILES ${RES_HEADERS})
SOURCE_GROUP("Source Files" FILES ${RES_SOURCES})
SOURCE_GROUP("Resource Files" FILES ${RES_RESOURCES_IMAGES} ${RES_RESOURCES_SOUNDS})

ADD_LIBRARY(Bombermaaan32 SHARED ${RES_HEADERS} ${RES_SOURCES} ${RES_RESOURCES_IMAGES} ${RES_RESOURCES_SOUNDS} ${RES_RESOURCES})

ADD_CUSTOM_COMMAND(TARGET Bombermaaan32 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/$\(Configuration\)/Bombermaaan32.dll ${PROJECT_BINARY_DIR}/../src/$\(Configuration\)/Bombermaaan32.dll)



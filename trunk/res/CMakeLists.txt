
project(Bombermaaan32)

set(RES_HEADERS
res.h
)

set(RES_SOURCES
res.cpp
)

set(RES_RESOURCES
res.rc
)

set(RES_RESOURCES_IMAGES
images/red_background_1.bmp
images/red_background_2.bmp
images/red_background_3.bmp
images/blue_background_1.bmp
images/blue_background_2.bmp
images/blue_background_3.bmp
images/green_background_1.bmp
images/green_background_2.bmp
images/green_background_3.bmp
images/purple_background_1.bmp
images/purple_background_2.bmp
images/purple_background_3.bmp
images/arena_arrows.bmp
images/arena_bomb.bmp
images/arena_bomber_death.bmp
images/arena_bomber_lift.bmp
images/arena_bomber_punch.bmp
images/arena_bomber_stunt.bmp
images/arena_bomber_throw.bmp
images/arena_bomber_walk.bmp
images/arena_bomber_walk_hold.bmp
images/arena_fire.bmp
images/arena_flame.bmp
images/arena_floor.bmp
images/arena_fly.bmp
images/arena_fumes.bmp
images/arena_hurry.bmp
images/arena_item.bmp
images/arena_pause.bmp
images/arena_remote_bomb.bmp
images/arena_wall.bmp
images/board_background.bmp
images/board_clock_bottom.bmp
images/board_clock_top.bmp
images/board_drawgame.bmp
images/board_heads.bmp
images/board_score.bmp
images/board_time.bmp
images/drawgame_flag.bmp
images/drawgame_fumes.bmp
images/drawgame_main.bmp
images/global_font.bmp
images/level_mini_bombers.bmp
images/level_mini_tiles.bmp
images/menu_bomber.bmp
images/menu_frame_1.bmp
images/menu_frame_2.bmp
images/menu_hand.bmp
images/menu_hand_title.bmp
images/title_background.bmp
images/title_bombers.bmp
images/title_cloud_1.bmp
images/title_cloud_2.bmp
images/title_cloud_3.bmp
images/title_menu_items.bmp
images/title_title.bmp
images/victory_bomber.bmp
images/victory_confettis_large.bmp
images/victory_confettis_medium.bmp
images/victory_confettis_small.bmp
images/victory_crowd.bmp
images/victory_title.bmp
images/victory_wall.bmp
images/winner_bomber.bmp
images/winner_coin.bmp
images/winner_cross.bmp
images/winner_lights.bmp
images/winner_sparks.bmp
images/winner_title.bmp
)

set(RES_RESOURCES_SOUNDS
sounds/bomb_bounce.ogg
sounds/bomb_drop.ogg
sounds/bomber_death.ogg
sounds/bomber_lose_item.ogg
sounds/bomber_punch.ogg
sounds/bomber_throw.ogg
sounds/break_1.ogg
sounds/break_2.ogg
sounds/controls_music.s3m
sounds/draw_game.ogg
sounds/draw_game_voice.ogg
sounds/explosion_01_1.ogg
sounds/explosion_01_2.ogg
sounds/explosion_02_1.ogg
sounds/explosion_02_2.ogg
sounds/explosion_03_1.ogg
sounds/explosion_03_2.ogg
sounds/explosion_04_1.ogg
sounds/explosion_04_2.ogg
sounds/explosion_05_1.ogg
sounds/explosion_05_2.ogg
sounds/explosion_06_1.ogg
sounds/explosion_06_2.ogg
sounds/explosion_07_1.ogg
sounds/explosion_07_2.ogg
sounds/explosion_08_1.ogg
sounds/explosion_08_2.ogg
sounds/explosion_09_1.ogg
sounds/explosion_09_2.ogg
sounds/explosion_10_1.ogg
sounds/explosion_10_2.ogg
sounds/hurry.ogg
sounds/item_fumes.ogg
sounds/match_music_1_fast.mod
sounds/match_music_1_normal.mod
sounds/menu_beep.ogg
sounds/menu_error.ogg
sounds/menu_music.s3m
sounds/menu_next.ogg
sounds/menu_previous.ogg
sounds/pause.ogg
sounds/pick_item_1.ogg
sounds/pick_item_2.ogg
sounds/ring_ding.ogg
sounds/sick_1.ogg
sounds/sick_2.ogg
sounds/sick_3.ogg
sounds/title_music.s3m
sounds/victory.ogg
sounds/victory_voice.ogg
sounds/wall_clap_1.ogg
sounds/wall_clap_2.ogg
sounds/winner.ogg
)

source_group("Header Files" FILES ${RES_HEADERS})
source_group("Source Files" FILES ${RES_SOURCES})
source_group("Resource Files" FILES ${RES_RESOURCES_IMAGES} ${RES_RESOURCES_SOUNDS})

add_library(Bombermaaan32 MODULE ${RES_HEADERS} ${RES_SOURCES} ${RES_RESOURCES_IMAGES} ${RES_RESOURCES_SOUNDS} ${RES_RESOURCES})

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  add_custom_command(TARGET Bombermaaan32 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/$\(Configuration\)/Bombermaaan32.dll ${PROJECT_BINARY_DIR}/../src/$\(Configuration\)/Bombermaaan32.dll)
endif()



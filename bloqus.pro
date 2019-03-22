
TEMPLATE = app
CONFIG += warn_on debug_and_release
QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9

CONFIG(debug, debug|release) {
  TARGET = bloqus_debug
  DEFINES += _DEBUG
  MOC_DIR = .moc_debug
  OBJECTS_DIR = .objs_debug
} else {
  TARGET = bloqus
  MOC_DIR = .moc_release
  UI_DIR = .ui_release
  OBJECTS_DIR = .objs_release
}

UI_DIR = .ui
RCC_DIR = .rcc

INCLUDEPATH += src
SOURCES += src/main.cpp \
    src/bloqus.cpp \
    src/bloqus_game.cpp \
    src/bloqus_game_piece.cpp \
    src/bloqus_game_player.cpp \
    src/bloqus_game_grid.cpp \
    src/bloqus_game_game.cpp \
    src/bloqus_gui_grid_square.cpp \
    src/bloqus_gui_grid.cpp \
    src/bloqus_gui_piece.cpp \
    src/bloqus_gui.cpp \
    src/bloqus_gui_game_scene.cpp \
    src/bloqus_gui_game_view.cpp \
    src/main_menu.cpp \
    src/game_setup.cpp \
    src/bloqus_gui_player.cpp \
    src/gui_setup.cpp \
    src/bloqus_game_computer_player.cpp \
    src/bloqus_game_human_player.cpp \
    src/game_results.cpp \
    src/game_window.cpp \
    src/player_setup.cpp \
    src/player_result.cpp \
    src/bloqus_game_computer_player_strength_0.cpp \
    src/bloqus_game_computer_player_strength_1.cpp \
    src/bloqus_game_computer_player_strength_2.cpp \
    src/bloqus_game_computer_player_strength_3.cpp \
    src/bloqus_game_computer_player_strength_4.cpp \
    src/bloqus_game_computer_player_strength_5.cpp
HEADERS += src/bloqus.h \
    src/bloqus_game.h \
    src/bloqus_game_piece.h \
    src/bloqus_game_player.h \
    src/bloqus_game_grid.h \
    src/bloqus_game_game.h \
    src/bloqus_gui_grid.h \
    src/bloqus_gui_grid_square.h \
    src/bloqus_gui.h \
    src/bloqus_gui_piece.h \
    src/bloqus_gui_game_scene.h \
    src/bloqus_gui_game_view.h \
    src/main_menu.h \
    src/game_setup.h \
    src/bloqus_gui_player.h \
    src/gui_setup.h \
    src/bloqus_game_computer_player.h \
    src/bloqus_game_human_player.h \
    src/game_results.h \
    src/game_window.h \
    src/player_setup.h \
    src/player_result.h \
    src/bloqus_game_computer_player_strength_0.h \
    src/bloqus_game_computer_player_strength_1.h \
    src/bloqus_game_computer_player_strength_2.h \
    src/bloqus_game_computer_player_strength_3.h \
    src/bloqus_game_computer_player_strength_4.h \
    src/bloqus_game_computer_player_strength_5.h
FORMS += forms/main_menu.ui \
    forms/game_setup.ui \
    forms/gui_setup.ui \
    forms/game_results.ui \
    forms/game_window.ui \
    forms/player_setup.ui \
    forms/player_result.ui
RESOURCES += ressources.qrc
TRANSLATIONS += i18n/bloqus_en.ts \
    i18n/bloqus_fr.ts
win32:RC_FILE = win_icon.rc

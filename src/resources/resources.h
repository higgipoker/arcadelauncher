#pragma once

#include <string>
#include <vector>

#include "../sdl/text.h"
#include "../widgets/background.h"
#include "../widgets/icon.h"
#include "../widgets/screeneffect.h"
#include "../widgets/video.h"

/**
 * @brief supported platforms
 */
enum PLATFORM {

    UNKNOWN = 0,
    MAME,
    PC,
    AMIGA,
    SNES,
    GENESIS
};

/**
 * @brief represent a game in the menu
 */
struct GAME {

    /**
     * @brief initializing constructor
     */
    GAME(void) {
        type = UNKNOWN;
        icon = NULL;
        menu_bg = NULL;
    }

    /// platform (eg mame, amiga)
    PLATFORM type;

    /// string name
    std::string gamename;

    /// file to execute
    std::string exec_file;

    /// emulator executable
    std::string exec_command;

    /// menu icon
    Icon *icon;

    /// video preview name
    std::string video_name;

    /// menu background (just a pointer, saves the menu looking up from platform
    /// every time )
    Background *menu_bg;
};

/**
 * @brief a resource manager
 */
class Resources {

public:
    /// the game list
    static std::vector<GAME *> game_list;

    /**
     * @brief constructor
     */
    Resources(void);

    /**
     * @brief init the resource manager
     */
    static void Init(void);

    /**
     * @brief clean up the memory of the static class
     */
    static void Destroy(void);

    /**
     * @breif universal helper to render the screen effect
     */
    static void RenderScreenEffect(void);

    /**
     * @brief get a game based on name
     */
    static GAME *GetGame(const std::string &rom_name);

    /**
     * @brief get background image filename for a platform
     */
    static std::string GetBackgroundName(PLATFORM _platform);

private:
    /// list of icons
    static std::vector<Icon *> icons;

    /// a screen effect
    static ScreenEffect *screen_effect;

    /**
     * @brief helper to make an icon
     */
    static Icon *make_icon(const std::string &search_dir, const std::string &name);

    /**
     * @brief helper to get the screen effect
     */
    static ScreenEffect *get_screen_effect(void);

    /**
     * @brief helper to load games for mame platform
     */
    static void load_mame_games(void);

    /**
     * @brief helper to load games for pc platform
     */
    static void load_pc_games(void);

    /**
     * @brief helper to load games for amiga platform
     */
    static void load_amiga_games(void);

    /**
     * @brief helper to load games for snes platform
     */
    static void load_snes_games(void);

    /**
     * @brief helper to load games for genesis platform
     */
    static void load_genesis_games(void);

    /**
     * @brief helper to strip out file extensions
     */
    static std::string strip_extension(const std::string &_str);

    /// pairs < platform name, background image >
    static std::vector< std::pair<PLATFORM, std::string> > backgrounds;

    /**
     * @brief a helper to create fs-uae params (can be multi disk)
     */
    static std::string make_amiga_game(const std::string &_games_folder,
                                       const std::string &_kicks_folder,
                                       const std::string &_game_name,
                                       std::vector<std::string> _disks);

    /**
    * @brief a helper to create fs-uae params (can be multi disk)
    */
    static std::string make_amiga_game(const std::string &_games_folder,
                                       const std::string &_kicks_folder,
                                       const std::string &_disk);

    /// only init once
    static bool inited;
};

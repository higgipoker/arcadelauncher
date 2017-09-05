#pragma once

#include "file.h"

class ConfigFile : public File {
public:
    /**
      * @brief constructor
      * @param f file name
      */
    ConfigFile(const std::string &f);

    /**
     * @brief default constructor
     */
    ConfigFile();

    /**
     * @brief open file
     * @param f filename
     */
    virtual void Open(const std::string &f);


    /// all the data vals
    std::string platform_root;
    std::string mame_executable;
    std::string mame_ini;
    std::string mame_games;
    std::string mame_artwork;
    std::string mame_icons;
    std::string mame_videos;
    std::string theme;
    std::string grid_theme_rows;
    std::string grid_theme_cols;
    std::string screen_width;
    std::string screen_height;
    std::string screen_offset_x;
    std::string screen_offset_y;
    std::string background_image;
    std::string coverflow_scroll_icon;
    std::string generic_icon;
    std::string icon_border;
    std::string icon_border_lit;
    std::string screen_effect;
    std::string screen_effect_intensity;
    std::string screen_effect_intensity_flicker;
    std::string screen_mode;
    std::string video;
    std::string single_game;
    std::string amiga_executable;
    std::string amiga_games;
    std::string amiga_kicks;
    std::string amiga_artwork;
    std::string amiga_icons;
    std::string amiga_vids;
    std::string pc_games;

private:
    /// data keys
    std::vector< std::pair<std::string, std::string> > keys;

    /// shortcut to home replace_home_folder
    std::string home_folder_string;

    /**
     * @brief helper
     */
    void read_data();

    /**
     * @brief helper
     */
    std::string replace_home_folder(const std::string &original);
};

#include "configfile.h"

// ------------------------------------------------------------
// ConfigFile
// ------------------------------------------------------------
ConfigFile::ConfigFile() {
    opened_for_read = opened_for_write = false;

}

// ------------------------------------------------------------
// ConfigFile
// ------------------------------------------------------------
ConfigFile::ConfigFile(const std::string &f) : File(f) {
    theme = "coverflow";
    grid_theme_rows = "3";
    grid_theme_cols = "3";
}

// ------------------------------------------------------------
// read_data
// ------------------------------------------------------------
void ConfigFile::read_data() {
    GetLines();

    // make keys
    for (unsigned int i = 0; i < lines.size(); i++) {

        std::pair<std::string, std::string> p;

        p.first = lines[i].substr(0, lines[i].find_first_of(' '));
        p.second = lines[i].substr(lines[i].find_first_of(' ') + 1);

        if (p.first == "home") {
            home_folder_string = p.second;
        } else {

            p.second = replace_home_folder(p.second);
            // temp just set here

            if (p.first == "platform_root") {
                platform_root = p.second;
                keys.push_back(p);
            }

            else if (p.first == "mame_executable") {
                mame_executable = p.second;
                keys.push_back(p);
            } else if (p.first == "mame_ini") {
                mame_ini = p.second;
                keys.push_back(p);
            } else if (p.first == "mame_games") {
                mame_games = p.second;
                keys.push_back(p);
            } else if (p.first == "mame_artwork") {
                mame_artwork = p.second;
                keys.push_back(p);
            } else if (p.first == "mame_icons") {
                mame_icons = p.second;
                keys.push_back(p);
            } else if (p.first == "mame_videos") {
                mame_videos = p.second;
                keys.push_back(p);
            } else if (p.first == "theme") {
                theme = p.second;
                keys.push_back(p);
            } else if (p.first == "grid_theme_rows") {
                grid_theme_rows = p.second;
                keys.push_back(p);
            } else if (p.first == "grid_theme_cols") {
                grid_theme_cols = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_width") {
                screen_width = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_height") {
                screen_height = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_offset_x") {
                screen_offset_x = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_offset_y") {
                screen_offset_y = p.second;
                keys.push_back(p);
            } else if (p.first == "background") {
                background_image = p.second;
                keys.push_back(p);
            } else if (p.first == "coverflow_scroll_icon") {
                coverflow_scroll_icon = p.second;
                keys.push_back(p);
            } else if (p.first == "generic_icon") {
                generic_icon = p.second;
                keys.push_back(p);
            } else if (p.first == "icon_border") {
                icon_border = p.second;
                keys.push_back(p);
            } else if (p.first == "icon_border_highlight") {
                icon_border_lit = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_effect") {
                screen_effect = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_effect_intensity") {
                screen_effect_intensity = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_effect_intensity_flicker") {
                screen_effect_intensity_flicker = p.second;
                keys.push_back(p);
            } else if (p.first == "screen_mode") {
                screen_mode = p.second;
                keys.push_back(p);
            } else if (p.first == "video") {
                video = p.second;
                keys.push_back(p);
            } else if (p.first == "pc_games") {
                pc_games = p.second;
                keys.push_back(p);
            } else if (p.first == "single_game") {
                single_game = p.second;
                keys.push_back(p);
            } else if (p.first == "amiga_executable") {
                amiga_executable = p.second;
                keys.push_back(p);
            } else if (p.first == "amiga_games") {
                amiga_games = p.second;
                keys.push_back(p);
            } else if (p.first == "amiga_kicks") {
                amiga_kicks = p.second;
                keys.push_back(p);
            } else if (p.first == "amiga_artwork") {
                amiga_artwork = p.second;
                keys.push_back(p);
            } else if (p.first == "amiga_icons") {
                amiga_icons = p.second;
                keys.push_back(p);
            } else if (p.first == "amiga_vids") {
                amiga_vids = p.second;
                keys.push_back(p);
            }
        }
    }
}

// ------------------------------------------------------------
// open
// ------------------------------------------------------------
void ConfigFile::Open(const std::string &f) {
    File::Open(f);

    read_data();
}

// ------------------------------------------------------------
// replace_home_folder
// ------------------------------------------------------------
std::string ConfigFile::replace_home_folder(const std::string &original) {
    std::string str = original;
    std::string from = "$HOME";
    std::string to = home_folder_string;

    size_t start_pos = str.find(from);
    if (start_pos != std::string::npos)
        str.replace(start_pos, from.length(), to);

    return str;
}

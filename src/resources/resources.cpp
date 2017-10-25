#include <algorithm>
#include <ctime>
#include <dirent.h>

#include "../config/config.h"
#include "resources.h"
#include "../sdl/sdl.h"
#include "../utils/stringtools.h"
#include "../widgets/genericicon.h"

bool Resources::inited = false;
std::vector<GAME *> Resources::game_list;
std::vector<Icon *> Resources::icons;
ScreenEffect *Resources::screen_effect = NULL;
std::vector<std::pair<PLATFORM, std::string>> Resources::backgrounds;

// ------------------------------------------------------------
// Resources
// ------------------------------------------------------------
Resources::Resources(void) {
    inited = false;
}

// ------------------------------------------------------------
// Init
// ------------------------------------------------------------
void Resources::Init(void) {
    if (inited)
        return;

    inited = true;

    game_list.clear();
    load_mame_games();
    load_pc_games();
    load_amiga_games();
    load_snes_games();
    load_genesis_games();

    // randomize the order for fun ;)
    std::srand(std::time(0));
    std::random_shuffle(game_list.begin(), game_list.end());

    // or alphabetical
    //    struct {
    //        bool operator()(const GAME *r1, const GAME *r2) const {
    //            return r1->gamename < r2->gamename;
    //        }
    //    } sort_games;
    //    std::sort(game_list.begin(), game_list.end(), sort_games);
}

// ------------------------------------------------------------
// load_mame_games
// ------------------------------------------------------------
void Resources::load_mame_games(void) {

    // the mame game folder:	platform_root/mame/games
    //
    std::string mame_games_folder = Config::data.platform_root + "/mame/games";
    std::string mame_art_folder = Config::data.platform_root + "/mame/artwork";
    std::string mame_icons_folder = Config::data.platform_root + "/mame/artwork/icons";
    std::string mame_vids_folder = Config::data.platform_root + "/mame/artwork/vids";
    std::string mame_executable = Config::data.platform_root + "/mame/bin/mame";
    std::string mame_ini = Config::data.platform_root + "/mame/other";

    DIR *dir;

    if ((dir = opendir(mame_games_folder.c_str())) != NULL) {

        struct dirent *ent;

        // for every file in the folder
        while ((ent = readdir(dir)) != NULL) {

            // only regular files
            if (ent->d_type == DT_REG) {

                // strip extension
                std::string rawname = strip_extension(std::string(ent->d_name));

                //  special case hack for neo geo folders
                if (rawname != "neogeo") {

                    // make a game object
                    GAME *new_game = new GAME();
                    new_game->gamename = rawname;
                    new_game->type = MAME;
                    new_game->exec_command =
                     mame_executable + " " + rawname + " -inipath " + mame_ini + " -rompath " + mame_games_folder;
                    new_game->icon = make_icon(mame_icons_folder, rawname);
                    new_game->video_name = mame_vids_folder + "/" + rawname + ".avi";

                    // stick in list
                    game_list.push_back(new_game);

                } // end not a neogeo folder
            }     // end check for regular file
        }         // end every file in folder

        closedir(dir);

    } else {
        /* could not open directory */
        std::string error = "could not open target directory: " + mame_games_folder;
        perror(error.c_str());
    }

    // set the mame background
    std::string bg_name = mame_art_folder + "/background.png";
    std::pair<PLATFORM, std::string> p = std::make_pair(MAME, bg_name);
    backgrounds.push_back(p);
}

// ------------------------------------------------------------
// load_snes_games
// ------------------------------------------------------------
void Resources::load_snes_games(void) {
    // the mame game folder:	platform_root/snes/games
    //
    std::string snes_games_folder = Config::data.platform_root + "/snes/games";
    std::string snes_art_folder = Config::data.platform_root + "/snes/artwork";
    std::string snes_icons_folder = Config::data.platform_root + "/snes/artwork/icons";
    std::string snes_vids_folder = Config::data.platform_root + "/snes/artwork/vids";
    std::string snes_executable = Config::data.platform_root + "/snes/bin/snes";
    std::string snes_ini = Config::data.platform_root + "/snes/other";

    DIR *dir;

    if ((dir = opendir(snes_games_folder.c_str())) != NULL) {

        struct dirent *ent;

        // for every file in the folder
        while ((ent = readdir(dir)) != NULL) {

            // only regular files
            if (ent->d_type == DT_REG) {

                std::string rawname = std::string(ent->d_name);

                //  special case hack for neo geo folders
                if (rawname != "neogeo") {

                    // make a game object
                    GAME *new_game = new GAME();
                    new_game->gamename = rawname;
                    new_game->type = SNES;
                    new_game->exec_command = snes_executable + " " + snes_games_folder + "/" + rawname;

                    // strip extension
                    rawname = strip_extension(rawname);

                    new_game->icon = make_icon(snes_icons_folder, rawname);
                    new_game->video_name = snes_vids_folder + "/" + rawname + ".avi";

                    // stick in list
                    game_list.push_back(new_game);

                } // end not a neogeo folder
            }     // end check for regular file
        }         // end every file in folder

        closedir(dir);

    } else {
        /* could not open directory */
        std::string error = "could not open target directory: " + snes_games_folder;
        perror(error.c_str());
    }

    // set the snes background
    std::string bg_name = snes_art_folder + "/background.png";
    std::pair<PLATFORM, std::string> p = std::make_pair(SNES, bg_name);
    backgrounds.push_back(p);
}

// ------------------------------------------------------------
// load_genesis_games
// ------------------------------------------------------------
void Resources::load_genesis_games(void) {
    // the mame game folder:	platform_root/genesis/games
    //
    std::string genesis_games_folder = Config::data.platform_root + "/genesis/games";
    std::string genesis_art_folder = Config::data.platform_root + "/genesis/artwork";
    std::string genesis_icons_folder = Config::data.platform_root + "/genesis/artwork/icons";
    std::string genesis_vids_folder = Config::data.platform_root + "/genesis/artwork/vids";
    std::string genesis_executable = Config::data.platform_root + "/genesis/bin/genesis";
    std::string genesis_ini = Config::data.platform_root + "/genesis/other/keys.cfg";

    DIR *dir;

    if ((dir = opendir(genesis_games_folder.c_str())) != NULL) {

        struct dirent *ent;

        // for every file in the folder
        while ((ent = readdir(dir)) != NULL) {

            // only regular files
            if (ent->d_type == DT_REG) {

                std::string rawname = std::string(ent->d_name);

                //  special case hack for neo geo folders
                if (rawname != "neogeo") {

                    // make a game object
                    GAME *new_game = new GAME();
                    new_game->gamename = rawname;
                    new_game->type = GENESIS;
                    new_game->exec_command = genesis_executable + " -f -G 800x600 -r " + genesis_ini + " " +
                                             genesis_games_folder + "/" + rawname;

                    // strip extension
                    rawname = strip_extension(rawname);

                    new_game->icon = make_icon(genesis_icons_folder, rawname);
                    new_game->video_name = genesis_vids_folder + "/" + rawname + ".avi";

                    // stick in list
                    game_list.push_back(new_game);

                } // end not a neogeo folder
            }     // end check for regular file
        }         // end every file in folder

        closedir(dir);

    } else {
        /* could not open directory */
        std::string error = "could not open target directory: " + genesis_games_folder;
        perror(error.c_str());
    }

    // set the genesis background
    std::string bg_name = genesis_art_folder + "/background.png";
    std::pair<PLATFORM, std::string> p = std::make_pair(GENESIS, bg_name);
    backgrounds.push_back(p);
}

// ------------------------------------------------------------
// load_amiga_games
// ------------------------------------------------------------
void Resources::load_amiga_games(void) {
    // the mame game folder:	platform_root/amiga/games
    //
    std::string amiga_games_folder = Config::data.platform_root + "/amiga/games";
    std::string amiga_executable_folder = Config::data.platform_root + "/amiga/bin";
    std::string amiga_kicks_folder = Config::data.platform_root + "/amiga/other";
    std::string amiga_icons_folder = Config::data.platform_root + "/amiga/artwork/icons";
    std::string amiga_vids_folder = Config::data.platform_root + "/amiga/artwork/vids";
    std::string amiga_art_folder = Config::data.platform_root + "/amiga/artwork";
    std::string amiga_executable = amiga_executable_folder + "/amiga";

    DIR *dir;

    if ((dir = opendir(amiga_games_folder.c_str())) != NULL) {

        struct dirent *ent;

        // for every file in the folder
        while ((ent = readdir(dir)) != NULL) {

            // regular files (single disk games)
            if (ent->d_type == DT_REG) {

                // for amiga emulator, we dont strip of the extension yet
                std::string rawname = std::string(ent->d_name);

                if (rawname != "." && rawname != "..") {

                    GAME *new_game = new GAME();
                    new_game->gamename = rawname;
                    new_game->type = AMIGA;
                    std::string command;

                    command = amiga_executable + " " + make_amiga_game(amiga_games_folder, amiga_kicks_folder, rawname);

                    std::cout << "fsuae cmd: " << command << std::endl << std::endl;

                    new_game->exec_command = command;

                    // now strip off the extension
                    rawname = strip_extension(ent->d_name);

                    // add icon and video
                    new_game->icon = make_icon(amiga_icons_folder, rawname);
                    new_game->video_name = amiga_vids_folder + "/" + rawname + ".avi";
                    game_list.push_back(new_game);
                }

            } else if (ent->d_type == DT_DIR) {

                std::string game_name = std::string(ent->d_name);

                if (game_name == "." || game_name == "..") {
                    continue;
                }

                DIR *dir_inner;
                std::string tmp = amiga_games_folder + "/" + std::string(ent->d_name);

                if ((dir_inner = opendir(tmp.c_str())) != NULL) {

                    // multiple disk game!
                    // get all the files in the folder
                    struct dirent *ent_inner;
                    std::vector<std::string> disks;
                    // for every file in the folder
                    std::string rawname_inner;

                    while ((ent_inner = readdir(dir_inner)) != NULL) {
                        rawname_inner = std::string(ent_inner->d_name);

                        if (ent_inner->d_type == DT_REG) {
                            if (rawname_inner != "." && rawname_inner != "..") {
                                disks.push_back(rawname_inner);
                            }
                        }
                    }

                    std::sort(disks.begin(), disks.end());
                    GAME *new_game2 = new GAME();
                    new_game2->gamename = game_name;
                    new_game2->type = AMIGA;
                    std::string command2;

                    command2 = amiga_executable + " " +
                               make_amiga_game(amiga_games_folder, amiga_kicks_folder, new_game2->gamename, disks);

                    std::cout << "fsuae cmd: " << command2 << std::endl << std::endl;

                    new_game2->exec_command = command2;

                    // add icon and video
                    new_game2->icon = make_icon(amiga_icons_folder, game_name);
                    new_game2->video_name = amiga_vids_folder + "/" + game_name + ".avi";
                    game_list.push_back(new_game2);
                }

                closedir(dir_inner);
            }
        }

        closedir(dir);

    } else {
        /* could not open directory */
        std::string error = "could not open target directory: " + amiga_games_folder;
        perror(error.c_str());
    }

    // set the amiga background
    std::string bg_name = amiga_art_folder + "/background.png";
    std::pair<PLATFORM, std::string> p = std::make_pair(AMIGA, bg_name);
    backgrounds.push_back(p);
}

// ------------------------------------------------------------
// make_amiga_game
// ------------------------------------------------------------
std::string Resources::make_amiga_game(const std::string &_games_folder, const std::string &_kicks_folder,
                                       const std::string &_game_name, std::vector<std::string> _disks) {

    std::string params;
    int i = 0;

    for (auto it = _disks.begin(); it != _disks.end(); ++it) {
        std::ostringstream arg;
        arg << " --floppy-drive-" << i << "=";
        params += arg.str() + _games_folder + "/" + _game_name + "/" + (*it);
        i++;
    }

    // params += " --floppy_drive_speed=0";
    params += " --floppy_drive_volume=0";
    params += " --amiga_model=A500+";
    params += " --kickstarts_dir=" + _kicks_folder;
    params += " --fullscreen=1";
    params += " --fade_in_duration = 0";
    params += " --fade_out_duration = 0";

    return params;
}

// ------------------------------------------------------------
// make_amiga_game
// ------------------------------------------------------------
std::string Resources::make_amiga_game(const std::string &_games_folder, const std::string &_kicks_folder,
                                       const std::string &_disk) {

    std::string params;

    params += " --floppy-drive-0=" + _games_folder + "/" + _disk;
    params += " --floppy_drive_speed=0";
    params += " --floppy_drive_volume=0";
    params += " --amiga_model=A500";
    params += " --kickstarts_dir=" + _kicks_folder;
    params += " --fullscreen=1";
    params += " --fade_in_duration = 0";
    params += " --fade_out_duration = 0";

    return params;
}

// ------------------------------------------------------------
// make_icon
// ------------------------------------------------------------
Icon *Resources::make_icon(const std::string &search_dir, const std::string &name) {
    Icon *icon = NULL;
    DIR *dir;
    bool found = false;

    if ((dir = opendir(search_dir.c_str())) != NULL) {

        std::string filename = name + ".png";
        struct dirent *ent;

        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL && !found) {
            if (std::string(ent->d_name) != "." && (std::string)ent->d_name != "..") {

                std::string rawname = std::string(ent->d_name);

                if (rawname == filename) {
                    icon = new Icon(SDL::window(), search_dir + "/" + filename, Config::data.icon_border,
                                    Config::data.icon_border_lit);
                    found = true;
                }
            }
        }

        closedir(dir);

    } else {
        std::string error = "could not open target directory: " + Config::data.mame_icons;
        perror(error.c_str());
    }

    if (icon == NULL) {       
        icon = new GenericIcon(SDL::window(), name, Config::data.generic_icon, Config::data.icon_border,
                               Config::data.icon_border_lit);
    }

    icons.push_back(icon);
    return icon;
}

// ------------------------------------------------------------
// get_screen_effect
// ------------------------------------------------------------
ScreenEffect *Resources::get_screen_effect() {
    return screen_effect;
}

// ------------------------------------------------------------
// RenderScreenEffect
// ------------------------------------------------------------
void Resources::RenderScreenEffect() {
    if (screen_effect) {
        screen_effect->render();
    }
}

// ------------------------------------------------------------
// Destroy
// ------------------------------------------------------------
void Resources::Destroy() {
    for (auto it=icons.begin();it!=icons.end(); ++it) {
        delete *it;
    }

    for (auto it=game_list.begin(); it!=game_list.end(); ++it) {
        delete *it;
    }

    delete screen_effect;
}

// ------------------------------------------------------------
// load_pc_games
// ------------------------------------------------------------
void Resources::load_pc_games() {
    DIR *dir;

    // pc games
    if ((dir = opendir(Config::data.pc_games.c_str())) != NULL) {

        struct dirent *ent;

        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL) {

            if (ent->d_type == DT_DIR) { // found a game dir

                std::string rawname = std::string(ent->d_name);

                if (rawname != "." && rawname != "..") {

                    GAME *new_game = new GAME();
                    new_game->gamename = rawname;
                    new_game->type = PC;
                    new_game->exec_command = Config::data.pc_games + "/" + rawname + "/" + rawname;
                    new_game->icon = make_icon(Config::data.pc_games + "/" + rawname, rawname);
                    new_game->video_name = Config::data.pc_games + "/" + rawname + "/" + rawname + ".avi";

                    game_list.push_back(new_game);
                }
            }
        }

        closedir(dir);

    } else {
        /* could not open directory */
        std::string error = "could not open target directory: " + Config::data.pc_games;
        perror(error.c_str());
    }
}

// ------------------------------------------------------------
// GetGame
// ------------------------------------------------------------
GAME *Resources::GetGame(const std::string &rom_name) {
    if (rom_name == "random") {
        int idx = rand() % game_list.size() - 1;
        return game_list[idx];
    }

    for (unsigned int i = 0; i < game_list.size(); i++) {
        if (rom_name == game_list[i]->gamename)
            return game_list[i];
    }

    return NULL;
}

// ------------------------------------------------------------
// GetBackgroundName
// ------------------------------------------------------------
std::string Resources::GetBackgroundName(PLATFORM _platform) {
    for (auto it = backgrounds.begin(); it != backgrounds.end(); ++it) {
        if ((*it).first == _platform) {
            return (*it).second;
        }
    }

    return "";
}

// ------------------------------------------------------------
// strip_extension
// ------------------------------------------------------------
std::string Resources::strip_extension(const std::string &_str) {
    int lastindex = _str.find_last_of(".");
    return _str.substr(0, lastindex);
}

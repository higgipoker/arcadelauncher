#include "../gui/coverflow.h"
#include "../gui/staticgrid.h"
#include "../utils/timer.h"
#include "../widgets/background.h"
#include "../widgets/icon.h"
#include "../input/input.h"

class MainMenu {
  public:
    // --------------------------------------------
    // CONSTRUCTOR / DESTRUCTOR
    // --------------------------------------------
    MainMenu();
    ~MainMenu();
    // --------------------------------------------

    void launchGame(const std::string &rom_name);

    void init(SDLWindow *window);
    void addGame(GAME *new_game);
    void update();
    void render();
    void handleInput(Event event);
    void start();
    void stop();

    void resume() { selection_theme->returning_from_game = false; }
    bool returningFromGame() { return selection_theme->returning_from_game; }

  private:
    /// background image
    Background *background;
    Background *background2;

    SDLWindow *wnd;

    std::vector< std::pair<PLATFORM, Background *> > backgrounds;
    Background *getBackground(PLATFORM _platform);
    void setBackground(PLATFORM _platform);

    /// a game selection theme
    SelectionTheme *selection_theme;

    /// games list
    std::vector<std::string> added_games;

    /// flags
    bool inited;
    bool running;

    Mix_Chunk *snd_mouhaha;
    Mix_Chunk *snd_choose;
};

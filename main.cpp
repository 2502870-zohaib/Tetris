/*Muhammad Zohaib (2502870)
Muhammad Abdullah (2502946)*/

/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You don't need to change anything in the driver code/main function except the part(s) where instructed.
 * You are expected to define your functionalities in the "functionality.h" header and call them here in the instructed part(s).
 * The game is started with a small box, you need to create other possible in "pieces.h" file in form of array.
 * E.g., Syntax for 2 pieces, each having four coordinates -> int Pieces[2][4];
 * Load the images into the textures objects from img directory.
 * Don't make any changes to the "utils.h" header file except for changing the coordinate of screen or global variables of the game.
 * SUGGESTION: If you want to make changes in "utils.h", do it once you complete rest of the functionalities.
 * TIP: You can study SFML for drawing scenes, sprites and window.
 * */

#include <SFML/Graphics.hpp>
#include <ctime>
#include <optional>          
#include "utils.h"
#include "pieces.h"
#include "functionality.h"

using namespace sf;

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));


    RenderWindow window(VideoMode({320, 480}),title);


    Texture obj1, obj2, obj3;
    // Load "frames.png","tiles.png" and "background.png" for Texture objects
    if (!obj1.loadFromFile("img/tiles.png"))      return -1;
    if (!obj2.loadFromFile("img/background.png")) return -1;
    if (!obj3.loadFromFile("img/frame.png"))     return -1;

    Sprite sprite(obj1), background(obj2), frame(obj3);


    ///////////////////////////////////////////////
    ///*** START VARIABLE DECLARATIONS ***///
    float levelTimer = 0.0f; // Stores remaining time to show text
    int lastKnownLevel = 1;  // Tracks level jumps
    float difficultyTimer = 0.0f;

    // Add your UI and State variables here
    Font font;
    bool fontLoaded = font.openFromFile("img/ARIAL.TTF");
    if (!fontLoaded) {
        fontLoaded = font.openFromFile("ARIAL.TTF");
    }
    Text uiText(font), sideText(font);
    uiText.setCharacterSize(26);
    uiText.setOutlineThickness(3);
    uiText.setOutlineColor(Color::Black);
    
    sideText.setCharacterSize(20);
    sideText.setFillColor(Color::Yellow);
    sideText.setPosition({240, 100}); 

    enum GameState { MENU, PLAYING, HELP, HIGHSCORE };
    GameState currentState = MENU;
    std::vector<int> highScores;
    bool isPaused = false;
    bool game_over = false;

    ///*** END VARIABLE DECLARATIONS ***///
    /////////////////////////////////////////////


    int delta_x = 0;
    float timer = 0, delay = 0.3f;
    bool rotate = false;
    Clock clock;

    // Initialize the first piece so a random shape falls immediately
    int restrictedBlocks[] = {0, 1, 2, 3}; 
    int n = restrictedBlocks[std::rand() % 4];
    for (int i = 0; i < 4; i++) {
        point_1[i][0] = BLOCKS[n][i] % 2 + N / 2 - 1;
        point_1[i][1] = BLOCKS[n][i] / 2;
    }

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        //---Event Listening Part---//
        while (auto eOpt = window.pollEvent()) {      // Event is occurring — until the game is in running state
            auto const &event = *eOpt;

            if (event.is<sf::Event::Closed>())        // If cross/close is clicked/pressed
                window.close();                       // Opened window disposes
            
            if (const auto* keyPressed = event.getIf<Event::KeyPressed>()) {// If any other key (not cross) is pressed
                
            ///////////////////////////////////////////////
            ///*** START EVENT HANDLING HERE ***///
            
                // Global ESC handling
                if (keyPressed->code == Keyboard::Key::Escape) {
                    if (currentState == HELP || currentState == HIGHSCORE) {
                        // Return to previous state
                        currentState = (isPaused) ? PLAYING : MENU;
                    }
                    else if (game_over) {
                        // Reset game after death
                        score = 0; 
                        gameLevel = 1;
                        lastKnownLevel = 1; 
                        levelTimer = 0.0f;  
                        game_over = false; 
                        isPaused = false;
                        unusableRows = 0;
                        difficultyTimer = 0.0f;
                        for(int i=0; i<M; i++) for(int j=0; j<N; j++) gameGrid[i][j]=0;
                        currentState = MENU;
                    }
                    else if (currentState != PLAYING) {
                        currentState = MENU;
                    }
                }

                // State-specific input
                if (currentState == MENU || isPaused) {
                    if (keyPressed->code == Keyboard::Key::Num1) {
                        // Start New Game: Reset everything
                        score = 0; 
                        gameLevel = 1;
                        lastKnownLevel = 1; 
                        levelTimer = 0.0f;   
                        game_over = false; 
                        isPaused = false;
                        unusableRows = 0;
                        difficultyTimer = 0.0f;
                        for(int i=0; i<M; i++) for(int j=0; j<N; j++) gameGrid[i][j]=0;
                        currentState = PLAYING;
                    }
                    if (keyPressed->code == Keyboard::Key::Num2) { 
                        handleHighScores(0, highScores); 
                        currentState = HIGHSCORE; 
                    }
                    if (keyPressed->code == Keyboard::Key::Num3) currentState = HELP;
                    if (keyPressed->code == Keyboard::Key::Num4) window.close();
                    if (keyPressed->code == Keyboard::Key::Num5 && isPaused) {
                        isPaused = false; // Continue existing game
                    }
                } 
                else if (currentState == PLAYING && !game_over) {
                    if (keyPressed->code == Keyboard::Key::P) isPaused = true; 

                    if (!isPaused) {
                        if (keyPressed->scancode == sf::Keyboard::Scan::Up) rotate = true;
                        else if (keyPressed->scancode == sf::Keyboard::Scan::Left) delta_x = -1;
                        else if (keyPressed->scancode == sf::Keyboard::Scan::Right) delta_x = 1;
                    }
                }
                else if (currentState == HELP) {
                    // Pressing Escape or any Menu number returns to Menu
                    if (keyPressed->code == Keyboard::Key::Escape || keyPressed->code == Keyboard::Key::Num4) {
                        currentState = MENU;
                    }
                }
            }
            ///*** END EVENT HANDLING HERE ***///
            ///////////////////////////////////////////
        }

        if (currentState == PLAYING && !game_over && !isPaused) {

            if (gameLevel == 2 && lastKnownLevel == 1) {
                levelTimer = 2.0f; 
                lastKnownLevel = 2;
            }
            if (levelTimer > 0) {
                levelTimer -= time; 
            }
            difficultyTimer += time;

            if (difficultyTimer >= 300.0f) { // 300 seconds = 5 minutes
                difficultyTimer = 0;
                if (unusableRows < M - 5) { // Ensure at least 5 rows remain playable
                    unusableRows++;
                    // Fill the new unusable row with a specific color (e.g., index 8 for grey)
                    for (int j = 0; j < N; j++) {
                        gameGrid[M - unusableRows][j] = 8; 
                    }
                }
            }
                    
            // Detect Down key separately 
            if (Keyboard::isKeyPressed(Keyboard::Key::Down))       // If DOWN key is held
                delay = 0.05f;                                     // Speed up falling
            else
                delay = 0.3f;                                      // Return to normal speed

            ///////////////////////////////////////////////
            ///*** START CALLING YOUR FUNCTIONS HERE ***///
            
            if (delta_x != 0) { moveHorizontal(delta_x); delta_x = 0; }
            if (rotate) { rotatePiece(); rotate = false; }
            if (fallingPiece(timer, delay)) {
                game_over = true;
                handleHighScores(score, highScores);
            }

            ///*** YOUR CALLING STATEMENTS END HERE ***///
            //////////////////////////////////////////////
        }

        window.clear(Color::Black);
        window.draw(background);

        // --- PRIORITY 1: HELP PAGE ---
        if (currentState == HELP) {
            uiText.setString("--- HOW TO PLAY ---");
            uiText.setFillColor(Color::Green);
            uiText.setPosition({40, 50});
            window.draw(uiText);

            sideText.setString(
                "UP ARROW:    Rotate Piece\n"
                "LEFT ARROW:  Move Left\n"
                "RIGHT ARROW: Move Right\n"
                "DOWN ARROW:  Soft Drop\n"
                "P KEY:       Pause Game\n"
                "ESC:         Return\n\n"
                "Goal: Clear lines to\n"
                "score points and level up!"
            );
            sideText.setFillColor(Color::Cyan);
            sideText.setPosition({30, 120}); 
            sideText.setCharacterSize(20);
            window.draw(sideText);

            Text returnText = uiText;
            returnText.setString("Press ESC to Return");
            returnText.setFillColor(Color::Green);
            returnText.setCharacterSize(18);
            returnText.setPosition({70, 420});
            window.draw(returnText);
        }
        
        // --- PRIORITY 2: HIGH SCORE PAGE ---
        else if (currentState == HIGHSCORE) {
            std::string hs = "TOP 10 SCORES:\n\n";
            if (highScores.empty()) {
                hs += "  No Scores Yet";
            } else {
                for(size_t i = 0; i < highScores.size(); i++) {
                    hs += std::to_string(i + 1) + ". " + std::to_string(highScores[i]) + "\n";
                }
            }
            hs += "\n\nPress ESC to Return";
            
            uiText.setString(hs);
            uiText.setFillColor(Color::Yellow);
            uiText.setCharacterSize(20); // Smaller to fit 10 lines
            uiText.setPosition({60, 50});
            window.draw(uiText);
        }

        // --- PRIORITY 3: GAME OVER (Must be checked before MENU/PAUSE) ---
        else if (game_over) {
            uiText.setString("   GAME OVER\n   Score: " + std::to_string(score) + "\n\nPress ESC");
            uiText.setFillColor(Color::Red);
            uiText.setPosition({60, 180});
            window.draw(uiText);
        }

        // --- PRIORITY 4: MENUS ---
        else if (currentState == MENU || isPaused) {
            std::string menuStr = "1. START NEW GAME\n2. HIGH SCORES\n3. HELP\n4. EXIT";
            if (isPaused) menuStr += "\n5. CONTINUE";
            
            uiText.setString(menuStr);
            uiText.setFillColor(Color::Cyan);
            uiText.setPosition({50, 150});
            window.draw(uiText);
        } 

        // --- PRIORITY 5: ACTIVE GAMEPLAY ---
        else if (currentState == PLAYING) {
            
            // Draw placed blocks
            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (gameGrid[i][j] == 0) continue;
                    sprite.setTextureRect(IntRect({gameGrid[i][j] * 18, 0}, {18, 18}));
                    sprite.setPosition({j * 18.f, i * 18.f});
                    sprite.move({28.f, 31.f});          // offset
                    window.draw(sprite);
                }
            }


            // Draw current falling piece
            for (int i = 0; i < 4; i++)
            {
                sprite.setTextureRect(IntRect({colorNum * 18, 0}, {18, 18}));
                sprite.setPosition({point_1[i][0] * 18.f, point_1[i][1] * 18.f});
                sprite.move({28.f, 31.f});
                window.draw(sprite);
            }

            // Calculate and Draw Ghost Position (Shadow)
            int ghost_points[4][2];
            getGhostPosition(ghost_points); 

            for (int i = 0; i < 4; i++) {
                sprite.setTextureRect(IntRect({colorNum * 18, 0}, {18, 18}));
                sprite.setColor(Color(255, 255, 255, 100)); // Semi-transparent
                sprite.setPosition({ghost_points[i][0] * 18.f, ghost_points[i][1] * 18.f});
                sprite.move({28.f, 31.f});
                window.draw(sprite);
            }

            // Draw UI overlays
            
            window.draw(frame);
            sideText.setString("SCORE:\n" + std::to_string(score) + "\n\nLEVEL:\n" + std::to_string(gameLevel));
            sideText.setPosition({240, 100});
            window.draw(sideText);

            // Draw "NEXT PIECE" Preview
            sideText.setString("NEXT:");
            sideText.setPosition({240, 250}); // Positioned below Level
            window.draw(sideText);

            for (int i = 0; i < 4; i++) {
                sprite.setTextureRect(IntRect({nextColorNum * 18, 0}, {18, 18}));
                sprite.setColor(Color(255, 255, 255, 255));
                
                // Calculate position: (nextShape blocks)
                float x = (BLOCKS[nextShape][i] % 2) * 18.f;
                float y = (BLOCKS[nextShape][i] / 2) * 18.f;
                
                sprite.setPosition({250 + x, 280 + y}); // Centered in the sidebar area
                window.draw(sprite);
            }


            //--- LEVEL 2 TEXT DRAW BLOCK ---
            if (gameLevel == 2 && levelTimer > 0) {
                Text levelText(font);
                levelText.setString("LEVEL 2\nFAST MODE");
                levelText.setCharacterSize(35);
                levelText.setFillColor(Color::Red);
                levelText.setStyle(Text::Bold);
                levelText.setOutlineColor(Color::White);
                levelText.setOutlineThickness(2);
                levelText.setPosition({50, 180}); 
                window.draw(levelText);
            }
        }

        window.display();
    }

    return 0;
}
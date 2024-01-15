#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cstdlib>
#include<fstream>

using namespace std;
using namespace sf;


void displayInstructions() {
    cout << "Instructions: ..." << endl;
    cout << "1:use arrow keys to move left and right" << endl;
    cout << "2:use space key to shoot" << endl;
    cout << "3:hit the space ship to win the game" << endl;
    cout << "4:press 3 in the main menu to find out teh creators of the game" << endl;
    cout << "press 4 in the main menu to exit the game" << endl;
}

void displayCredits() {
    cout << "Game created by..." << endl;
    cout << "1:Mustafa" << endl;
    cout << "2:Arham" << endl;
    cout << "3:Beluga" << endl;
    cout << "4:Victor Timely" << endl;
    cout << "5:Einstien" << endl;
}




bool collisionDetection(const RectangleShape& bullet, Sprite aliens[], int maxAliens, float alienWidth, float alienHeight) {
    Vector2f bulletPosition = bullet.getPosition();
    Vector2f bulletSize = bullet.getSize();

    for (int i = 0; i < maxAliens; ++i) {
        Vector2f alienPosition = aliens[i].getPosition();
        
        // Check for overlap
        bool overlap_X = bulletPosition.x + bulletSize.x > alienPosition.x && alienPosition.x + alienWidth > bulletPosition.x;
        bool overlap_Y = bulletPosition.y + bulletSize.y > alienPosition.y && alienPosition.y + alienHeight > bulletPosition.y;

        if (overlap_X && overlap_Y) {
            aliens[i].setPosition(-100, -100); // Move the hit alien off-screen
            return true;
        }
    }
    return false;
}

bool playerAlienCollision(const Sprite& player, Sprite aliens[], int maxAliens, float playerWidth, float playerHeight, float alienWidth, float alienHeight) {
    Vector2f playerPosition = player.getPosition();

    for (int i = 0; i < maxAliens; ++i) {
        Vector2f alienPosition = aliens[i].getPosition();

        // Check for overlap
        bool overlap_X = playerPosition.x + playerWidth > alienPosition.x && alienPosition.x + alienWidth > playerPosition.x;
        bool overlap_Y = playerPosition.y + playerHeight > alienPosition.y && alienPosition.y + alienHeight > playerPosition.y;

        if (overlap_X && overlap_Y) {
            aliens[i].setPosition(-100, -100); // here doing same moving alien the hit alien off-screen
            return true;
        }
    }
    return false;
}

int main() {

    int choice = 0;
    cout << "Game Menu:" << endl;
    cout << "1. Play Game" << endl;
    cout << "2. Instructions" << endl;
    cout << "3. Credits" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:

        goto gameplay;
    case 2:
        displayInstructions();
        system("pause");
        break;
    case 3:
        displayCredits();
        system("pause");
        break;
    case 4:
        cout << "Exiting game..." << endl;
        return 0;
        break;
    default:
        cout << "Invalid choice. Please try again." << endl;
        break;

    }

gameplay:



    RenderWindow window(VideoMode(800, 600), "Shooting Game");


    //music
    Music music;
    if (!music.openFromFile("gameMusic.ogg")) {
        cout << "music file not open";
    }
    music.play();
    Music boom;
    if (!boom.openFromFile("boom.mp3")) {
        cout << "music file not open";
    }

    Music lastBoom;
    if (!lastBoom.openFromFile("lastboom.mp3")) {
        cout << "music file not open";
    }



    //background

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Background.jpg")) {
        cout << "Error, problem with backgroundimage image" << endl;
        return -1;
    }
    Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    //--------------Player creation----------------
    Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png")) {
        cout << "Problem With opening the player.png file";
    }
    Sprite player;
    player.setTexture(playerTexture);
    player.setScale(0.16f, 0.16f);
    player.setPosition(375, 500);
    float playerWidth = playerTexture.getSize().x * 0.16f; 
    float playerHeight = playerTexture.getSize().y * 0.16f; 


    //---------------------------------------------

    //------------Alien Creation-------------------
    const int maxAliens = 100;
    Texture alienTexture;
    if (!alienTexture.loadFromFile("alien.png")) {
        cout << "Problem with alien.png File";
    }

    Sprite aliens[maxAliens];
    for (int i = 0; i < maxAliens; ++i) {
        aliens[i].setTexture(alienTexture);
        aliens[i].setScale(0.16f, 0.16f);
        aliens[i].setPosition(-100, -100);
    }


    float alienWidth = alienTexture.getSize().x * 0.16f; 
    float alienHeight = alienTexture.getSize().y * 0.16f;




    //----------------------------------------------
    // 
    //-----------Bullet Creation ------------------
    // 
    //B1
    RectangleShape bullet(Vector2f(5, 20));
    bullet.setFillColor(Color::Red);
    bullet.setPosition(player.getPosition().x + 22.5, player.getPosition().y);
    //B2
    RectangleShape bullet2(Vector2f(5, 20));
    bullet2.setFillColor(Color::Red);
    bullet2.setPosition(player.getPosition().x + 55, player.getPosition().y);


    //-----------Adding font------------------
    Font font;
    if (!font.loadFromFile("font.ttf")) {
        cout << "Problem with font.tft file";

        return 0;
    }

    //----Giving font position and Color to Highscor
    Text highScoreText;
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(13);
    highScoreText.setFillColor(Color::White);
    highScoreText.setPosition(630, 66);


    //---Giving Font size color and position for lives---
    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(13);
    livesText.setFillColor(Color::White);
    livesText.setPosition(630, 8);

    //---Giving Font size color and position for levels---
    Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(13);
    levelText.setFillColor(Color::White);
    levelText.setPosition(630, 25);


    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(13);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(630, 46);
    //-------------------------------------------------------------------------------------------------------------------------------------

    //---------Some-Initializations--------------------
    bool shooting = false;
    float spawntimer = 0.0f;
    int playerLives = 4;
    int currentLevel = 1;
    int aliensKilled = 0;
    const int aliensToKill = 5;
    const int maxLevel = 6;
    int score = 0;
    int HighScore;
    

    

    //---------Our Main Game Loop-------------------------------
    while (window.isOpen()) {
       
        //FileHandling   Reading

        ifstream readFile;
        readFile.open("highScore.txt");
        if (readFile.is_open()) {
            readFile >> HighScore;
        }
        else { cout << "problem in reading highScore File"; }


        //event handling
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        //bg
        window.clear();
        window.draw(backgroundSprite);

        //text
        highScoreText.setString("HighScore: " + to_string(HighScore));
        livesText.setString("Lives: " + to_string(playerLives));
        levelText.setString("Level: " + to_string(currentLevel));
        scoreText.setString("score: " + to_string(score));
        //------------------------------------------------------------------

        //----------------Defining All key presses------------------------
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (player.getPosition().x > 0) {
                player.move(-4, 0);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (player.getPosition().x < window.getSize().x - player.getGlobalBounds().width) {
                player.move(4, 0);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Space) && !shooting) {
            shooting = true;
            bullet.setPosition(player.getPosition().x + 22.5, player.getPosition().y);//bulletOne
            bullet2.setPosition(player.getPosition().x + 55, player.getPosition().y);//BulletTwo

        }
        if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
            window.close();
        }
        //---------------------------------------------------------------------------------------------
        //-------------------Bullet Movement-----------------------

        if (shooting) {
            bullet.move(0, -10);
            bullet2.move(0, -10);
            //when bullet reach out of main window
            if (bullet.getPosition().y < 0 || bullet2.getPosition().y < 0) {
                shooting = false;
            }
            else {
                //when bullet collide with alien
                if (collisionDetection(bullet, aliens, maxAliens, alienWidth, alienHeight) || collisionDetection(bullet2, aliens, maxAliens, alienWidth, alienHeight)) {
                    shooting = false;
                    bullet.setPosition(player.getPosition().x + 22.5, player.getPosition().y);
                    bullet2.setPosition(player.getPosition().x + 22.5, player.getPosition().y);
                    aliensKilled++;
                    score = score + 1;
                    boom.play();
                    if (score > HighScore)
                    {
                        HighScore = score;
                    }
                    if (aliensKilled >= aliensToKill && currentLevel < maxLevel) {
                        currentLevel++;
                        aliensKilled = 0;
                        if (currentLevel == maxLevel) {
                            cout << "You Won, You saved the Earth";
                            window.close();
                        }
                    }
                }
            }
        }

        //----------------increasing alien speed at every level(as current level inc allien speed also will inc)----------

        float alienSpeed = 0.8 + 0.5 * (currentLevel - 1);
        cout << alienSpeed << endl;
        for (int i = 0; i < maxAliens; ++i) {
            aliens[i].move(0, alienSpeed);
            if (aliens[i].getPosition().y > window.getSize().y) {
                // cout << aliens[i].getPosition().y;
                aliens[i].setPosition(-100, -100);
            }
        }

        ////--------------------------------------------------------------------------------------------------------

                //alien hit player----also dec live-
        if (playerAlienCollision(player, aliens, maxAliens, playerWidth, playerHeight, alienWidth, alienHeight)) {
            playerLives--;
            lastBoom.play();
            if (playerLives <= 0) {
                lastBoom.play();
                cout << "You lost! Try again to save the earth....." << endl;
                window.close();
            }
        }
        //-----------------------------------------------------------------------------------------------------------


        spawntimer = spawntimer + 0.1f;
        if (spawntimer >= 30.0f) {
            spawntimer = 0.0f;
            for (int i = 0; i < maxAliens; ++i) {
                if (aliens[i].getPosition().y < 0) {

                    aliens[i].setPosition(rand() % (window.getSize().x - 50), 50); ///rand() % (window.getSize().x - 50) generates a random x-coordinate within the width of the window
                    break;
                }
            }
        }

        //-------------------------------------------------------

        //writing
        ofstream highScoreOutput;
        highScoreOutput.open("highScore.txt");
        if (highScoreOutput.is_open()) {
            highScoreOutput << HighScore;
            highScoreOutput.close();
        }
        else {
            cout << "Unable to open file for writing." << endl;
        }




        



        //-------------Rendering----------------------
        //window.clear();
        window.draw(player);
        for (int i = 0; i < maxAliens; i++) {
            window.draw(aliens[i]);
        }
        if (shooting)
            window.draw(bullet);
        if (shooting)
            window.draw(bullet2);
        window.draw(highScoreText);
        window.draw(livesText);
        window.draw(levelText);
        window.draw(scoreText);
        window.display();
    }
  


    return 0;
}

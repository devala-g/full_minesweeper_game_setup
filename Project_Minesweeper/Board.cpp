#include "Board.h"

Board::Board(int windowWidth, int windowHeight, int numMines) {
    happyFace.setTexture(TextureManager::GetTexture("face_happy"));
    happyFace.setPosition(((float)windowWidth / 2.f) - 32.f, (float)windowHeight - 88.f);

    loseFace.setTexture(TextureManager::GetTexture("face_lose"));
    loseFace.setPosition(happyFace.getPosition());

    winFace.setTexture(TextureManager::GetTexture("face_win"));
    winFace.setPosition(happyFace.getPosition());

    debugger.setTexture(TextureManager::GetTexture("debug"));
    debugger.setPosition(happyFace.getPosition().x + 128, happyFace.getPosition().y); //Same y coordinates as happyFace but 128 pix to the right.

    test1.setTexture(TextureManager::GetTexture("test_1"));
    test1.setPosition(debugger.getPosition().x + 64, happyFace.getPosition().y);

    test2.setTexture(TextureManager::GetTexture("test_2"));
    test2.setPosition(test1.getPosition().x + 64, happyFace.getPosition().y);

    test3.setTexture(TextureManager::GetTexture("test_3"));
    test3.setPosition(test2.getPosition().x + 64, happyFace.getPosition().y);

    digits.setTexture(TextureManager::GetTexture("digits"));

    mineCounter = numMines;
}

void Board::Draw(RenderWindow& window) {

    if (win)
        mineCounter = 0;

    //1. display Mine Counter
    if (mineCounter < 0) {
        digits.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
        digits.setPosition(0.f,happyFace.getPosition().y);
        window.draw(digits);
    }

    int firstDigit = abs(mineCounter) / 100;
    int sndDigit = (abs(mineCounter) % 100) /10;
    int trdDigit = abs(mineCounter) % 10;

    digits.setTextureRect(sf::IntRect(21 * firstDigit, 0, 21, 32));
    digits.setPosition(21.f,happyFace.getPosition().y);
    window.draw(digits);

    digits.setTextureRect(sf::IntRect(21 * sndDigit, 0, 21, 32));
    digits.setPosition(42.f,happyFace.getPosition().y);
    window.draw(digits);

    digits.setTextureRect(sf::IntRect(21 * trdDigit, 0, 21, 32));
    digits.setPosition(63.f,happyFace.getPosition().y);
    window.draw(digits);

    //2. Display buttons:
    window.draw(happyFace);

    if (lost)//draw win or lose faces over happy face in case these conditions are met.
        window.draw(loseFace);
    else if(win)
        window.draw(winFace);

    window.draw(debugger);
    window.draw(test1);
    window.draw(test2);
    window.draw(test3);
}
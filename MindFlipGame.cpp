#include "Game.h"
#include "Card.h"
#include "HighScoreManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Memory Match Game");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font! Make sure arial.ttf is in the same directory.\n";
        return EXIT_FAILURE;
    }

    // UI Elements
    sf::Text titleText("Memory Match", font, 36);
    titleText.setPosition(300, 10);
    titleText.setFillColor(sf::Color(255, 105, 180));  // Hot Pink
    titleText.setStyle(sf::Text::Bold);

    sf::Text movesText("Moves: 0", font, 24);
    movesText.setPosition(20, 20);
    movesText.setFillColor(sf::Color(255, 105, 180));  

    // Undo/Redo buttons
    sf::RectangleShape undoButton(sf::Vector2f(120, 40));
    undoButton.setPosition(650, 20);
    undoButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text undoText("Undo (Ctrl+Z)", font, 18);
    undoText.setPosition(655, 25);
    undoText.setFillColor(sf::Color::White);

    sf::RectangleShape redoButton(sf::Vector2f(120, 40));
    redoButton.setPosition(650, 70);
    redoButton.setFillColor(sf::Color(100, 100, 200));

    sf::Text redoText("Redo (Ctrl+Y)", font, 18);
    redoText.setPosition(655, 75);
    redoText.setFillColor(sf::Color::White);

    // Player name input
    sf::Text namePrompt("Enter your name:", font, 24);
    namePrompt.setPosition(300, 250);
    namePrompt.setFillColor(sf::Color(255, 105, 180));

    std::string playerName;
    sf::Text nameText("", font, 24);
    nameText.setPosition(300, 280);
    nameText.setFillColor(sf::Color(255, 105, 180));

    // Game state
    bool nameEntered = false;
    bool gameFinished = false;
    const int gridSize = 4;
    Game game(gridSize);
    std::vector<std::vector<Card>> cards;

    sf::Text resultText("", font, 32);
    resultText.setPosition(100, 250);
    resultText.setFillColor(sf::Color(102, 0, 153));

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!nameEntered) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b' && !playerName.empty()) {
                        playerName.pop_back();
                    }
                    else if (event.text.unicode == '\r' && !playerName.empty()) {
                        nameEntered = true;
                        // Initialize game after name entry
                        float cardSize = 90.f;
                        float padding = 15.f;
                        float startX = (800.f - (gridSize * (cardSize + padding))) / 2.f;
                        float startY = 150.f;

                        for (int i = 0; i < gridSize; i++) {
                            std::vector<Card> row;
                            for (int j = 0; j < gridSize; j++) {
                                row.emplace_back(
                                    startX + j * (cardSize + padding),
                                    startY + i * (cardSize + padding),
                                    game.getBoardSymbol(i, j),
                                    font
                                );
                            }
                            cards.push_back(std::move(row));
                        }
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != '\r') {
                        playerName += static_cast<char>(event.text.unicode);
                    }
                    nameText.setString(playerName);
                }
            }
            else if (!gameFinished) {
                // Handle keyboard shortcuts
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.control && event.key.code == sf::Keyboard::Z) {
                        game.undo();
                    }
                    else if (event.key.control && event.key.code == sf::Keyboard::Y) {
                        game.redo();
                    }
                }
                // Handle mouse clicks
                else if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(
                        { event.mouseButton.x, event.mouseButton.y });

                    // Check button clicks first
                    if (undoButton.getGlobalBounds().contains(mousePos)) {
                        game.undo();
                    }
                    else if (redoButton.getGlobalBounds().contains(mousePos)) {
                        game.redo();
                    }
                    else {
                        // Card clicks
                        for (int i = 0; i < gridSize; i++) {
                            for (int j = 0; j < gridSize; j++) {
                                if (cards[i][j].contains(mousePos)) {
                                    game.handleCardClick(i, j);
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(255, 209, 220));  // Baby Pink 

        if (!nameEntered) {
            window.draw(namePrompt);
            window.draw(nameText);
        }
        else if (gameFinished) {
            window.draw(resultText);
        }
        else {
            // Update moves display
            std::string movesStr = "Moves: " + std::to_string(game.getMoves());
            movesText.setString(movesStr);

            // Draw UI elements
            window.draw(titleText);
            window.draw(movesText);
            window.draw(undoButton);
            window.draw(undoText);
            window.draw(redoButton);
            window.draw(redoText);

            game.update();

            // Draw cards
            for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < gridSize; j++) {
                    if (game.isRevealed(i, j)) {
                        cards[i][j].reveal();
                    }
                    else if (game.isTemporarilyFlipped(i, j)) {
                        cards[i][j].reveal();
                    }
                    else {
                        cards[i][j].hide();
                    }
                    cards[i][j].draw(window);
                }
            }

            // Check for game completion
            if (game.isFinished()) {
                gameFinished = true;
                HighScoreManager::saveUserHighScore(playerName, game.getMoves());

                std::ostringstream oss;
                oss << "Congratulations " << playerName << "!\n"
                    << "You completed the game in " << game.getMoves() << " moves.\n";

                int bestScore = HighScoreManager::loadUserHighScore(playerName);
                if (bestScore != HighScoreManager::VERY_LARGE_NUMBER) {
                    oss << "Your best: " << bestScore << " moves";
                }

                resultText.setString(oss.str());
            }
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
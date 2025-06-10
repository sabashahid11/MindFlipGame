#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <SFML/System.hpp>
#include <stack>

class Game {
public:
    Game(int gridSize);

    void initializeBoard();
    void displayBoard();
    bool flipCards(int row1, int col1, int row2, int col2);
    bool isFinished() const;
    std::string getBoardSymbol(int row, int col) const;
    int getSize() const;
    bool isRevealed(int row, int col) const;
    bool isTemporarilyFlipped(int row, int col) const;
    void handleCardClick(int row, int col);
    void update();
    int getMoves() const { return moves; }

    // Undo/Redo functionality
    bool undo();
    bool redo();
    void saveState();
    void clearRedoStack();

private:
    void resetTemporarilyFlipped();

    struct GameState {
        std::vector<std::vector<bool>> revealed;
        std::vector<std::vector<bool>> temporarilyFlipped;
        sf::Vector2i firstFlippedCard;
        int moves;
    };

    // Game state
    int size;
    int moves;
    bool isWaiting;
    bool isUndoRedoOperation;  // Flag to prevent saving during undo/redo
    sf::Vector2i firstFlippedCard;
    sf::Clock flipClock;
    std::vector<std::vector<std::string>> board;
    std::vector<std::vector<bool>> revealed;
    std::vector<std::vector<bool>> temporarilyFlipped;

    // Undo/Redo stacks
    std::stack<GameState> undoStack;
    std::stack<GameState> redoStack;
};

#endif // GAME_H
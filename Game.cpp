#include "Game.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>

Game::Game(int gridSize) :
    size(gridSize),
    moves(0),
    isWaiting(false),
    isUndoRedoOperation(false),
    firstFlippedCard(-1, -1)
{
    board.resize(size, std::vector<std::string>(size));
    revealed.resize(size, std::vector<bool>(size, false));
    temporarilyFlipped.resize(size, std::vector<bool>(size, false));
    initializeBoard();
}

void Game::initializeBoard() {
    std::vector<std::string> symbols;

    // Create symbol pairs
    int pairsNeeded = (size * size) / 2;
    for (char ch = 'A'; ch < 'A' + pairsNeeded; ch++) {
        symbols.push_back(std::string(1, ch));
        symbols.push_back(std::string(1, ch));
    }

    // Shuffle symbols
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    auto rng = std::default_random_engine(seed);
    std::shuffle(symbols.begin(), symbols.end(), rng);

    // Fill board
    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = symbols[k++];
        }
    }
}

void Game::displayBoard() {
    std::cout << "    ";
    for (int i = 0; i < size; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << std::setw(3) << i << " ";
        for (int j = 0; j < size; j++) {
            std::cout << std::setw(4) << (revealed[i][j] ? board[i][j] : "??");
        }
        std::cout << std::endl;
    }
}

bool Game::flipCards(int row1, int col1, int row2, int col2) {
    moves++;
    bool match = (board[row1][col1] == board[row2][col2]) &&
        !(row1 == row2 && col1 == col2);

    if (match) {
        revealed[row1][col1] = true;
        revealed[row2][col2] = true;
    }
    return match;
}

bool Game::isFinished() const {
    for (const auto& row : revealed) {
        for (bool card : row) {
            if (!card) return false;
        }
    }
    return true;
}

std::string Game::getBoardSymbol(int row, int col) const {
    return (row >= 0 && row < size && col >= 0 && col < size) ? board[row][col] : "";
}

int Game::getSize() const {
    return size;
}

bool Game::isRevealed(int row, int col) const {
    return (row >= 0 && row < size && col >= 0 && col < size) ? revealed[row][col] : false;
}

bool Game::isTemporarilyFlipped(int row, int col) const {
    return (row >= 0 && row < size && col >= 0 && col < size) ? temporarilyFlipped[row][col] : false;
}

void Game::handleCardClick(int row, int col) {
    if (isWaiting || revealed[row][col] ||
        row < 0 || row >= size || col < 0 || col >= size) {
        return;
    }

    saveState();

    if (firstFlippedCard == sf::Vector2i(-1, -1)) {
        firstFlippedCard = sf::Vector2i(row, col);
        temporarilyFlipped[row][col] = true;
    }
    else {
        temporarilyFlipped[row][col] = true;

        if (board[row][col] == board[firstFlippedCard.x][firstFlippedCard.y]) {
            revealed[row][col] = true;
            revealed[firstFlippedCard.x][firstFlippedCard.y] = true;
        }
        else {
            isWaiting = true;
            flipClock.restart();
        }
        firstFlippedCard = sf::Vector2i(-1, -1);
    }
    moves++;
}

void Game::update() {
    if (isWaiting && flipClock.getElapsedTime().asSeconds() > 1.0f) {
        saveState();
        resetTemporarilyFlipped();
        isWaiting = false;
    }
}

void Game::resetTemporarilyFlipped() {
    for (auto& row : temporarilyFlipped) {
        std::fill(row.begin(), row.end(), false);
    }
}

void Game::saveState() {
    if (isUndoRedoOperation) {
        isUndoRedoOperation = false;
        return;
    }

    clearRedoStack();

    GameState state;
    state.revealed = revealed;
    state.temporarilyFlipped = temporarilyFlipped;
    state.firstFlippedCard = firstFlippedCard;
    state.moves = moves;

    undoStack.push(state);
}

bool Game::undo() {
    if (undoStack.empty() || isWaiting) return false;

    GameState currentState;
    currentState.revealed = revealed;
    currentState.temporarilyFlipped = temporarilyFlipped;
    currentState.firstFlippedCard = firstFlippedCard;
    currentState.moves = moves;
    redoStack.push(currentState);

    GameState prevState = undoStack.top();
    undoStack.pop();

    revealed = prevState.revealed;
    temporarilyFlipped = prevState.temporarilyFlipped;
    firstFlippedCard = prevState.firstFlippedCard;
    moves = prevState.moves;

    isUndoRedoOperation = true;
    return true;
}

bool Game::redo() {
    if (redoStack.empty() || isWaiting) return false;

    GameState currentState;
    currentState.revealed = revealed;
    currentState.temporarilyFlipped = temporarilyFlipped;
    currentState.firstFlippedCard = firstFlippedCard;
    currentState.moves = moves;
    undoStack.push(currentState);

    GameState nextState = redoStack.top();
    redoStack.pop();

    revealed = nextState.revealed;
    temporarilyFlipped = nextState.temporarilyFlipped;
    firstFlippedCard = nextState.firstFlippedCard;
    moves = nextState.moves;

    isUndoRedoOperation = true;
    return true;
}

void Game::clearRedoStack() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}
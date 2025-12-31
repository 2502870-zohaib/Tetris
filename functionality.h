/*Muhammad Zohaib (2502870)
Muhammad Abdullah (2502946)*/

/* PROGRAMMING FUNDAMENTAL'S PROJECT FOR FALL 2022 BS(CS)
 * You need to define the required function in the part instructed here below.
 * Avoid making any unnecessary changes, particularly the ones you don't understand.
 * The different pieces should be defined using an array - uncomment the following code once you are done creating the array.
 * TIP: Understand thoroughly before getting started with coding.
 * */
#pragma once
#include <cstdlib>
#include <algorithm>
#include "pieces.h"
#include "utils.h"
#include <fstream>   
#include <vector>    

int colorNum = 1 + std::rand() % 7;      // Color of the piece currently falling
int nextColorNum = 1 + std::rand() % 7;  // Color prepared for the next piece
int score = 0; 
int linesClearedTotal = 0;
int gameLevel = 1;
int restrictedBlocks[] = {0, 1, 2, 3}; 
int nextShape = restrictedBlocks[std::rand() % 4]; // Store the index of the next shape
void clearLines();

//---Piece Starts to Fall When Game Starts---//
bool fallingPiece(float& timer, float& delay) {
    // Determine speed based on level
    float currentDelay = (gameLevel >= 2) ? (delay * 0.7f) : delay; 

    if (timer > currentDelay) {
        for (int i = 0; i < 4; i++) {
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];
            point_1[i][1] += 1;
        }

        if (!anamoly()) {
            for (int i = 0; i < 4; i++) 
                gameGrid[point_2[i][1]][point_2[i][0]] = colorNum;

            clearLines();

            colorNum = nextColorNum; 
            int currentShape = nextShape; // Take the shape previously determined as "next"
            nextColorNum = 1 + std::rand() % 7; 

            // --- LEVEL LOGIC FOR SPAWNING ---
            // Pick the NEW next shape for the future preview
            if (gameLevel == 1) {
                // Only use 4 specific blocks (e.g., indices 0, 1, 2, 3)
                nextShape = restrictedBlocks[std::rand() % 4];
            } else {
                // Level 2+: Use all 7 blocks
                nextShape = std::rand() % 7;
            }

            for (int i = 0; i < 4; i++) {
                point_1[i][0] = BLOCKS[currentShape][i] % 2 + N / 2 - 1;
                point_1[i][1] = BLOCKS[currentShape][i] / 2;
            }

            if (!anamoly()) return true; // Game Over
        }
        timer = 0;
    }
    return false;
}
/////////////////////////////////////////////
///*** START CODING YOUR FUNTIONS HERE ***///
void updateScore(int lines) {
    if (lines == 1) score += 10 * gameLevel;
    else if (lines == 2) score += 30 * gameLevel;
    else if (lines == 3) score += 60 * gameLevel;
    else if (lines == 4) score += 100 * gameLevel;
    linesClearedTotal += lines;
    if (linesClearedTotal >= 10) gameLevel = 2; // Level 2 logic
}

void clearLines() {
    int row = M - 1;
    int linesThisTurn = 0;
    for (int i = M - 1; i > 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (gameGrid[i][j] > 0) count++;
            gameGrid[row][j] = gameGrid[i][j];
        }
        if (count < N) row--; else linesThisTurn++;
    }
    if (linesThisTurn > 0) updateScore(linesThisTurn);
}
void moveHorizontal(int delta_x) {
    for (int i = 0; i < 4; i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
        point_1[i][0] += delta_x;
    }
    if (!anamoly()) for (int i = 0; i < 4; i++) point_1[i][0] = point_2[i][0];
}

void rotatePiece() {
    int center_x = point_1[1][0];
    int center_y = point_1[1][1];
    for (int i = 0; i < 4; i++) {
        point_2[i][0] = point_1[i][0];
        point_2[i][1] = point_1[i][1];
        int x = point_1[i][1] - center_y;
        int y = point_1[i][0] - center_x;
        point_1[i][0] = center_x - x;
        point_1[i][1] = center_y + y;
    }
    if (!anamoly()) {
        for (int i = 0; i < 4; i++) {
            point_1[i][0] = point_2[i][0];
            point_1[i][1] = point_2[i][1];
        }
    }
}
void handleHighScores(int currentScore, std::vector<int>& scores) {
    std::ifstream inFile("highscores.txt");
    int s; 
    scores.clear();
    if (inFile.is_open()) {
        while (inFile >> s) scores.push_back(s);
        inFile.close();
    }
    if (currentScore > 0) {
        scores.push_back(currentScore);
        std::sort(scores.rbegin(), scores.rend());
        if (scores.size() > 10) scores.resize(10);
        std::ofstream outFile("highscores.txt");
        for (int sc : scores) outFile << sc << "\n";
        outFile.close();
    }
}
void getGhostPosition(int ghost_points[4][2]) {
    // 1. Copy current piece coordinates to ghost
    for (int i = 0; i < 4; i++) {
        ghost_points[i][0] = point_1[i][0];
        ghost_points[i][1] = point_1[i][1];
    }

    // 2. Move ghost down until it hits something
    bool collision = false;
    while (!collision) {
        for (int i = 0; i < 4; i++) ghost_points[i][1]++; // Move down
        
        // Check for collision (anamoly check for ghost)
        for (int i = 0; i < 4; i++) {
            if (ghost_points[i][0] < 0 || ghost_points[i][0] >= N || 
            ghost_points[i][1] >= (M - unusableRows) || gameGrid[ghost_points[i][1]][ghost_points[i][0]]) {
            collision = true;
            }
        }
        
        if (collision) {
            for (int i = 0; i < 4; i++) ghost_points[i][1]--; // Move back up one step
        }
    }
}

///*** YOUR FUNCTIONS END HERE ***///
/////////////////////////////////////
Chess AI Design
==

## Objective

The objective of the Chess AI is to do its best to beat the other player (whether human or not). However, we don't have time to design the best Chess AI, so the goal is to make a nominally challenging opponent that a human player can play and have a certain amount of difficulty beating. The AI should make obvious moves, avoid obvious blunders, and overall be able to hold its own, and ideally not be predictable. If it can do those things, then it will be fun to play against and our goal will have been achieved.

## Design

The main idea behind the AI is that it is given the current state of the board, then it computes all possible moves it can make, then it tries each move in succession and calculates the score of that position in its favor.
The score represents how good that position is for its color. That score also represents the value of that move, aka it's the score of the move.
It then compares each move's score and picks the highest one and makes that move.

Scoring is the core component that determines what move the AI will make. In order for the AI to make a good move, it must be able to score a board well. In order to score a board well, the scorer must be able to determine what are good positions and what are bad positions, and rate them appropriately.

## Scoring

### Objectives

1. Piece Development
   1. Encourage the Player to develop his minor pieces
   2. Encourage the Player to castle
   3. Encourage the Player to develop his queen
   4. Encourage the Player to connect his rooks
2. Center Control
   1. Encourage the Player to control the center of the board
3. Attack/Defend Pieces
   1. Encourage the Player to attack & take weak pieces
   2. Encourage the Player to defend & preserve its own pieces
4. Checkmate
   1. Encourage the Player to checkmate the king
   2. Encourage the Player to protect its king from checkmate 

### Methods

The Player doesn't have infinite time, and it also doesn't need to make the best move, it just needs to make a good move. 
The best move is ideal but often hard to find. In order to compensate for this, there must be two ways of computing score, a fast and dirty method, and a more accurate but slow method.
That way we can appropriately mix these methods to meet our constraints.

For each objective, we can design a separate scoring algorithm to assess the current position based on this particular objective. Ideally, we would like a gradient of algorithms for each objective that allow us to pick between a faster method or a more accurate method.

Once we have evaluated each objective, we will need to combine each score into one, as an aggregate score. This is where weights for each score will come in. Certain objectives can be prioritized by the weight put on them. An example could be to prioritize piece development in the early to middle game over achieving a checkmate.

1. Piece Development
   1. Fast
      1. 
         1. Check if minor pieces are developed, if so, then increase score.
         2. Check if king is castled, if so, increase score.
         3. Check if rooks are connected, if so, increase score.
2. Center Control
   1. Fast
      1. Count the number of our color's pieces in the center or nearby the center (ie a 4x4 square).
      2. Look at each of our pieces, and calculate their distance from the center, if they are closer, give a higher score, if they are farther, a lower score.
   2. Accurate
      1. For each of our pieces, calculate all their possible moves, for each of those moves, see if they attack or move the center position.
      2. For each of our pieces, see if they can make a legal move to the center position, this will mean that they are either attacking or can occupy the center.
3. Attack/Defend Pieces
   1. Fast
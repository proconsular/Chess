# Chess
### Overview

A working-in-progress chess app I'm working on currently. It's written in C++ w/ SFML.
At the moment, you can play as White against Black. The AI is still a work-in-progress since I'll still figuring out how to get it to work the best.

### Goal

My goal is to first create an MVP, which I would define as the app having a basic UI that would allow you to pick which side you want.
Then you can play and the AI is pretty challenging, with potentially a means of adjusting its difficulty before you play.

### Media

Here is a video of some gameplay:
https://www.youtube.com/watch?v=q2YW3cY-vpY

### Progress

The first big obstacle technically was writing a thorough algorithm to check whether a move was legal or not. Since the player has to be able to drag a piece and therefore select a move, the first priority was a means to verify if whatever move they made was valid. For instance, a pawn can't move 3 squares, but a player could try to make that move. This took a little while but was straight forward and there are many unit tests to verify many different cases in order to ensure the is_move_legal algorithm works.

The next and probably the more challenging obstacle is to write an AI that can present a reasonable challenge to the player. Writing a chess AI isn't easy, and the objective isn't to write my own Stockfish, but if I can make an AI that can at least not make obvious blunders and take advantage if the player makes obvious blunders then I think that would be sufficient for the MVP. Currently, the algorithm can attack and defend reasonably well, but can't do any planning, and is poor at checkmating.

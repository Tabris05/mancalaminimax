# Mancala Minimax
A command line mancala game for 0, 1, or 2 players written in C++23. When less than two human players are present, the game employs an AI opponent that makes moves according to a minimax search of the decision tree with alpha-beta pruning. Currently, the search depth is hardcoded at 10 levels and the game state evaluation heuristic is simply the difference in score between the two players at that state. 

## How to play
This program implements the Empty Capture variant of the Kalah rule set. Player 1 controls the southern pits and scores in the east store and player 2 controls the northern pits and scores in the west store. For each player, pits are numbered 1-6 where 1 is the furthest pit from that player's store and 6 is the closest pit to that player's store.

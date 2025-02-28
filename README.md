This is a terminal-based 2048 game written in C, featuring a full-screen interactive interface. The game starts with a menu, where players can navigate using arrow keys and select options with Enter.

Menu Options:
New Game – Start a new game after entering the player's name.
Resume – Continue the last game (if available).
Top Players – View a leaderboard of up to 30 players, sorted by score.
Quit – Exit the game.

Gameplay:
The board starts with two random tiles.
Players move tiles using W, A, S, D keys.
Score and high score are displayed on the right, while valid moves and date/time appear on the left.
If no move is made for 10 seconds, the best possible move is executed automatically.
The game ends when 2048 is reached (win) or no valid moves are left (loss).

Game Over & Resume:
If the game is lost, a special screen is displayed.
If no previous game exists, selecting Resume will notify the player.
The leaderboard stores up to 30 players, replacing the oldest if full.
This C implementation offers a smooth and interactive 2048 experience with an automated best-move feature, ensuring an engaging challenge for players.

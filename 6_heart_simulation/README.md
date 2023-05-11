# Hearts

A Python implementation of Hearts. This project is a work in progress.

# Objective
Win all the hearts and the queen of spades, or none of these cards. Get as few points as possbile.

# Rules
* Each of the four player is dealt 13 cards.

* Before the round starts, all four players pass three cards, left, right, across, or no pass: the game cycles through these four instances with the passing of rounds.

* The player with the two of clubs leads the first hand.

* A player must play a card with the suit of the card that lead the trick if he or she has one, otherwise any card may be played.

* The player with the high ranked card of the suit that lead wins the trick (aces are high).

* A player may not play hearts or the queen of spades on the first hand.

* After all the cards have been played, points are tallied for each player.

* A player receives one point per heart and 13 points for the queen of spades.

* If a player has all of the points, he or she has "shot the moon" and all other players get 26 points (not yet implemented).

* The game ends when the first player gets to 100. The winner is the player with the fewest points.

# Implementation
The game can be played manually or simulated by the computer. This mode can be toggled at the top of the `Hearts.py` file by changing the variable `auto` between `True` and `False`. The computer will play until one player's score reaches 100 and then the game ends with the final score display. The computer uses a "guess and check" method to play the game. It will attempt to play a card from the current player's hand at random. If the play is valid, the game moves on to the next player. If the play is invalid, the computer is forced to try again, until it makes a valid play. Passing is disabled for the computer simulation.

##Simulation
This Hearts simulation used the original hearts game implemented by Daniel Corin in https://github.com/danielcorin/Hearts. The objective of the test was to find out all of the factors that go into winning a Hearts game by calculating how the winning rate changes when each factor is changed. The BetterPlayer is a player that is able to pass according to the hiearchy specified by the user. Each test, from passTest to fourthTest is able to test what occurs when only the hiearchy upon which the player follows for passing to when they are the fourth player to play a card is changed. The test is able to test all the factors at once.

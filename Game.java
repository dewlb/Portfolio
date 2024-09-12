/* Ethan Jewell
 */
import java.util.Random;

public class Game {
	
	// Our variables we will need to store including our board
	int board[][] = new int[8][8];
	char moves[] = new char[] {'d', 'r', 'b'};
	Random behavior;

	// Our constructor including the random seed
	public Game(Random rand) {
		behavior = rand;
	}
	
	// Select the move for player two based on random selector
	public char selectPlayerTwoMove() {
		int choice = behavior.nextInt() % 3;
		
		// Account for negative random number
		if (choice < 0) {
			choice = choice * -1;
		}
		
		// Return selected choice
		return moves[choice];
	}
	
	// Simulate a game being played
	public int play() {
		
		// Set up our tracker for the knight piece
		int current[] = new int[2];
		current[0] = 0;
		current[1] = 0;
		
		// Place knight on board
		board[current[0]][current[1]] = 1;

		// Loop for players to make their moves
		while (current[0] != 7 || current[1] != 7) {
			
			// Remove knight temporarily
			board[current[0]][current[1]] = 0;
			
			// Player 1 algorithm, created spaces to avoid in an excel sheet and coded to not land on them, in case AI stalls, still wins
			if (current[0] + 1 > 3 && current[1] + 1 == 4) {
				current[1] = current[1] + 1;
			} else if (current[0] + 1 > 3 && current[1] + 1 == 6) {
				current[0] = current[0] + 1;
			} else if (current[0] + 1 == 4 && current[1] + 1 > 3) {
				current[0] = current[0] + 1;
			} else if (current[0] + 1 == 6 && current[1] + 1 > 3) {
				current[1] = current[1] + 1;
			} else if (current[0] + 1 > 7) {
				current[1] = current[1] + 1;
			} else if (current[1] + 1 > 7) {
				current[0] = current[0] + 1;
			} else {
				current[0] = current[0] + 1;
				current[1] = current[1] + 1;
			}
			
			// Place knight back with new move
			board[current[0]][current[1]] = 1;
			
			// Player 1 check for win early
			if (current[0] == 7 && current[1] == 7) {
				return 1;
			}
			
			// Remove knight temporarily
			board[current[0]][current[1]] = 0;
			
			// Player 2
			char pick = selectPlayerTwoMove();
			if (pick == 'd' && current[1] < 7) {
				current[1] = current[1] + 1;
			} else if (pick == 'r' && current[0] < 7) {
				current[0] = current[0] + 1;
			} else if (pick == 'd' && current[0] < 7 && current[1] < 7) {
				current[0] = current[0] + 1;
				current[1] = current[1] + 1;
			} else if (current[0] < 7) {
				current[0] = current[0] + 1;
			} else if (current[1] < 7) {
				current[1] = current[1] + 1;
			}
			
			// Place knight back with new move
			board[current[0]][current[1]] = 1;
			
		}
		
		return 2;
	}
}

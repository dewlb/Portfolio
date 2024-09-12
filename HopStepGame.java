/* Ethan Jewell
 */

public class HopStepGame {
	
	int memo[];

	// Top down, O(2^n) approach with recursion
	public int minCost(int[] steps, int size) {
		
		// Account for array of size 1
		if (steps.length == 1) {
			return steps[0];
		}
		
		// Account for possible starting position and run recursion
		return minCostR(steps, size);
	}
	
	// Actual recursion method
	public int minCostR(int[] steps, int size) {
		
		// Base case
		if (size == 0 || size == 1) {
			return steps[size];
		}
		
		// Return min of either possible move
		return steps[size] + Math.min(minCostR(steps, size - 1), minCostR(steps, size - 2));
	}
	
	// Top down, O(n) approach with memoization
	public int minCostMemoization(int[] steps, int size, int[] memo) {
		
			// Account for array of size 1
			if (steps.length == 1) {
				return steps[0];
			}
				
			// Account for possible starting position
			return minCostMemoizationR(steps, size, memo);
	}
	
	public int minCostMemoizationR(int[] steps, int size, int[] memo) {
		
		// Base case
		if (size == 0 || size == 1) {
			return steps[size];
		}
		
		// Memoization addition
		if (memo[size] > 0) {
			return memo[size];
		}
				
		// Return min of either possible move and add to memoization
		memo[size] = steps[size] + Math.min(minCostMemoizationR(steps, size - 1, memo), minCostMemoizationR(steps, size - 2, memo));
		return memo[size];
	}
	
	// Bottom up, O(n) approach with tabulation
	public int minCostTabulation(int[] steps) {
				
		// Account for array of size 1
		if (steps.length == 1) {
			return steps[0];
		}
		
		// Set up tabulation table
		int tab[] = new int[steps.length];
		
		// Fill in with values
		tab[0] = steps[0];
		tab[1] = steps[1];
		for (int i = 2; i < steps.length; i++) {
			if (tab[i - 1] < tab[i - 2]) {
				tab[i] = tab[i - 1] + steps[i];
			} else {
				tab[i] = tab[i - 2] + steps[i];
			}
		}
		
		// Return final value of tabulation array, of the two possibilities
		return Math.min(tab[steps.length - 1], tab[steps.length - 2]);
		
	}
}

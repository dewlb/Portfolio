/* Ethan Jewell
 */

import java.io.File;
import java.util.*;

public class MagicMaze {
	
	// Desired variables
	char maze[][];
	String mazeNumber;
	int mazeNumTracker = 0;
	String fileName;
	int rows;
	int cols;
	HashMap<Integer, Integer[]> portalMap = new HashMap<Integer, Integer[]>();
	Integer[] tempCoordinates = new Integer[4];
	
	// Class constructor for each maze
	public MagicMaze(String name, int rowNum, int colNum) {
		fileName = name;
		rows = rowNum;
		cols = colNum;
		mazeNumTracker++;
		mazeNumber = "maze" + mazeNumTracker;
		scanMaze();
	}
	
	// Maze scan method
	public void scanMaze() {
		
		// Attempt to set up file and scanner
		File file = new File(fileName);
		Scanner myScan = null;
		try {
			myScan = new Scanner(file);
		} catch (Exception e) {
			System.out.println("File Not Found...");
		}
		
		// If success, we then scan our maze into our 2D array
		maze = new char[rows][cols];
		for (int row = 0; row < rows; row++) {
			if (myScan.hasNext() != true) {
				return;
			}
			String currentRow = myScan.next();
			for (int col = 0; col < cols; col++) {
				char curRead = currentRow.charAt(col);
				maze[row][col] = curRead;
				// If the read in value is a number, or a portal in this case, we can add the coordinates to our hashmap using the portal number as the key
				if ((int) curRead >= 48 && (int) curRead <= 57) {
					// This if is for when the key already exists, we just take it and add on to the end with our new values
					if (portalMap.containsKey((int) curRead - 48)) {
						Integer[] tempCoordinates = new Integer[4];
						tempCoordinates = portalMap.get((int) curRead - 48);
						tempCoordinates[2] = row;
						tempCoordinates[3] = col;
						portalMap.put((int) curRead - 48, tempCoordinates);
					// This else is for if the key doesn't exist so we add x1 and y1 to the first two values
					} else {
						Integer[] tempCoordinates = new Integer[4];
						tempCoordinates[0] = row;
						tempCoordinates[1] = col;
						portalMap.put((int) curRead - 48, tempCoordinates);
					}
				}
			}
		}
		
		// Close scanner when finished
		myScan.close();
	}
	
	// Wrapper function
	public boolean solveMagicMaze() {
		return solveMagicMazeR(rows - 1, 0);
	}
	
	// Recursive funtion to solve or not solve the magic maze
	public boolean solveMagicMazeR(int curRow, int curCol) {
		
		// Win condition
		if (maze[curRow][curCol] == 'X') {
			return true;
		}

		// Check for portal landed on, and preemptively change the landing zone to 'checked' so that we don't immediately teleport back
		// We also check and make sure we are using the correct set of coordinates to teleport and act based on that
		if ((int) maze[curRow][curCol] >= 48 && (int) maze[curRow][curCol] <= 57) {
			tempCoordinates = portalMap.get((int) maze[curRow][curCol] - 48);
			if (tempCoordinates[0] == curRow && tempCoordinates[1] == curCol) {
				maze[tempCoordinates[2]][tempCoordinates[3]] = '@';
				if (solveMagicMazeR(tempCoordinates[2], tempCoordinates[3])) {
					return true;
				}
			} else {
				maze[tempCoordinates[0]][tempCoordinates[1]] = '@';
				if (solveMagicMazeR(tempCoordinates[0], tempCoordinates[1])) {
					return true;
				}
			}
		}
				
		// Mark spot as visited by making it unavailable
		maze[curRow][curCol] = '@';
		
		// Backtracking process
		
		// Check and go right
		if (curCol + 1 < cols && maze[curRow][curCol + 1] != '@') {
			if (solveMagicMazeR(curRow, curCol + 1)) {
				return true;
			}
		}
		
		// Check and go up
		if (curRow - 1 >= 0 && maze[curRow - 1][curCol] != '@') {
			if (solveMagicMazeR(curRow - 1, curCol)) {
				return true;
			}
		}
		
		// Check and go down
		if (curRow + 1 < rows && maze[curRow + 1][curCol] != '@') {
			if (solveMagicMazeR(curRow + 1, curCol)) {
				return true;
			}
		}
		
		// Check and go left
		if (curCol - 1 >= 0 && maze[curRow][curCol - 1] != '@') {
			if (solveMagicMazeR(curRow, curCol - 1)) {
				return true;
			}
		}
		
		return false;

	}

}

/* Ethan Jewell
 */

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Collections;
import java.util.HashMap;
import java.util.Scanner;

public class Railroad {
	
	// Global variables and arrays
	//HashMap<Integer, String[]> trackList = new HashMap<Integer, String[]>();
	ArrayList<String> stops = new ArrayList<String>();
	ArrayList<edge> edges = new ArrayList<>();
	
	// Edge class to keep track of all the information
	public class edge implements Comparable<edge> {
		
		// Crucial info
		public String src;
		public String des;
		public int weight;
		
		// Constructor for the info
		public edge(String vertex1, String vertex2, int size) {
			src = vertex1;
			des = vertex2;
			weight = size;
		}
		
		// New comparable so I can easily sort! (I am sure there is an easier solution however I can not find it lol)
		public int compareTo(edge otherEdge) {
			return this.weight - otherEdge.weight;
		}
		
	}
	
	// Borrowed code from provided DisjointSetImproved class
	public class DisjointSetImproved
	{
		int [] rank;
		int [] parent;
	    int n;
		
	    public DisjointSetImproved(int n)
	    {
	        rank = new int[n];
	        parent = new int[n];
	        this.n = n;
	        makeSet();
	    }
		
		// Creates n sets with single item in each
	    public void makeSet()
	    {
	        for (int i = 0; i < n; i++) 
			{
	            parent[i] = i;
	        }
	    }
		
		//path compression
		public int find(int x)
	    {
	        if (parent[x] != x) 
			{
	            parent[x] = find(parent[x]);
	        }
	 
	        return parent[x];
	    }
		
		//union by rank
		public void union(int x, int y)
	    {
	        int xRoot = find(x), yRoot = find(y);
	 
	        if (xRoot == yRoot)
	            return;
			
	        if (rank[xRoot] < rank[yRoot])
	            parent[xRoot] = yRoot;
			
	        else if (rank[yRoot] < rank[xRoot])
	            parent[yRoot] = xRoot;
	        else 
	        {
	            parent[yRoot] = xRoot;
	            rank[xRoot] = rank[xRoot] + 1;
	        }
	    }
	}
	
	// Scan in the file and assign the information respectively
	public Railroad(int tracks, String filename) {
		
		String curRead1;
		String curRead2;
		int curValue;
		int found = 0;
		
		// Attempt to set up file and scanner
		File file = new File(filename);
		Scanner myScan = null;
		try {
			myScan = new Scanner(file);
		} catch (Exception e) {
			System.out.println("File Not Found...");
		}
		
		// Loop through number of tracks
		for (int i = 0; i < tracks; i++) {
			
			// Find and add first vertex
			curRead1 = myScan.next();
			found = 0;
			for (int x = 0; x < stops.size(); x++) {
				if (stops.get(x).equals(curRead1)) {
					found = 1;
				}
			}
			if (found == 0) {
				stops.add(curRead1);
			}
			found = 0;
			
			// Find and add second vertex
			curRead2 = myScan.next();
			found = 0;
			for (int x = 0; x < stops.size(); x++) {
				if (stops.get(x).equals(curRead2)) {
					found = 1;
				}
			}
			if (found == 0) {
				stops.add(curRead2);
			}
			found = 0;
			
			// Find value of track
			curValue = myScan.nextInt();
			
			// Add new edge alphabetically
			edge curEdge;
			if (curRead1.compareTo(curRead2) <= 0) {
				curEdge = new edge(curRead1, curRead2, curValue);
			} else {
				curEdge = new edge(curRead2, curRead1, curValue);
			}
			edges.add(curEdge);
		}		
	}
	
	// our actual solving, which is essentially kruskals algorithm with extra steps
	public String buildRailroad() {
		
		// Our final result string and total value
		String result = "";
		int total = 0;
		
		// Empty disjoint set
		DisjointSetImproved A = new DisjointSetImproved(stops.size());
		
		// Sorting our edges by weight, as modified within the class
		Collections.sort(edges);
		
		// Loop through each edge to add to our disjoint set, if no loop is caused
		for (int i = 0; i < edges.size(); i++) {
			
			// Current edge
			edge curEdge = edges.get(i);
			
			// Check if link already established
			if (A.find(stops.indexOf(curEdge.src)) != A.find(stops.indexOf(curEdge.des))) {
				
				// Create union
				A.union(stops.indexOf(curEdge.src), stops.indexOf(curEdge.des));
				
				// Add to result to be printed and store our total
				result = result + curEdge.src + "---" + curEdge.des + "\t$" + curEdge.weight + "\n";
				total += curEdge.weight;
			}
		}
		// Add our final total cost to the result and return to be printed
		result = result + "The cost of the railroad is $" + total + ".";
		return result;
	}
}

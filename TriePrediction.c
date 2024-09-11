// Ethan Jewell
// Predicting possible string outcomes using Tries

// NOTE - I definitely overcomplicated a couple parts of this, mainly when I ended up using tokens
// with pulling in input when building my trie. I am aware we have not used tokens in class but
// when searching for a solution to tracking what the last word was (or, more specifically, that
// there was no last word) I ended up doing some research on string tokens and found they were
// perfect for my problem.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"

// HELPER FUNCTIONS

// Create a TrieNode
TrieNode *createNode(void)
{
	int i;

	// Create our new node and assign values for default new node
	TrieNode *newNode = malloc(sizeof(TrieNode));
	newNode->count = 0;

	// Assign each of the children pointers to NULL by default
	for (i = 0; i < 26; i++) 
	{
		newNode->children[i] = NULL;
	}

	newNode->subtrie = NULL;

	// Return our newly made node
	return newNode;
}

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

// Builds the trie we will be working with
TrieNode *buildTrie(char *filename)
{
	TrieNode *root = NULL;
	TrieNode *tptr;
	TrieNode *sub = NULL;
	// Added 40 just to be safe and it is an insignifigant amount compared to rest
	int lineLength = MAX_CHARACTERS_PER_WORD * MAX_WORDS_PER_LINE + 40;
	char curWord[MAX_CHARACTERS_PER_WORD + 1];
	char curLine[lineLength];
	char *token;
	int i;
	int length;
	int index;

	// Open a file pointer
	FILE *fptr = fopen(filename, "r");

	// If the input file doesn't exist return NULL
	if (fptr == NULL) 
	{
		return NULL;
	}

	// Create our new root node and assigns our pointer to start there
	root = createNode();

	// Loop through and add each word in our corpus to the trie
	while (fgets(curLine, lineLength, fptr) != NULL) 
	{
		// Signifies the start of the line and that there is no previous subtrie
		tptr = NULL;
		sub = NULL;

		// Creates a new string token that will be pulling from our line
		token = strtok(curLine, " ");

		// Keep pulling from this line until we have nothing left
		while (token != NULL) 
		{
			strcpy(curWord, token);

			// Create subtrie if needed and assign subtrie pointer to start there
			if (tptr != NULL) 
			{
				if (tptr->subtrie == NULL) 
				{
					tptr->subtrie = createNode();
				}
				sub = tptr->subtrie;
			}

			// Set the starting pointer and length of word to be added
			tptr = root;
			length = strlen(curWord);

			// Loop through each character of the word
			for (i = 0; i < length; i++) 
			{
				// If a valid character
				if (isalpha(curWord[i]))
				{
					// Find the child node index and if it doesn't exist, create one
					index = tolower(curWord[i]) - 'a';
					if (tptr->children[index] == NULL) 
					{
						tptr->children[index] = createNode();
					}

					// If we are adding to our subtrie as well, do so
					if (sub != NULL) 
					{
						if (sub->children[index] == NULL) 
						{
							sub->children[index] = createNode();
						}
					}

					// Move the node pointer forward, and subtrie pointer if needed
					tptr = tptr->children[index];
					if (sub != NULL) 
					{
						sub = sub->children[index];
					}
				}
			}

			// Add to the count of how often said word occurs, and subtrie if needed
			tptr->count = tptr->count + 1;
			if (sub != NULL) 
			{
				sub->count = sub->count + 1;
			}

			// Actually pull our next word (or nothing)
			token = strtok(NULL, " ");
		}
	}

	// Close our pointer
	fclose(fptr);

	// Return our root of this trie
	return root;
}

// Our function to process our input file
int processInputFile(TrieNode *root, char *filename)
{
	char request[MAX_CHARACTERS_PER_WORD + 1];
	char wordpar[MAX_CHARACTERS_PER_WORD + 1];
	char *frequent;
	TrieNode *ptr;
	int n;
	int i;

	// Open a file pointer
	FILE *fptr = fopen(filename, "r");

	// If the input file doesn't exist return 1
	if (fptr == NULL) 
	{
		return 1;
	}

	// Pulls our input from the file until nothing left
	while (fscanf(fptr, "%s", request) != EOF) 
	{
		// Text prediction
		if (strcmp(request, "@") == 0) 
		{
			// Set up the rest of our parameters and get the memory allocated
			fscanf(fptr, " %s ", wordpar);
			fscanf(fptr, "%d", &n);
			frequent = malloc(sizeof(char) * MAX_CHARACTERS_PER_WORD + 1);

			// Print the starting word and find its node (if any) in the trie
			printf("%s", wordpar);
			ptr = getNode(root, wordpar);

			// If the word exists in the trie, start looping through the trie
			if (ptr != NULL) 
			{
				for (i = 0; i < n; i++) 
				{
					// Leave early if no subtrie found
					if (ptr->subtrie == NULL) 
					{
						break;
					}

					// Pull our most frequent word from the subtrie and assign our pointer to be there
					getMostFrequentWord(ptr->subtrie, frequent);
					ptr = getNode(root, frequent);

					// Print the space and word we found to the end of our line
					printf(" ");
					printf("%s", frequent);
				}
			}
			// Print the new line character when done with the rest and free our dynamic memory
			printf("\n");
			free(frequent);

		// Print out the Trie
		} 
		else if (strcmp(request, "!") == 0) 
		{
			printTrie(root, 0);

		// If scan is just a single string, print string and its subtrie if any
		} 
		else if (strlen(request) > 0) 
		{
			printf("%s\n", request);
			ptr = getNode(root, request);
			if (ptr != NULL) 
			{
				if (ptr->subtrie != NULL) 
				{
					printTrie(ptr->subtrie, 1);
				} 
				else 
				{
					printf("(EMPTY)\n");
				}
			} 
			else 
			{
				printf("(INVALID STRING)\n");
			}
		}
	}

	fclose(fptr);

	// Return 0 at the end of our input processing
	return 0;
}

TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	// If node is null return
	if (root == NULL) 
	{
		return NULL;
	}

	// If there is a subtrie, start freeing that
	if (root->subtrie != NULL)
	{
		destroyTrie(root->subtrie);
	}

	for (i = 0; i < 26; i++) 
	{
		if (root->children[i] != NULL) 
		{
			// Start freeing the rest of the nodes
			destroyTrie(root->children[i]);
		}
	}

	// After everything below is free, free this and return to free any others above
	free(root);
	return NULL;
}

TrieNode *getNode(TrieNode *root, char *str)
{
	int i;
	int length;
	int index;
	TrieNode *ptr = root;

	// If root is NULL return NULL
	if (root == NULL) 
	{
		return NULL;
	}

	// Set the length equal to the word we are searching for length
	length = strlen(str);
	for (i = 0; i < length; i++) 
	{
		index = tolower(str[i]) - 'a';
		// If there isn't already a node where we want to go, end early
		if (ptr->children[index] == NULL) 
		{
			return NULL;
		}

		// Otherwise move forward
		ptr = ptr->children[index];
	}

	// As long as there is a count higher than zero at the ending node, the word exists
	if (ptr->count > 0) 
	{
		return ptr;
	}

	// Otherwise while word has all nodes, it isn't actually documented
	return NULL;
}

// Helper function for finding the most frequent word in a trie, see primary
void getMostFrequentWordHelper(TrieNode *root, char *buffer, char **word, int k, int *max) 
{
	int i;

	// Check if root is NULL
	if (root == NULL) 
	{
		return;
	}

	// Check if root count is greater than max, and replace new max if so
	if (root->count > 0 && root->count > *max) 
	{
		*max = root->count;
		strcpy(*word, buffer);
	}

	// If equal in count, replace only if the word is before the current max in the alphabet
	if (root->count == *max && strcmp(buffer, *word) < 0) 
	{
		*max = root->count;
		strcpy(*word, buffer);
	} 

	buffer[k + 1] = '\0';

	// Loop through all child options and call the function with modified parameters
	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		getMostFrequentWordHelper(root->children[i], buffer, word, k + 1, max);
	}

	buffer[k] = '\0';
}

// Program to find the most frequent word in a trie 
// (CREDIT to DR. S for the framework to solving this one, modified version of his print function)
void getMostFrequentWord(TrieNode *root, char *str)
{
	// Create the two string arrays we will use and the max count
	char buffer[MAX_CHARACTERS_PER_WORD + 1];
	char *word = malloc(sizeof(char) * MAX_CHARACTERS_PER_WORD + 1);
	int *max = malloc(sizeof(int));

	// Assign our starting max to 0
	*max = 0;

	// Pass the following (root, current word, current max word, size of current word, current max)
	getMostFrequentWordHelper(root, buffer, &word, 0, max);

	// Transfer the word to outside the function and free memory
	strcpy(str, word);
	free(word);
	free(max);
}

// Check if the given trie contains a string
int containsWord(TrieNode *root, char *str)
{
	int i;
	int length;
	int index;
	TrieNode *ptr = root;

	// If root is NULL, return 0
	if (root == NULL) 
	{
		return 0;
	}

	// If string is empty return based on if root has a count
	if (strcmp(str, "") == 0)
	{
		if (root->count > 0) 
		{
			return 1;
		} 
		else 
		{
			return 0;
		}
	}

	// Loop through trie for as much as we can
	length = strlen(str);
	for (i = 0; i < length; i++) 
	{
		index = tolower(str[i]) - 'a';
		// If we can't move forward the trie can not possibly contain the word
		if (ptr->children[index] == NULL) 
		{
			return 0;
		}

		// Otherwise move forward
		ptr = ptr->children[index];
	}

	// Our last check to make sure the word is contained
	if (ptr->count > 0) 
	{
		return 1;
	} 
	else 
	{
		return 0;
	}
}

// Recursively count all the words in this Trie
int recursiveCount(TrieNode *root, int count)
{
	int i;

	// Check that root isn't null
	if (root == NULL) 
	{
		return 0;
	}

	// Call each child and check if 
	for (i = 0; i < 26; i++) 
	{
		count += recursiveCount(root->children[i], 0);
	}

	// Return the current count as well as the count of the node we are in
	return count + root->count;
}

// Count how many words in the trie there are with the given prefix
int prefixCount(TrieNode *root, char *str)
{
	int i;
	int length;
	int index;
	int count = 0;
	TrieNode *ptr = root;

	// If root is NULL, return 0
	if (root == NULL) 
	{
		return 0;
	}

	// If string is empty return root count
	if (strcmp(str, "") == 0)
	{
		return recursiveCount(root, count);
	}

	length = strlen(str);

	// Loop through the word and check if the child nodes are there
	for (i = 0; i < length; i++) 
	{
		index = tolower(str[i]) - 'a';
		if (ptr->children[index] == NULL) 
		{
			return 0;
		}

		// Otherwise move forward
		ptr = ptr->children[index];
	}

	// Search through remaining tree and tally up every word
	return recursiveCount(ptr, count);

}

// Count how many nodes we would need to add a word
int newNodeCount(TrieNode *root, char *str)
{
	int i;
	TrieNode *ptr;
	int length = strlen(str);
	int index;

	// If root is null, we will need the maximum number of nodes including the root node
	if (root == NULL) 
	{
		return length + 1;
	}

	// If an empty string, we should return 0;
	if (strlen(str) < 1) 
	{
		return 0;
	}

	// Traverse the Trie and where we can't continue is where we can find how many nodes we need
	ptr = root;
	for (i = 0; i < length; i++) 
	{
		index = tolower(str[i]) - 'a';
		if (ptr->children[index] == NULL) 
		{
			return length - i;
		}

		ptr = ptr->children[index];
	}

	// If we reach the end that means we need no extra nodes
	return 0;
}

// How difficult I found this assignment
double difficultyRating(void)
{
	return 4.5;
}

// How many hours I spent on this assignment
double hoursSpent(void)
{
	return 8.0;
}

// Our main function
int main(int argc, char **argv)
{
	// Create our root pointer and store our command line arguments
	TrieNode *root;
	char *corpusName = argv[1];
	char *inputName = argv[2];

	// Create our trie based on the corpus input
	root = buildTrie(corpusName);

	// Work with our trie based on specified input
	processInputFile(root, inputName);

	// Free our trie nodes
	root = destroyTrie(root);

	return 0;
}

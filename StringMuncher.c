// Ethan Jewell
// Implementing a data structure called String Muncher that automatically expands to eat more strings

// All the neccessary libraries
#include "StringMuncher.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Dynamically allocates space for, and sets up a new String Muncher
StringMuncher *summonStringMuncher(char *name, int num_stomachs);

// Free all dynamic memory associated with the String Muncher
StringMuncher *banishStringMuncher(StringMuncher *munchy);

// Expand the stomach chain of specific String Muncher
StringMuncher *expandDigestiveTract(StringMuncher *munchy, int num_stomachs);

// Shrink the stomach chain of specific String Muncher
StringMuncher *contractDigestiveTract(StringMuncher *munchy);

// Eat a new string, and expand if needed
char *omNomNom(StringMuncher *munchy, char *snack);

// Return the pointer of a snack at a given stomach
char *endoscopy(StringMuncher *munchy, int which_stomach);

// Transform the food item, if any, into something else
char *transmogrify(StringMuncher *munchy, int which_stomach, char *snack);

// Send food to specified index and displaced those who are passed
char *gobbleGulp(StringMuncher *munchy, int which_stomach, char *snack);

// Remove a snack from the stomach and let the others pass down through
int digest(StringMuncher *munchy, int which_stomach);

// Remove the uppermost string in the digestive track or return 0
int heave(StringMuncher *munchy);

// Remove the lowermost string and adjust the rest of the track accordingly
int evacuate(StringMuncher *munchy);

// Return the max number of strings String Muncher could hold
int getDigestiveTractCapacity(StringMuncher *munchy);

// Return the number of Strings currently in String Muncher
int getDigestiveTractSize(StringMuncher *munchy);

// Print the contents of a String Muncher
void printStomachContents(StringMuncher *munchy);

// Return difficulty rating of this assignment
double difficultyRating(void);

// Return the hours spent on this assignment
double hoursSpent(void);

// -- BONUS FUNCTION -- Clones munchy and returns null if any malloc call fails
StringMuncher *cloneStringMuncher(StringMuncher *munchy);

// Dynamically allocates space for, and sets up a new String Muncher
StringMuncher *summonStringMuncher(char *name, int num_stomachs) 
{

	int i;
	StringMuncher *newMunch = NULL;

	// Makes sure the name sent to the function is viable
	if (name == NULL || strlen(name) < 1) 
	{
		return NULL;
	}

	// Dynamic memory for the struct
	newMunch = (StringMuncher *) malloc(sizeof(StringMuncher));

	// Check newMunch dynamically initiallized correctly
	if (newMunch == NULL) 
	{
		return NULL;
	}

	// Sets the name in new struct to desired and makes the memory the correct size
	newMunch->name = NULL;
	newMunch->name = (char *) malloc((sizeof(char) * strlen(name)) + 1);

	// Check newMunch->name dynamically initiallized correctly
	if (newMunch->name == NULL) 
	{
		free(newMunch);
		return NULL;
	}

	strcpy(newMunch->name, name);

	//Sets up the stomach int variables and makes sure size is valid
	newMunch->num_stomachs = num_stomachs;
	newMunch->num_stomachs_occupied = 0;

	if (newMunch->num_stomachs < DEFAULT_DIGESTIVE_TRACT_CAPACITY) 
	{
		newMunch->num_stomachs = DEFAULT_DIGESTIVE_TRACT_CAPACITY;
	}

	// Allocate space for stomach chain and set each stomach to NULL
	newMunch->stomachs = NULL;
	newMunch->stomachs = (char **) malloc(sizeof(char *) * num_stomachs);

	// Check newMunch->stomachs dynamically initiallized correctly
	if (newMunch->stomachs == NULL) 
	{
		free(newMunch->name);
		free(newMunch);
		return NULL;
	}

	for (i = 0; i < num_stomachs; i++) 
	{
		newMunch->stomachs[i] = NULL;
	}

	// If successful, print message to screen and return address
	printf("-> %s, a string muncher with %d stomachs, has emerged from the void!\n", newMunch->name, newMunch->num_stomachs);
	return newMunch;
}

// Free all dynamic memory associated with the String Muncher
StringMuncher *banishStringMuncher(StringMuncher *munchy) 
{

	int i;

	// Make sure munchy isn't NULL
	if (munchy == NULL) 
	{
		return NULL;
	}

	// Free each individual unused stomach
	for (i = 0; i < munchy->num_stomachs_occupied; i++) 
	{
		free(munchy->stomachs[i]);
		munchy->stomachs[i] = NULL;
	}

	free(munchy->stomachs);

	// Free the name and then the munchy, after the message is printed
	printf("-> %s has returned to the void.\n", munchy->name);
	free(munchy->name);
	free(munchy);
	return NULL;
}

// Expand the stomach chain of specific String Muncher
StringMuncher *expandDigestiveTract(StringMuncher *munchy, int num_stomachs) 
{

	char **newStomachs = NULL;
	int i;

	// Make sure we can even work with munchy at all
	if (munchy == NULL || num_stomachs <= munchy->num_stomachs || num_stomachs < 0) 
	{
		return NULL;
	}

	// Create new stomach and populate it with the old one and NULL for the new spots
	newStomachs = (char **) malloc(sizeof(char *) * num_stomachs);

	// Make sure newStomachs was dynamically allocated correctly
	if (newStomachs == NULL) 
	{
		free(newStomachs);
		return NULL;
	}

	// Set each previous stomach to the new slot
	for (i = 0; i < munchy->num_stomachs; i++) 
	{
		newStomachs[i] = munchy->stomachs[i];
	}

	// Every unused slot is now set to NULL for safety purposes
	for (i = munchy->num_stomachs; i < num_stomachs; i++) 
	{
		newStomachs[i] = NULL;
	}

	// New number of stomachs
	munchy->num_stomachs = num_stomachs;

	// Free old stomach and reassign
	free(munchy->stomachs);
	munchy->stomachs = newStomachs;

	// If successful, print the output and return munchy
	if (munchy->num_stomachs == 1) 
	{
		printf("-> %s's digestive tract has expanded to consist of %d stomach!\n", munchy->name, munchy->num_stomachs);
	} else 
	{
		printf("-> %s's digestive tract has expanded to consist of %d stomachs!\n", munchy->name, munchy->num_stomachs);
	}
	
	return munchy;
}

// Shrink the stomach chain of specific String Muncher
StringMuncher *contractDigestiveTract(StringMuncher *munchy) 
{
	char **newStomachs = NULL;
	int i;

	// Make sure we can even work with munchy at all
	if (munchy == NULL) 
	{
		return NULL;
	}

	// Make sure we can contract any more
	if (munchy->num_stomachs <= munchy->num_stomachs_occupied) 
	{
		return NULL;
	}

	// Create new stomach and populate it with the old one
	newStomachs = (char **) malloc(sizeof(char *) * munchy->num_stomachs_occupied);

	// Make sure newStomachs dynamically allocates correctly
	if (newStomachs == NULL) 
	{
		free(newStomachs);
		return NULL;
	}

	// Assign each of the stomachs to the new stomach array
	for (i = 0; i < munchy->num_stomachs_occupied; i++) 
	{
		newStomachs[i] = munchy->stomachs[i];
	}

	// New number of stomachs
	munchy->num_stomachs = munchy->num_stomachs_occupied;

	// Free old stomach and reassign
	free(munchy->stomachs);
	munchy->stomachs = newStomachs;

	// If successful, print the output and return munchy
	if (munchy->num_stomachs == 1) 
	{
		printf("-> %s's digestive tract has contracted to consist of only %d stomach!\n", munchy->name, munchy->num_stomachs);
	} else 
	{
		printf("-> %s's digestive tract has contracted to consist of only %d stomachs!\n", munchy->name, munchy->num_stomachs);
	}
	
	return munchy;

}

// Eat a new string, and expand if needed
char *omNomNom(StringMuncher *munchy, char *snack) 
{

	char *newSnack = NULL;

	// Make sure the pointers are valid
	if (munchy == NULL || snack == NULL)
	{
		return NULL;
	}

	// Expand the stomach chain if it is full
	if (munchy->num_stomachs == munchy->num_stomachs_occupied) 
	{
		expandDigestiveTract(munchy, (munchy->num_stomachs * 2 + 1));
	}

	// Dynamically allocte room for the new snack and assign at next stomach
	newSnack = (char *) malloc((sizeof(char) * strlen(snack)) + 1);

	// Make sure newSnack dynamically allocated correctly
	if (newSnack == NULL) 
	{
		free(newSnack);
		return NULL;
	}

	strcpy(newSnack, snack);
	munchy->stomachs[munchy->num_stomachs_occupied] = newSnack;

	//Update the value of occupied stomachs and return
	munchy->num_stomachs_occupied += 1;

	return newSnack;

}

// Return the pointer of a snack at a given stomach
char *endoscopy(StringMuncher *munchy, int which_stomach) 
{

	// If munchy is NULL
	if (munchy == NULL) 
	{
		return NULL;
	}

	// If out of bounds, return NULL
	if (which_stomach >= munchy->num_stomachs_occupied || which_stomach < 0) 
	{
		return NULL;
	}

	// return the address at which_stomach location, which may be NULL
	return munchy->stomachs[which_stomach];
}

// Transform the food item, if any, into something else
char *transmogrify(StringMuncher *munchy, int which_stomach, char *snack) 
{

	char *newSnack = NULL;

	// If munchy is NULL
	if (munchy == NULL) 
	{
		return NULL;
	}

	// If out of bounds, return NULL
	if (which_stomach >= munchy->num_stomachs_occupied || which_stomach < 0) 
	{
		return NULL;
	}

	//If specific stomach is empty
	if (munchy->stomachs[which_stomach] == NULL) 
	{
		return NULL;
	}

	// Make sure the new snack is valid
	if (snack == NULL)
	{
		return NULL;
	}

	// Dynamically allocte room for the new snack and assign at stomach location after freeing the old snack
	newSnack = (char *) malloc((sizeof(char) * strlen(snack)) + 1);

	// Make sure newSnack is dynamically allocated correctly
	if (newSnack == NULL) 
	{
		return NULL;
	}

	strcpy(newSnack, snack);
	free(munchy->stomachs[which_stomach]);
	munchy->stomachs[which_stomach] = newSnack;

	return newSnack;
}

// Send food to specified index and displaced those who are passed
char *gobbleGulp(StringMuncher *munchy, int which_stomach, char *snack) 
{

	char *newSnack = NULL;
	int i;

	//If munchy or snack are NULL, return NULL
	if (munchy == NULL || snack == NULL)
	{
		return NULL;
	}

	//If invalid index
	if (which_stomach < 0) 
	{
		return NULL;
	}

	// Check if there is room in the stomachs, if not expand
	if (munchy->num_stomachs == munchy->num_stomachs_occupied) 
	{
		expandDigestiveTract(munchy, (munchy->num_stomachs * 2 + 1));
	}

	// Dynamically allocate space for the new snack
	newSnack = (char *) malloc((sizeof(char) * strlen(snack)) + 1);

	// Make sure newSnack is dynamically allocated correctly
	if (newSnack == NULL) 
	{
		return NULL;
	}

	strcpy(newSnack, snack);

	// Just add snack to the top if higher index desired than max and return
	if (which_stomach >= munchy->num_stomachs_occupied) 
	{
		munchy->stomachs[munchy->num_stomachs_occupied] = newSnack;
		munchy->num_stomachs_occupied += 1;
		return newSnack;
	}

	// Displace each address of preexisting food to make room for the new one
	for (i = munchy->num_stomachs_occupied; i > which_stomach; i--) 
	{
		munchy->stomachs[i] = munchy->stomachs[i - 1];
	}

	// Assign the snack to the newly opened stomach
	munchy->stomachs[which_stomach] = newSnack;
	munchy->num_stomachs_occupied += 1;

	// Return the address of the new snack
	return newSnack;
}

// Remove a snack from the stomach and let the others pass down through
int digest(StringMuncher *munchy, int which_stomach) 
{

	int i;

	// If munchy is null
	if (munchy == NULL) 
	{
		return 0;
	}

	// If desired index is too high
	if (munchy->num_stomachs_occupied <= which_stomach) 
	{
		return 0;
	}

	// Remove (digest) food at specified index
	free(munchy->stomachs[which_stomach]);

	// Shift all the food items down and assign the last spot to NULL
	for (i = which_stomach; i < munchy->num_stomachs_occupied - 1; i++) 
	{
		munchy->stomachs[i] = munchy->stomachs[i + 1];
	}

	munchy->stomachs[munchy->num_stomachs_occupied - 1] = NULL;

	// Adjust the number of stomachs occupied accordingly
	munchy->num_stomachs_occupied -= 1;

	// Return 1 cause successful
	return 1;
}

// Remove the uppermost string in the digestive track or return 0
int heave(StringMuncher *munchy) 
{
	// If munchy is NULL or there are no strings in the stomach
	if (munchy == NULL || munchy->num_stomachs_occupied == 0) 
	{
		return 0;
	}

	// Free the string at the location, set it to NULL, and update the count of stomachs full
	free(munchy->stomachs[munchy->num_stomachs_occupied - 1]);
	munchy->stomachs[munchy->num_stomachs_occupied - 1] = NULL;
	munchy->num_stomachs_occupied -= 1;

	return 1;

}

// Remove the lowermost string and adjust the rest of the track accordingly
int evacuate(StringMuncher *munchy) 
{

	int i;

	// If munchy is NULL or there are no strings in the stomach
	if (munchy == NULL || munchy->num_stomachs_occupied == 0) 
	{
		return 0;
	}

	// Free the string at the bottom of the track, set it to NULL, and update the count of stomachs full
	free(munchy->stomachs[0]);
	munchy->stomachs[0] = NULL;
	munchy->num_stomachs_occupied -= 1;

	// Adjust the remaining string locations, if any, and update the last one
	for (i = 0; i < munchy->num_stomachs_occupied; i++) 
	{
		munchy->stomachs[i] = munchy->stomachs[i + 1];
	}

	munchy->stomachs[munchy->num_stomachs_occupied] = NULL;

	return 1;
}

// Return the max number of strings String Muncher could hold
int getDigestiveTractCapacity(StringMuncher *munchy) 
{
	return munchy->num_stomachs;
}

// Return the number of Strings currently in String Muncher
int getDigestiveTractSize(StringMuncher *munchy) 
{
	return munchy->num_stomachs_occupied;
}

// Print the contents of a String Muncher
void printStomachContents(StringMuncher *munchy) 
{

	int i;

	// Print either ??? for NULL muncher, empty if empty, or each string in the stomachs
	if (munchy == NULL) 
	{
		printf("(?\?\?)\n");
	} else if (munchy->num_stomachs_occupied > 0)
	{
		for (i = 0; i < munchy->num_stomachs_occupied; i++) 
		{
			printf("%s\n", munchy->stomachs[i]);
		}
	} else 
	{
		printf("(%s's digestive tract is empty.)\n", munchy->name);
	}
}

// Return difficulty rating of this assignment
double difficultyRating(void) 
{
	return 3.1;
}

// Return the hours spent on this assignment
double hoursSpent(void) 
{
	return 11.5;
}

// Clones munchy and returns NULL if any malloc call fails
StringMuncher *cloneStringMuncher(StringMuncher *munchy) 
{
	int i;
	StringMuncher *munchyClone = NULL;

	// Make sure Munchy isn't NULL
	if (munchy == NULL) 
	{
		return NULL;
	}

	// Create new malloc to hold StringMuncher struct clone
	munchyClone = (StringMuncher *) malloc(sizeof(StringMuncher));

	// Check to see if malloc call went through
	if (munchyClone == NULL) 
	{
		return NULL;
	}

	// Populate the int values of the struct, we will update stomachs occupied later
	munchyClone->num_stomachs_occupied = 0;
	munchyClone->num_stomachs = munchy->num_stomachs;

	// Create a name malloc for the struct
	munchyClone->name = NULL;
	munchyClone->name = (char *) malloc((sizeof(char) * strlen(munchy->name)) + 1);

	// Check to see if malloc call went through
	if (munchyClone->name == NULL) 
	{
		free(munchyClone);
		return NULL;
	}

	// Copy the name over
	strcpy(munchyClone->name, munchy->name);

	// Malloc for the stomach array
	munchyClone->stomachs = NULL;
	munchyClone->stomachs = ((char **) malloc(sizeof(char *) * munchy->num_stomachs));

	// Check to make sure the stomach was properly created as a malloc
	if (munchyClone->stomachs == NULL) 
	{
		free(munchyClone->name);
		free(munchyClone);
		return NULL;
	}

	// Populate the new stomach with new copies of the old snacks
	for (i = 0; i < munchy->num_stomachs_occupied; i++) 
	{
		omNomNom(munchyClone, munchy->stomachs[i]);
	}

	// Print the success statement depending on number of stomachs and how many are full
	if (munchyClone->num_stomachs == 1) 
	{
		if (munchyClone->num_stomachs_occupied == 1) 
		{
			printf("-> Successfully cloned %s, a string muncher with %d stomach, %d of which is full.\n", munchyClone->name, munchyClone->num_stomachs, munchyClone->num_stomachs_occupied);
		} else 
		{
			printf("-> Successfully cloned %s, a string muncher with %d stomach, %d of which are full.\n", munchyClone->name, munchyClone->num_stomachs, munchyClone->num_stomachs_occupied);
		}
	} else 
	{
		if (munchyClone->num_stomachs_occupied == 1) 
		{
			printf("-> Successfully cloned %s, a string muncher with %d stomachs, %d of which is full.\n", munchyClone->name, munchyClone->num_stomachs, munchyClone->num_stomachs_occupied);
		} else 
		{
			printf("-> Successfully cloned %s, a string muncher with %d stomachs, %d of which are full.\n", munchyClone->name, munchyClone->num_stomachs, munchyClone->num_stomachs_occupied);
		}
	}

	// Return the new munchy struct pointer
	return munchyClone;
}

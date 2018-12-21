/*main.c*/

//
// Searching for movies in a binary search tree.
//
// << Jakub >>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #02
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int     BSTMovieID;		//Creatng variable for MovieID
typedef char  *BSTMovieName;   //Creating a variable for MovieName

typedef struct BSTNode
{
	BSTMovieID   MovieID;  //Creating MovieID
	BSTMovieName  MovieName[200];   //Creating NameID
	struct BSTNode  *Left;  //pointer to the left
	struct BSTNode  *Right;  //pointer to the right
} BSTNode;

typedef struct BST
{
	BSTNode *Root;	//Creating root
	int      Count; //creatng count
} BST;


//
// BSTCreate: dynamically creates and returns an empty
// binary search tree:
//
BST *BSTCreate()
{
	//creating tree
	BST *tree;

	tree = (BST *)malloc(sizeof(BST)); //allocate memory
	tree->Root = NULL;  //Root is null
	tree->Count = 0;  //Count is 0

	return tree;  
}


BSTNode *BSTSearch(BST *tree, BSTMovieID movieID)
{
	// creating current pointer
	BSTNode *curr = tree->Root;
	while (curr != NULL) {  //looping through binary tree
		printf(">>Visiting %d: '%s'\n", curr->MovieID, curr->MovieName);   //Printing out each visited Node

		if (movieID == curr->MovieID) {  //If ID's match return the node thats pointing to
			return curr;
		}
		else if (movieID < curr->MovieID) { //If ID is less than, go left
			curr = curr->Left;
		}
		else {
			curr = curr->Right; //If ID is greater than, go right
		}
	}
	return NULL;  //MovieID not found
}


//
// BSTInsert: inserts the given (key, value) pair into the binary search
// tree.  Returns true (non-zero) if the insert was successful, returns 
// false (0) if the given key is already in the tree -- in which case the
// given (key, value) pair is not inserted.
//
int BSTInsert(BST *tree, BSTMovieID movieID, BSTMovieName movieName)
{
	BSTNode *prev;
	BSTNode *curr;
	prev = NULL;
	curr = tree->Root;

	while (curr != NULL) {    //While not at end of BST
		prev = curr;  //Set prev to curr
		if (movieID < curr->MovieID) {  // If movieID is less than Current movie ID go left
			curr = curr->Left;
		}
		else if (movieID > curr->MovieID) {// If movieID is greater than Current movie ID go right 
			curr = curr->Right;
		}
		else {
			return 0; 
		}
	}
	BSTNode *t = (BSTNode*)malloc(sizeof(BSTNode)); //Allocate memory
	if (prev == NULL) {  //If prev is NULL make t head pointer Node
		tree->Root = t;
	}
	else if (movieID < prev->MovieID) {  //If movie iD is bigger than prev, go left
		prev->Left = t;    
	}
	else {  //If movie iD is less than prev, go right
		prev->Right = t;   
	}

	t->MovieID = movieID; //store movieID
	strcpy(t->MovieName, movieName);  //store MovieName
	t->Left = NULL;  //Set left pointer to NULL
	t->Right = NULL;  //Set right pointer to NULL

	//printf(" %s\n", t->MovieName);
	tree->Count++;  //Increase tree count
	return 0;
}


//
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
//
char *getFileName()
{
	char filename[512];
	int  fnsize = sizeof(filename) / sizeof(filename[0]);

	// input filename from the keyboard:
	fgets(filename, fnsize, stdin);
	filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

												 // make sure filename exists and can be opened:
	FILE *infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("**Error: unable to open '%s'\n\n", filename);
		exit(-1);
	}

	fclose(infile);

	// duplicate and return filename:
	char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(s, filename);

	return s;
}

void fileOneFunction(FILE *File1, char File1Arr[], int  FILE1nsize,  BST *tree, char *MoviesFileName) {

	//Declaring tokens
	char *token = NULL;
	char *token2 = NULL;

	//Declaring variables for use
	int MovieID = 0;
	char MovieName[255];
	File1 = fopen(MoviesFileName, "r"); //opening File
	
	//Gets first line of File
	fgets(File1Arr, FILE1nsize, File1);
	//While loop to keep going throught the file line by line
	while (fgets(File1Arr, FILE1nsize, File1) != NULL) {
		
		File1Arr[strcspn(File1Arr, "\r\n")] = '\0'; //Adding null to b safe

		token = strtok(File1Arr, ","); //Paring data
		MovieID = atoi(token);         //Storing movieID as number
		token2 = strtok(NULL, ",");   //Paring data
		strcpy(MovieName, token2);    //Copy the token into  moviename
		BSTInsert(tree, MovieID, MovieName);  //Builds binary tree with movieID and movieName
	}
}


int main()
{
	// get movies filename from the user/stdin:
	char *MoviesFileName = getFileName();
	FILE *File1 = fopen(MoviesFileName, "r"); //Opening File

	//Declaring variable array to store each line of file
	char File1Arr[512];

	//Declaring variables
	int  FILE1nsize = sizeof(File1Arr) / sizeof(File1Arr[0]);
	int searchMovieID;
	BSTNode *found;

	//Creating "handle" for binary tree
	BST *tree = BSTCreate();

	//Closing file
	fclose(File1);

	//Call function to parse info and update BST
	fileOneFunction(File1, File1Arr,FILE1nsize, tree, MoviesFileName);

	//Lets user input what movie ID to search for
	scanf("%d", &searchMovieID);

	//returns Node that gets stored into pointer "found"
	found = BSTSearch(tree, searchMovieID);

	//If found is not NULL print out where it was found
	if (found != NULL) {
		printf("Movie %d: %s\n", searchMovieID, found->MovieName);
	}

	//if get here NULL, print out did not find
	else {
		printf("Movie %d: not found\n", searchMovieID);
	}
	return 0;
}

/* 
 Jeremiah Spears
 This program will search the file the user inputs for all non-background
 pixels and will create each object from the pixels using two separate
 calculations (passes). It will then count the
 number of objects in the image (file).
 */

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>

using namespace std;

// struct to hold the details for each pixel
struct pixel
{
	int background = 0;									// stores the value of the pixel (0,1,2,etc)
	int status = 0;										// holds the relationship status of the objects
};														// (this is for after the milestone)

// functions
void createGrid();
void printGrid();
void calc();
void relationship();
void count();

// global variables
pixel nPixel;											// new pixel object
vector < vector<pixel>> vec;							// 2D vector to hold pixels
vector <pixel> tempVec;									// temporary vector to hold pixels
int length = 0;											// variable to hold the dimensions of the grid
int backCount = 0;										// count of objects

int main()
{
	ifstream fileName;									// fileName to open
	string fName;										// Input from user to get fileName
	bool fFound = false;								// if the file is open set to true

	// Ask the user which file they want to open
	//cout << "Input the file name (and extension) of the file you would like to open\n";
	//cin >> fName;
	fName = "test.txt";

	fileName.open(fName);								// open the file the user asked for

	// make sure file is open
	if (fileName.is_open())
	{
		fFound = true;									// let the program know if the file was found
		char stream;									// parser

		// read through the file and count the lines
		while (fileName >> noskipws >> stream)		
		{
			if (stream == '\n')
			{
				length++;								// increase 
			}
		}

		fileName.clear();								// clear eof			
		fileName.seekg(0, ios::beg);					// return to beginning of file

		// read through the file and store the data in tempVec
		while (fileName >> noskipws >> stream)
		{
			if (stream == '1')
			{
				int num = stream - 48;					// convert to int
				nPixel.background = num;				// assign nPixel values 
				tempVec.push_back(nPixel);				// push back nPixel
			}
			if (stream == '0')
			{
				int num = stream - 48;					// convert to int
				nPixel.background = num;				// assign nPixel values 
				tempVec.push_back(nPixel);				// push back nPixel
			}
		}

	}
	else cout << "Could not find file.\n";

	// make sure the file was found before the program tries
	// to output the data
	if (fFound == true)
	{
		createGrid();									// setup the 2d Vector
		calc();											// find the objects
		relationship();									// find objects with a relationship and
														// assign the relationship
		printGrid();									// print the grid
		cout << endl << "Number of unique objects: ";
		count();										// count the objects
		cout << endl;
	}

	system("PAUSE");
	return 0;
}

// this function will setup the 2d vector and assign the values from the
// temporary vector
void createGrid()
{
	// setup the 2d Vector
	for (int col = 0; col < length; col++)				// set number of columns
	{
		vector<pixel>temp;								// create temporary vector to hold columns
		for (int row = 0; row < length; row++)			// set number of rows
		{
			temp.push_back(nPixel);						// push back nAgent onto temp vector
		}
		vec.push_back(temp);							// push back the temp vector onto the 2dVec
	}

	// Set the values of the 2d Vector
	for (int col = 0; col < vec.size(); col++)
	{
		for (int row = 0; row < vec[col].size(); row++)
		{
			vec[col][row] = tempVec[col * length + row];// fill each row and column with vecA
		}
	}
}

// this function will print the grid for the 2d Vector
void printGrid()
{
	// print the grid
	for (int col = 0; col < vec.size(); col++)
	{
		for (int row = 0; row < vec[col].size(); row++)
		{
			cout << vec[col][row].status << " ";		// output the grid
		}
		cout << endl;									// skip a line
	}
}

// this function will check the entire image and find each object
void calc()
{
	// correct the backCount if the first pixel is a non-background pixel
	if (vec[0][0].background == 1)
	{
		backCount++;
	}

	// go through the 2d vector
	for (int row = 0; row < vec.size(); row++)
	{
		for (int col = 0; col < vec[row].size(); col++)
		{
			// only check non-background pixels
			if (vec[row][col].background == 1)
			{
				// row 0
				if (row == 0)
				{
					if (col != 0)
					{
						// Case 1 (new label)
						if (vec[row][col - 1].background == 0)
						{
							vec[row][col].background = vec[row][col].background + backCount;
							backCount++;
						}
						// Case 2 (old label)
						if (vec[row][col - 1].background != 0)
						{
							vec[row][col].background = vec[row][col - 1].background;
						}
					}
				}
				// col 0
				if (col == 0)
				{
					if (row != 0)
					{
						// Case 1 (new label)
						if (vec[row - 1][col].background == 0)
						{
							vec[row][col].background = vec[row][col].background + backCount;
							backCount++;
						}
						// Case 2 (old label)
						if (vec[row - 1][col].background != 0)
						{
							vec[row][col].background = vec[row - 1][col].background;
						}
					}
				}
				// all non edge rows and cols
				if (col != 0 && row != 0)
				{
					// Case 1 - none nearby (new label)
					if (vec[row - 1][col].background == 0 && vec[row][col - 1].background == 0)
					{
						vec[row][col].background = vec[row][col].background + backCount;
						backCount++;
					}
					// Case 2 - one nearby (old label)
					if (vec[row - 1][col].background != 0 && vec[row][col - 1].background == 0)
					{
						vec[row][col].background = vec[row - 1][col].background;
					}
					if (vec[row][col - 1].background != 0 && vec[row - 1][col].background == 0)
					{
						vec[row][col].background = vec[row][col - 1].background;
					}
					// Case 3 - both nearby (old label)
					if (vec[row - 1][col].background != 0 && vec[row][col - 1].background != 0)
					{
						// compare the two labels and assign the smaller one
						if (vec[row - 1][col].background < vec[row][col - 1].background)
						{
							vec[row][col].background = vec[row - 1][col].background;
						}
						else
						{
							vec[row][col].background = vec[row][col - 1].background;
						}
					}
				}
				// set status to background
				vec[row][col].status = vec[row][col].background;
			}
		}
	}
}

void relationship()
{
	// go through the 2d vector
	for (int row = 0; row < vec.size(); row++)
	{
		for (int col = 0; col < vec[row].size(); col++)
		{
			if (vec[row][col].background != 0)
			{
				// All non 0 row and col
				if (col != 0 && row != 0)
				{
					// col - 1
					if (vec[row][col - 1].background != 0)
					{
						if (vec[row][col - 1].background != vec[row][col].background)
						{
							vec[row][col - 1].status = vec[row][col].background;
						}
					}

					// row - 1
					if (vec[row - 1][col].background != 0)
					{
						if (vec[row - 1][col].background != vec[row][col].background)
						{
							vec[row - 1][col].status = vec[row][col].status;
						}
					}

					// while goign through the 2d vec
					// set all pixels that have the same value as the ones that were changed
					// to the new value
					for (int row1 = 0; row1 < vec.size(); row1++)
					{
						for (int col1 = 0; col1 < vec[row1].size(); col1++)
						{
							if (vec[row1][col1].status == vec[row][col - 1].background)
							{
								vec[row1][col1].status = vec[row][col - 1].status;
							}
							if (vec[row1][col1].status == vec[row - 1][col].background)
							{
								vec[row1][col1].status = vec[row - 1][col].status;
							}
						}
					}
				}
			}
		}
	}
}

// This function will count the objects in
// the 2d vector and output the count of
// unique objects
void count()
{
	set<int, greater <int> > set;

	// read throught the 2d vec
	for (int row1 = 0; row1 < vec.size(); row1++)
	{
		for (int col1 = 0; col1 < vec[row1].size(); col1++)
		{
			int count = vec[row1][col1].status;
			set.insert(count);
		}
	}
	
	// the count is - 1 because it is counting 0s
	cout << set.size() - 1;
}
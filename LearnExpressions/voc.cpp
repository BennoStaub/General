/*
	This is a simple programm supporting you in learning new words/expressions.
	Possible use: -learn vocabulary of a new language
				  -learn definitions for your study, i.e. diseases in medicin
				  -learn corresponding expressions, i.e. countries and their main cities

	Copy the executive file in the desired repository and add a library.txt file with the following structure:
		expression1_1,expression1_2;0
		expression2_1,expression2_2;0
		etc.

	The 0 at the end is the score. This value will be used to prefer values with a low score either because they are new
	or because they need to be learned more because they have often been wrong in past learning sessions.
	This results in optimal focus on expressions.

	To finish the learning session, write quit as input.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

// Define array of samples
struct sample {
	string it,ge;
	int value;
};

// Smaller equal function 
bool cmp(sample i, sample j) { return i.value<j.value; }

int main()
{
	// Variable declaration
	string aray;
	int number_lines = 0;
	string input;
	sample * minval;

	// Open file
	ifstream infile("library.txt");

	// Write data from file into array 
	while (getline(infile, aray))
	{
		number_lines++;
	}
	sample data[number_lines];
	infile.clear();
	infile.seekg(0, infile.beg);
	int iterate = 0;
	while (getline(infile, aray))
	{
		string delimiter = ",";
		string delimiter2 = ";";
		string token = aray.substr(0, aray.find(delimiter));
		string token2 = aray.substr(aray.find(delimiter) + 1, aray.find(delimiter2)-aray.find(delimiter)-1);
		string value = aray.substr(aray.find(delimiter2)+1,string::npos);
		data[iterate].it = token;
		data[iterate].ge = token2;
		string::size_type sz;
		int d;
		istringstream(value) >> d;
		data[iterate].value = d;
		++iterate;
	}

	// Close file
	infile.close();

	cout << endl << "Hi, nice to see you motivated to learn words!" << endl;
	cout << "So far, there are " << number_lines << " words in your list" << endl;

	int weaksamples[number_lines];
	int index;
	while (input != "quit")
	{
		// Define: In (percent)% of all cases, only use expressions with lowest score in library, otherwise use all expressions from library.
		srand(time(NULL));
		int old_new = rand() % 100;
		int iterweak = 0;
		int percent = 67;
		if (old_new <= percent) 
		{
			// Create index array with only low score expressions and choose random index out of it
			minval = min_element(data, data+number_lines, cmp);
			for (int i = 0; i <= number_lines; i++)
			{
				if (data[i].value == minval->value)
					weaksamples[iterweak++] = i;
			}
			int indexweak = rand() % iterweak;
			index = weaksamples[indexweak];
		}
		else {
			// Choose random index among all expressions
			index = rand() % number_lines;
		}

		cout << endl << data[index].ge << " = ";
		getline(cin, input);
		
		if (input != "quit")
		{
			// If input is correct, increase score of corresponding expression, otherwise show correct input and decrease score.
			if (input == data[index].it)
			{
				cout << "Correct!" << endl;
				data[index].value++;
			}
			else
			{
				cout << "Wrong, correct would be " << data[index].it << endl;
				if (data[index].value >= 1)
				{
					data[index].value--;
				}
			}
		}
		else {
			cout << endl <<  "Have a nice day." << endl << endl;
		}

	}

	// Overwrite library.txt file with improven data
	ofstream myfile("library.txt");
	for (int i = 0; i < number_lines; i++)
	{
		myfile << data[i].it << "," << data[i].ge << ";" << data[i].value << "\r\n";
	}
	myfile.close();



	return 0;
}
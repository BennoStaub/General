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

	Features:
	-In the beginning the user will be asked to learn expressions or add new ones. By choosing to add new expressions,
	 the new expressions can be directly input which will then be added to the library file as soon as the adding
	 session via input 'quit' is terminated.
	-During learning session it is possible to correct wrong saved expressions. As soon as the wrong expression occured, 'change'
	 can be used as input. This will open the possibility to change the expression and as soon as the change is accepted via 'yes' as input
	 the learning session continues.

	To finish the learning session, write quit as input. As remark, it will list all wrong answered expressions so that they can 
	looked at and learned for further progress.

	
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

// Define struct of samples
struct sample {
	string it,ge;
	int value;
};

// Smaller equal function 
bool cmp(sample i, sample j) { return i.value<j.value; }

int main()
{
	cout << "Hi, what do you want? \n[L]Learn words \n[A]Add words ";
	string action;
	getline(cin, action);
	if (action == "L") {



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
			string token2 = aray.substr(aray.find(delimiter) + 1, aray.find(delimiter2) - aray.find(delimiter) - 1);
			string value = aray.substr(aray.find(delimiter2) + 1, string::npos);
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

		// Initialize
		cout << endl << "Hi, nice to see you motivated to learn words!" << endl;
		cout << "So far, there are " << number_lines << " words in your list" << endl;
		cout << "To terminate the learning session, type 'quit' at any time." << endl;
		cout << "To change a wrong expression, type 'change' immediately." << endl;
		int weaksamples[number_lines];
		int wronganswered[100] = {0};
		int wrong = 0;
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
				minval = min_element(data, data + number_lines, cmp);
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
				// If input is correct, increase score of corresponding expression, otherwise show correct input,
				// decrease score and add word to list of wrong answered words.
				if (input == data[index].it)
				{
					cout << "Correct!" << endl;
					data[index].value++;
				}
				else
				{
					wronganswered[wrong] = index;
					wrong++;
					cout << "Wrong, correct would be " << data[index].it << endl;
					if (data[index].value >= 1)
					{
						data[index].value--;
					}
				}
				// Implement change option in case a mistake has been found during learn session.
				if (input == "change")
				{
					string change, changege, changeit;
					change = "change";
					while (change != "yes")
					{
						cout << "You want to change this word, please input the correct german word" << endl;
						getline(cin, changege);
						data[index].ge = changege;
						cout << "And now the correct italian word" << endl;
						getline(cin, changeit);
						data[index].it = changeit;
						cout << "Is this correct?[yes]" << endl << data[index].ge << " = " << data[index].it << endl;
						getline(cin, change);
					}
				}
			}
			else {
				// List all wrong answered words so that they can be learned for future sessions. (up to 100 words)
				cout << endl << "Have a nice day. Here are the words you answered wrong:" << endl << endl;
				for (int w=0; w <= 99; w++)
				{
					if (wronganswered[w] != 0)
					{
						cout << data[wronganswered[w]].ge << " = " << data[wronganswered[w]].it << endl;
					}
				}
			}

		}

		// Overwrite library.txt file with improven data
		ofstream myfile("library.txt");
		for (int i = 0; i < number_lines; i++)
		{
			myfile << data[i].it << "," << data[i].ge << ";" << data[i].value << "\r\n";
		}
		myfile.close();

		
	}
	// Implement option to add new words to library.
	else {
		ofstream myfile("library.txt", fstream::app);
		string itw, gew;
		while (itw != "quit") {
			cout << "Nice to see you have new expressions. To terminate the adding session, type 'quit' at any time." << endl;
			cout << "Italian word: ";
			getline(cin, itw);
			if (itw != "quit") {
				cout << "German word: ";
				getline(cin, gew);
				myfile << itw << "," << gew << ";0\r\n";
			}
		}
	
		myfile.close();


	}



	return 0;
}
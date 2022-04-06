/*implementation example of Field of Dreams game*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>


/* This function find input letter(inp) in word, if letter exist doing note to dictionary(dict)
 * and return 1, if not return 0 
 */
int Find(std::unordered_map<char, int>& dict, std::string& wrd, char inp)
{
	int res{0};
	for(size_t i=0; i < wrd.size(); i++)
	{
		if(wrd[i] == inp)
		{
			dict[inp] = 1; //note
			res = 1;
			return res;
		}
	}
	return res; //return(0), here we will come when inp not contained in worda and res not changed
};

/* This function run on word and check if letter from word noted in dictionary, and if it noted 
 * corresponding symbol in imagine opens*/
void Print(std::unordered_map<char, int>& dict, std::string& wrd, std::string& img)
{
	for(size_t i=0; i < wrd.size(); i++)
	{
		if(dict[wrd[i]]) //check existing
		{
			img[i] = wrd[i]; //open symbol
		}
	}
	std::cout << img << std::endl; //print imagine, does not matter with change or without
}

int main()
{
	std::unordered_map<char, int> dictionary{0}; //create dictionary and initialize all its value 0

	std::string word{"yakubovich"}; //assume that word generate by random
	std::string imagine;
	char tmp; //for input


	int life{3};

	for(auto x : word) //Here I created the imagine string consists of * symbols
	{		   //without an assumption abot Word length
		imagine.push_back('*');
	}

	std::cout << "        Welcome to the Field of Dreams game!\n"
	     << "           Try to guess the word "; 
	Print(dictionary, word, imagine); //dictionary at the moment didn't cnange, respectively Print 
					  //will print only **********

	while(life) //while life > 0
	{
		if(life == 1) //last chance
		{
			std::cout << "You have the last life, try to guess whole word(or lose): ";
			std::cin >> imagine; //read word from input to imagine, because imagine no longer needed
			std::cout << "word - " << word << std::endl; //show word
			std::cout << "you option - " << imagine << "\n\n"; //show entered value
			
			if(imagine != word) //and check for print: win or loose
			{
				std::cout << "You lose, I'am sorry.\n\n";
			}
			else
			{
				std::cout << "Congratulations, you win!!!\n\n";
			}
			return 0;

		}
		else  //regular iteration
		{
			std::cout << "Enter the letter: ";
			std::cin >> tmp; //read input

			if(!isalpha(tmp)) //Input error handling
			{
				std::cout << "Bad input, enter only lowercase letter!\n";
				break;
			}

			if(Find(dictionary, word, tmp)) //if Find return 1 then entered letter exists in the word
			{	
				std::cout << "Keep up the good work: ";
				Print(dictionary, word, imagine);
				std::cout << std::endl;
			}
			else //here if Find return 0 then entered letter not exists in the word
			{
				std::cout << "Thre are no letters (" << tmp << ") in the word\n\n";
				life--; //reduce life
			}
		}
	}
	return 1;
}

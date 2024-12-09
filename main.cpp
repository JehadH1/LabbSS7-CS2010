
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct funcObj
{
	bool operator()(const std::string &a, const std::string &b) { return a.length() < b.length(); }
};

using WORD = string;
using WORD_LIST = set<WORD>;
using CHAR_FREQ_LIST = map<char, int>;
using FREQ_WORD_LIST = multimap<int, WORD>;
using COUNTER = tuple<int, int, int>;
using LONGEST = priority_queue<WORD, vector<WORD>, funcObj>;

struct DICTION
{
	COUNTER stats;
	WORD_LIST words;
	CHAR_FREQ_LIST freqChar;
	FREQ_WORD_LIST freqWord;
	LONGEST longWord;
};

void printHeader(string);
void initlizeFreqChar(DICTION &);
void printFreqChar(DICTION &);
WORD getNextWord(DICTION &);
void printStats(DICTION &);
void fillDiction(DICTION &);
void printFreqWord(DICTION &);
void printHistogram(DICTION &);
void printChar(int, char);

int main()
{
	DICTION d;
	initlizeFreqChar(d);
	fillDiction(d);
	printStats(d);
	if (d.words.size() == 0)
	{
		cout << endl;
		cout << "There are no words in your file so we cant print anything else";
		cout << endl
			 << endl;
		return 0;
	}

	printFreqChar(d);
	printFreqWord(d);
	printHistogram(d);
	return 0;
}

//gets next word
WORD getNextWord(DICTION &d)
{
	WORD w;
	char ch;
	bool inWord = false;

	cin.get(ch);
	while (!cin.eof())
	{
		get<0>(d.stats)++; // inc char by 1
		if (ch == '\n')
		{
			get<2>(d.stats)++; // inc newline by 1
		}
		if (isalpha(ch)) // is ch in [A-Z, a-z]
		{

			w.push_back(ch); // start forming the word
			inWord = true;
			d.freqChar[(ch)]++; // inc the the value at the key of the char
		}
		else if (inWord)
		{
			get<1>(d.stats)++; // inc words by one
			return w;
		}
		cin.get(ch);
	}
	get<1>(d.stats)++;
	return w;
}

//fills dict
void fillDiction(DICTION &d)
{
	get<2>(d.stats) = 1;
	while (!cin.eof())
	{
		WORD currWord = getNextWord(d);

		// if non alpha char skip
		if (currWord == "")
		{
			continue;
		}

		// add the word to your word map;
		d.words.insert(currWord);
		bool newWord = true;
		// go through your d.freqWord
		for (auto i = d.freqWord.begin(); i != d.freqWord.end(); i++)
		{
			// if the word is currently in the freqword multmap
			// save the amount u have fot now
			// erase the element
			// add the element and the amount +1 back into your mult map
			// set new word false
			if (i->second == currWord)
			{
				int amount = i->first;
				d.freqWord.erase(i);
				d.freqWord.insert({amount + 1, currWord});
				newWord = false;
				break;
			}
		}
		// if its the first time for a word add it and have the amount be one
		// push your word into longWord so it cna be sorted
		if (newWord)
		{
			d.freqWord.insert({1, currWord});
			d.longWord.push(currWord);
		}
	}
}

//print header
void printHeader(string s)
{
	// prints header for the string i give it
	cout << endl;
	cout << "/";
	printChar(s.length() + 2, '-');
	cout << "\\" << endl;
	cout << "| " << s << " |" << endl;
	cout << "\\";
	printChar(s.length() + 2, '-');
	cout << '/';
	cout << endl;
	cout << endl;
}

//intilize FrecChar
void initlizeFreqChar(DICTION &d)
{
	// just initilzes my d.freqchar with 0 at first so we can have all letters printed
	for (char i = 'a'; i <= 'z'; i++)
	{
		d.freqChar[i] = 0;
		d.freqChar[toupper(i)] = 0;
	}
}

//print freqchar
void printFreqChar(DICTION &d)
{
	// print header
	printHeader("Letter Freq");

	int starNum;
	bool overTen;
	for (auto c : d.freqChar)
	{
		// prints th letter
		overTen = false;
		cout << '[' << c.first << ']' << " |";
		starNum = c.second;

		// if over ten freq for that letter set star amount to ten and ivertem = true
		if (starNum > 10)
		{
			starNum = 10;
			overTen = true;
		}
		// prints the stars
		for (int i = 0; i < starNum; i++)
		{
			cout << "*";
		}
		// overten = true print how many you ave in total
		if (overTen == true)
		{
			cout << " (" << c.second << ")";
		}
		cout << endl;
	}
}

//print stats
void printStats(DICTION &d)
{
	// print stats header
	printHeader("Stats");

	// print the words in the dict
	cout << "Words in dictionary - ";
	for (auto word : d.words)
	{
		cout << word << " ";
	}

	// print the stats
	cout << endl;
	cout << endl;
	cout << "Number of Chars   : " << get<0>(d.stats) << endl;
	cout << "Number of Words   : " << get<1>(d.stats) << endl;
	cout << "Number of Lines   : " << get<2>(d.stats) << endl;
}

//print freq word
void printFreqWord(DICTION &d)
{
	// print the header
	printHeader("Dictionary");
	cout << "Word                 Frequency" << endl;
	cout << "------------------------------" << endl;
	// go through freqword
	for (auto pair : d.freqWord)
	{
		// prints the word than a space then the number of time it shows up
		// looked up how to format cout so it looks nice
		cout << setw(14) << left << pair.second
			 << setw(2) << left << " "
			 << setw(14) << right << pair.first;
		cout << endl;
	}
}

//print histogram
void printHistogram(DICTION &d)
{
	// prints histogram header
	printHeader("Histogram");

	// makes a vector of pairs that has your freqWord elements
	// sorts the vector based on word size so i have my longest first
	vector<pair<int, WORD>> freqVec(d.freqWord.begin(), d.freqWord.end());
	sort(freqVec.begin(), freqVec.end(), [](const auto &a, const auto &b)
		 {
			 return a.second.length() > b.second.length(); // Sort by word length
		 });

	// just read it
	int most_stars = 0;
	int longetsWordLen = d.longWord.top().size();
	int offset = longetsWordLen + 2;
	int amountOfWords = d.longWord.size();

	// find the element with the most stars and set that number to be the max stars i have
	for (auto i : freqVec)
	{
		if (i.first >= most_stars)
			most_stars = i.first;
	}

	// first forloop will go through star amount as thast how long it will be
	for (int i = most_stars; i > 0; i--)
	{
		printChar(offset, ' ');

		// go through freqVec
		for (auto &j : freqVec)
		{
			// if my element.first (amount) == most_stars
			// print a star and then dec how many i have of that
			if (j.first == most_stars)
			{
				cout << "*";
				(j.first)--;
			}
			// else print space
			else
			{
				cout << " ";
			}
		}
		// then i want to decrease the most_stars as i went through all that had that value and i dont want to find it again.
		most_stars--;
		cout << endl;
	}
	// print the dash line
	printChar((offset + amountOfWords), '-');
	cout << endl;
	// print the offset
	printChar((offset), ' ');
	// print the ^ * how many words i have
	printChar(amountOfWords, '^');
	cout << endl;
	

	// just read it
	int amountOfDash = 0;
	int amountOfLine = amountOfWords;

	// forloop that will go amount of words
	for (int i = 0; i < amountOfWords; i++)
	{
		// print the freqVec[i].second (word)
		cout << freqVec[i].second;
		// amount of dash is offset - the lengthof the word
		// then print it
		amountOfDash = offset - freqVec[i].second.length();
		printChar(amountOfDash + i, '-');
		cout << ('/');
		// print the line (|) * amountofLines (wordamount) - i (number of )
		printChar(amountOfLine - i - 1, '|');
		cout << endl;
	}
	cout << endl;
	cout << "Longest Word is : " << d.longWord.top() << endl;
}

//print chars
void printChar(int n, char s)
{
	// will just print a char n times
	string ss(n, s);
	cout << ss;
}

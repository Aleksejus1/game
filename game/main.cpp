#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/* CONSTANTS */				
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))
#define STRING_UPPER(string) (transform(string.begin(), string.end(), string.begin(), ::toupper))
string
	answer_yes_s[] = { "YES", "Y", "YEAH", "YE", "YUP" },
	answer_no_s[] = { "NO", "N", "NAH", "NOPE" };
vector<string>
	answer_yes (answer_yes_s, answer_yes_s+ ARRAY_SIZE(answer_yes_s)),
	answer_no(answer_no_s, answer_no_s + ARRAY_SIZE(answer_no_s));
const int LINE_LENGTH = 80;

/* Struct initiation */		
struct option_;
struct page_;
struct menu_;
/* Functions initiation */	
bool defaultCondition();
bool stringInArray(string, vector<string>);
bool isNumberInBetween(int number, int betweenThis, int andThis);
bool isNumber(const std::string&);
void writeInMiddle(string);
void clear();
void pageInit();
void pause();

/* Enum definitions */		
enum pageTypes {
	PAGE_TYPE_DEFAULT,
	PAGE_TYPE_YESNO
};
/* Struct definitions */	
struct option_ {
	bool(*condition)() = defaultCondition;
	option_(string _name) {
		name = _name;
	}
	string name;
	page_* page;
};
struct page_ {

	pageTypes type = PAGE_TYPE_DEFAULT;
	vector<option_>
		options,
		viableOptions;
	string intro;

	int getOptionAnswer(vector<option_> _options) {
		int size = (int)_options.size();
		cout << "Select option by entering a number [1-" << size << "] : ";
		string answer; cin >> answer;
		if (isNumber(answer) && isNumberInBetween(stoi(answer), 1, size)) {
			return stoi(answer) - 1;
		}
		else {
			return -1;
		}
	}
	int getYesNoAnswer() {
		string answer;
		cout << "Choose Yes or No [Y/N]: ";
		cin >> answer;
		STRING_UPPER(answer);
		if (stringInArray(answer, answer_yes)) return 1;
		else if (stringInArray(answer, answer_no)) return 0;
		else return -1;
	}
	void addOption(string _name) {
		options.push_back(option_(_name));
	}
	void writeIntro() {
		writeInMiddle(intro);
		cout << endl;
	}
	vector<option_> writeOptions(vector<option_> _options) {
		vector<option_> validOptions;
		for (option_ &option : _options) {
			if (option.condition()) {
				writeInMiddle(option.name);
				cout << endl;
				validOptions.push_back(option);
			}
		}
		return validOptions;
	}
	int show() {
		int answeredOptionID = -1;
		do {
			clear();
			writeIntro();
			if (type == PAGE_TYPE_DEFAULT) {
				viableOptions = writeOptions(options);
				answeredOptionID = getOptionAnswer(viableOptions);
			}
			else if (type == PAGE_TYPE_YESNO) {
				answeredOptionID = getYesNoAnswer();
			}
		} while (answeredOptionID == -1);
		return answeredOptionID;
	}
};
struct menu_ {
	page_ main;
	menu_() {
		main.intro = "Welcome to a game";
		main.addOption("New Game");
		main.addOption("Continue");
		main.addOption("Load Game");
		main.addOption("Options");
		main.addOption("Exit");
	}
};

/* Global Variables */
page_ exitGame;
menu_ menu;

/* Functions definitions */	
int main() {
	pageInit();
	while (menu.main.viableOptions[menu.main.show()].name != "Exit");
	cout << exitGame.show();
	pause();
	return 0;
}
bool stringInArray(string _string, vector<string> _vector) {
	int length = (int)_vector.size();
	for (int counter = 0; counter < length; counter++) {
		if (_string == _vector[counter]) return true;
	}
	return false;
}
void pageInit() {
	exitGame.intro = "Are you sure you want to exit the game?";
	exitGame.type = PAGE_TYPE_YESNO;
}
bool defaultCondition() {
	return true;
}
void writeInMiddle(string _text) {
	int
		length = strlen(_text.c_str()),
		pos = (int)((LINE_LENGTH - length) / 2);
	for (int i = 0; i<pos; i++) cout << " ";
	cout << _text;
}
void clear() {
	system("cls");
}
void pause() {
	cout << endl << "------------------------------------------------" << endl;
	system("pause");
}
bool isNumber(const std::string& s){
	return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}
bool isNumberInBetween(int number, int betweenThis, int andThis) {
	return (number >= betweenThis && number <= andThis);
}
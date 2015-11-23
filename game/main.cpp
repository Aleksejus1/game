#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

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
const int currencyExchangeValue = 12; //12:1 a.k.a. 12 of lesser value = 1 of greater value

/* Struct initiation */			
struct option_;
struct page_;
struct meter_;
struct entity_;
struct money_;
struct player_;
struct area_;
struct menu_;
/* Functions initiation */		
bool defaultCondition();
bool stringInArray(string _string, vector<string> _vector);
string base12(unsigned long long _number);
bool isNumberInBetween(int number, int betweenThis, int andThis);
bool isNumber(const std::string& _string);
bool areThereAnySavedGames();
void writeInMiddle(string _text);
void clear();
void init();
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
	option_(string _name, bool(*_condition)() = defaultCondition) {
		name = _name;
		condition = _condition;
	}
	string name;
	page_* page;
};
struct money_{
	int sum = 0;
	string currencyIndicator;
	string getString() {
		return base12(sum);
	}
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
	page_* addOption(string _name, bool(*_condition)() = nullptr) {
		if(_condition == nullptr) options.push_back(option_(_name));
		else options.push_back(option_(_name, _condition));
		return this;
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
struct meter_ {
	float
		current,
		max;
	string outOf() {
		stringstream ss;
		ss << fixed << setprecision(2) << current << "/" << max;
		return ss.str();
	}
};
struct entity_ {
	meter_ health;
};
struct area_ {
	string name = "default";
	page_ page;
	area_(string _name) {
		if (_name != "") name = _name;
	}
};
struct menu_ {
	page_ main;
	menu_() {
		main.intro = "Welcome to a game";
		main
			.addOption("New Game")
			->addOption("Continue", areThereAnySavedGames)
			->addOption("Load Game", areThereAnySavedGames)
			->addOption("Options")
			->addOption("Exit");
	}
};
struct areas_ : vector<area_>{
	area_& add(string _name) {
		push_back((area_)(_name));
		return back();
	}
	void init() {
		add("Main");
		add("Forest");
		add("Village").page
			.addOption("Enter a Bar")
			->addOption("Enter the Sherifs' office")
			->addOption("Enter the mayor's office");
		add("Swamp");
		add("Cave");
	}
};

/* Global Variables */			
areas_ areas;
page_ exitGame;
menu_ menu;

/* Struct definitions part2 */	
struct player_ : entity_ {
	area_ *area = nullptr;
	money_ money;
	void init() {
		area = &areas[2];
		money.sum+=59;
	} // areas must be pre-initiated before calling this
};

/* Global Variables part2 */	
player_ player;

/* Functions definitions */ 			
int main() {
	init();
	/*game*/
	cout 
		<< "Health[" << player.health.outOf() << "]" 
		<< " - Area[" << player.area->name << "]"
		<< " - Souls[" << player.money.getString() << "]";
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
void init() {
	pageInit();
	areas.init();
	player.init();
}
string base12(unsigned long long _number) {
	if (_number < 12)
		return string() + "0123456789AB"[_number];
	return base12(_number / 12) + base12(_number % 12);
}
void pageInit() {
	exitGame.intro = "Are you sure you want to exit the game?";
	exitGame.type = PAGE_TYPE_YESNO;
}
bool areThereAnySavedGames() {
	return false;
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
bool isNumber(const std::string& _string){
	return !_string.empty() && find_if(_string.begin(), _string.end(), [](char c) { return !isdigit(c); }) == _string.end();
}
bool isNumberInBetween(int number, int betweenThis, int andThis) {
	return (number >= betweenThis && number <= andThis);
}
// HearthStats.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::this_thread;
using namespace std::chrono_literals;
using std::chrono::system_clock;
//using std::ostringstream;

typedef enum StateIDs{
	StartGame,
	RobbingBank,
	HavingGoodTime,
	LayingLow,
	Fleeing,
	BeingInJail,
	BeingTipsy
} State;

/*typedef enum ActionIDs {
	Start,
	GetRich,
	SpotCop,
	GetCaught,
	GetTired,
	GoToBar,
	FeelSafe,
	GetBored,
	Escape,
	TellTooMuch
} Action;*/

//Max value = 100 for wealth and strength, 10 for distanceCop
int wealth;
int distanceCop;
int strength;

int fps = 60;

State currentState = StartGame;
//Action currentAction;

string actionInput;
string actionPerformingText;

void playGame();

void checkMax();
void varChangeByState();
State stateManager(State beforeState, int currentWealth, int currentStrength, int currentDistanceCop);
//Action actionManager(int currentWealth, int currentStrength, int currentDistanceCop);
//State stateManager(State beforeState, Action performingAction);
string stateMessage();
//Action actionManager(string actionInputGiven);

int main() {
	srand(time(NULL));

	cout << "Press enter to begin." << endl;
	if (cin.get() == '\n') {
		varChangeByState();
		checkMax(); //Check whether the variables haven't exceeded their maximum and minimum values

		//currentAction = actionManager("");
		//currentAction = actionManager(currentState, wealth, strength, distanceCop);
		//currentState = stateManager(currentState, currentAction);
		currentState = stateManager(currentState, wealth, strength, distanceCop);
		cout << actionPerformingText << endl;
		cout << stateMessage() << endl << endl << endl;
		//cout << "test" << distanceCop << endl;
		playGame();
	}

	
}

void playGame() {
	//USER INPUT VERSION
	/*cout << "What's next? (type exit to quit the game)" << endl;
	getline(cin, actionInput);
	currentAction = actionManager(actionInput);*/

	for (int i = 1; i < fps; i++) {
		varChangeByState();
		checkMax(); //Check whether the variables haven't exceeded their maximum and minimum values
	}

	sleep_until(system_clock::now() + 2.5s);

	currentState = stateManager(currentState, wealth, strength, distanceCop);
	cout << "Wealth: " << wealth << endl << "Strength: " << strength << endl << "Distance to cop: " << distanceCop << endl << endl;
	cout << actionPerformingText << endl << endl;
	cout << stateMessage() << endl << endl << endl << endl;

	/*if (cin.get() == 'exit') {
		return;
	}
	else {*/
		//cout << "What's next? (type exit to quit the game)" << endl;
		playGame();
	//}
}

void checkMax() {
	if(strength > 100) {
		strength = 100;
	} else if(strength < 0) {
		strength = 0;
	}

	if(distanceCop > 10) {
		distanceCop = 10;
	} else if(distanceCop < 0) {
		distanceCop = 0;
	}

	if(wealth > 100) {
		wealth = 100;
	} else if(wealth < 0) {
		wealth = 0;
	}
}

void varChangeByState(){
	switch(currentState) {
		case StartGame:
			wealth = 0;
			strength = 100;
			distanceCop = rand() % 9;

			break;
		case RobbingBank:
			wealth += rand() % 10 + 1;
			strength--;
			distanceCop = rand() % 9;

			break;
		case HavingGoodTime:
			wealth -= rand() % 8 + 1;
			strength -= rand() % 8 + 1;
			distanceCop = rand() % 9;

			break;
		case LayingLow:
			wealth = wealth;
			strength += rand() % 10 + 1;
			distanceCop = 5;

			break;
		case Fleeing:
			wealth--;
			strength -= rand() % 8 + 1;
			distanceCop = rand() % 9;

			break;
		case BeingInJail:
			wealth = 0;
			strength += rand() % 2 + 1;
			distanceCop = 0;

			break;
		case BeingTipsy:
			wealth -= rand() % 2 + 1;
			strength -= rand() % 2 + 1;
			distanceCop = rand() % 4;

			break;
		default:
			wealth = 0;
			strength = 100;
			distanceCop = rand() % 9;
	}
}

State stateManager(State beforeState, int currentWealth, int currentStrength, int currentDistanceCop) {
	State outputState;

	switch (beforeState) {
		case StartGame:
			if (wealth == 0 && strength == 100) {
				outputState = RobbingBank;
				actionPerformingText = "'Let's start this'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'I'm so confused'";
			}
			break;
		case RobbingBank:
			if (wealth >= 60 && distanceCop > 5) {
				//GetRich
				outputState = HavingGoodTime;
				actionPerformingText = "'I'm rich enough to have a good time'";
			}
			else if (distanceCop > 0 && distanceCop <= 5) {
				//SpotCop
				outputState = Fleeing;
				actionPerformingText = "'I see a cop, so I have to start running'";
			}
			else if (distanceCop == 0) {
				//GetCaught
				outputState = BeingInJail;
				actionPerformingText = "'Oh no, the cops are bringing me to my new home!'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'Let's try again'";
			}
			break;
		case HavingGoodTime:
			if (wealth >= 40 && wealth <= 70 && strength < 40) {
				//GetTired
				outputState = LayingLow;
				actionPerformingText = "'I'm getting very tired, so I better lay low for a while'";
			}
			else if (distanceCop == 0) {
				//SpotCop
				outputState = Fleeing;
				actionPerformingText = "'I see a cop, so I have to start running'";
			}
			else if (strength < 40) {
				//GoToBar
				outputState = BeingTipsy;
				actionPerformingText = "'All drinks on me!'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'Let's try again'";
			}
			break;
		case LayingLow:
			if (strength >= 65 && distanceCop >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				actionPerformingText = "'Let’s get back to what I was doing'";
			}
			else if (strength < 65 && wealth >= 40 && wealth <= 70) {
				//GetBored
				outputState = BeingTipsy;
				actionPerformingText = "'A bar is a good safe spot, right? Right??'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'Let's try again'";
			}
			break;
		case Fleeing:
			if (distanceCop < 7 && distanceCop > 0) {
				//GetTired
				outputState = LayingLow;
				actionPerformingText = "'I'm getting very tired, so I better lay low for a while'";
			}
			else if (distanceCop >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				actionPerformingText = "'Let’s get back to what I was doing'";
			}
			else if (distanceCop == 0 && strength < 50) {
				//GetCaught
				outputState = BeingInJail;
				actionPerformingText = "'Oh no, the cops are bringing me to my new home!'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'Let's try again'";
			}
			break;
		case BeingInJail:
			if (strength >= 70) {
				//Escape
				outputState = Fleeing;
				actionPerformingText = "'RUUUNNN!! RUUUUUUUUNNNN!!!'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'Let's try again'";
			}
			break;
		case BeingTipsy:
			if (strength < 40) {
				//Tell too much
				outputState = BeingInJail;
				actionPerformingText = "'No! Wait! Those were just stories!'";
			}
			else if (strength >= 40 && wealth < 40) {
				//Get bored
				outputState = RobbingBank;
				actionPerformingText = "'You know what would be funny..? If we go ‘borrow’ something from the bank…hehe'";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "'Let's try again'";
			}
			break;
		default:
			outputState = StartGame;
			actionPerformingText = "'Let's start over'";
	}

	/*switch(beforeState) {
		case StartGame:
			if (performingAction == Start) {
				outputState = RobbingBank;
				actionPerformingText = "'Let's start this'";
			}
			else {
				outputState = StartGame;
				actionPerformingText = "'I'm so confused'";
			}
			break;
		case RobbingBank:
			switch (performingAction) {
				case GetRich:
					outputState = HavingGoodTime;
					actionPerformingText = "'I'm rich enough to have a good time'";
					break;
				case SpotCop:
					outputState = Fleeing;
					actionPerformingText = "'I see a cop, so I have to start running'";
					break;
				case GetCaught:
					outputState = BeingInJail;
					actionPerformingText = "'Oh no, the cops are bringing me to my new home!'";
					break;
				default:
					outputState = RobbingBank;
					actionPerformingText = "'Let's try again'";
			}
			break;
		case HavingGoodTime:
			switch (performingAction) {
				case GetTired:
					outputState = LayingLow;
					actionPerformingText = "'I'm getting very tired, so I better lay low for a while'";
					break;
				case SpotCop:
					outputState = Fleeing;
					actionPerformingText = "'I see a cop, so I have to start running'";
					break;
				case GoToBar:
					outputState = BeingTipsy;
					actionPerformingText = "'All drinks on me!'";
					break;
				default:
					outputState = HavingGoodTime;
					actionPerformingText = "'Let's try again'";
			}
			break;
		case LayingLow:
			switch (performingAction) {
				case FeelSafe:
					outputState = RobbingBank;
					actionPerformingText = "'Let’s get back to what I was doing'";
					break;
				case GetBored:
					outputState = BeingTipsy;
					actionPerformingText = "'A bar is a good safe spot, right? Right??'";
					break;
				default:
					outputState = LayingLow;
					actionPerformingText = "'Let's try again'";
			}
			break;
		case Fleeing:
			switch (performingAction) {
				case GetTired:
					outputState = LayingLow;
					actionPerformingText = "'I'm getting very tired, so I better lay low for a while'";
					break;
				case FeelSafe:
					outputState = RobbingBank;
					actionPerformingText = "'Let’s get back to what I was doing'";
					break;
				case GetCaught:
					outputState = BeingInJail;
					actionPerformingText = "'Oh no, the cops are bringing me to my new home!'";
					break;
				default:
					outputState = Fleeing;
					actionPerformingText = "'Let's try again'";
			}
			break;
		case BeingInJail:
			switch (performingAction) {
				case Escape:
					outputState = Fleeing;
					actionPerformingText = "'RUUUNNN!! RUUUUUUUUNNNN!!!'";
					break;
				default:
					outputState = BeingInJail;
					actionPerformingText = "'Let's try again'";
			}
			break;
		case BeingTipsy:
			switch (performingAction) {
				case TellTooMuch:
					outputState = BeingInJail;
					actionPerformingText = "'No! Wait! Those were just stories!'";
					break;
				case GetBored:
					outputState = RobbingBank;
					actionPerformingText = "'You know what would be funny..? If we go ‘borrow’ something from the bank…hehe'";
					break;
				default:
					outputState = BeingTipsy;
					actionPerformingText = "'Let's try again'";
			}
			break;
		default:
			outputState = StartGame;
			actionPerformingText = "'Let's start over'";
	}*/

	return outputState;
}

string stateMessage() {
	string outputStateMessage;

	switch (currentState) {
		case RobbingBank:
			outputStateMessage = "'I'm robbing banks and getting loads of money! Pew pew!'";
			break;
		case HavingGoodTime:
			outputStateMessage = "'I'm having a good time spending my money'";
			break;
		case LayingLow:
			outputStateMessage = "'I’ll just wait a minute..'";
			break;
		case Fleeing:
			outputStateMessage = "'Who said being a thief isn’t a sport? I’m running all day!'";
			break;
		case BeingInJail:
			outputStateMessage = "'So when can I go home? Jail is stupid.'";
			break;
		case BeingTipsy:
			outputStateMessage = "'Glug..glug…. hmmmm……. beeeer'";
			break;
		default:
			outputStateMessage = "'I am so confused'";
	}

	return outputStateMessage;
}

/*Action actionManager(string actionInputGiven) {
Action actionManager(State beforeState, int currentWealth, int currentStrength, int currentDistanceCop) {
	Action actionOutput;

	/*if (actionInput == "Get rich") {
		actionOutput = GetRich;
	}
	else if (actionInput == "Spot cop") {
		actionOutput = SpotCop;
	}
	else if (actionInput == "Get caught") {
		actionOutput = GetCaught;
	}
	else if (actionInput == "Get tired") {
		actionOutput = GetTired;
	}
	else if (actionInput == "Go to bar") {
		actionOutput = GoToBar;
	}
	else if (actionInput == "Feel safe") {
		actionOutput = FeelSafe;
	}
	else if (actionInput == "Get bored") {
		actionOutput = GetBored;
	}
	else if (actionInput == "Escape") {
		actionOutput = Escape;
	}
	else if (actionInput == "Tell too much") {
		actionOutput = TellTooMuch;
	}
	else {
		actionOutput = Start;
	}

	return actionOutput;
}*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

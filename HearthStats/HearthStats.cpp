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

typedef enum StateIDs{
	StartGame,
	RobbingBank,
	HavingGoodTime,
	LayingLow,
	Fleeing,
	BeingInJail,
	BeingTipsy
} State;

//Max value = 100 for wealth and strength, 10 for distanceCop
int wealth;
int distanceCop;
int strength;

int fps = 60;

State currentState = StartGame;

string actionInput;
string actionPerformingText;

void playGame();

void checkMax();
void varChangeByState();
State stateManager(State beforeState, int currentWealth, int currentStrength, int currentDistanceCop);
string stateMessage();

int main() {
	srand(time(NULL));

	cout << "Press enter to begin." << endl;
	if (cin.get() == '\n') {
		varChangeByState();
		checkMax(); //Check whether the variables haven't exceeded their maximum and minimum values

		currentState = stateManager(currentState, wealth, strength, distanceCop);
		cout << actionPerformingText << endl;
		cout << stateMessage() << endl << endl << endl;
		playGame();
	}

	
}

void playGame() {
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
			distanceCop = rand() % 4 + 7;

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
				actionPerformingText = "\"Let\'s start this\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"I\'m so confused\" + ERROR CHECK STARTGAME";
			}
			break;
		case RobbingBank:
			if (wealth >= 60 && distanceCop > 5) {
				//GetRich
				outputState = HavingGoodTime;
				actionPerformingText = "\"I\'m rich enough to have a good time\"";
			}
			else if (distanceCop > 0 && distanceCop <= 5) {
				//SpotCop
				outputState = Fleeing;
				actionPerformingText = "\"I see a cop, so I have to start running\"";
			}
			else if (distanceCop == 0) {
				//GetCaught
				outputState = BeingInJail;
				actionPerformingText = "\"Oh no, the cops are bringing me to my new home!\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"Let\'s try again\" + ERROR CHECK ROBBINGBANK";
			}
			break;
		case HavingGoodTime:
			if (wealth >= 40 && wealth <= 70 && strength < 40) {
				//GetTired
				outputState = LayingLow;
				actionPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
			}
			else if (distanceCop == 0) {
				//SpotCop
				outputState = Fleeing;
				actionPerformingText = "\"I see a cop, so I have to start running\"";
			}
			else if (strength < 40) {
				//GoToBar
				outputState = BeingTipsy;
				actionPerformingText = "\"All drinks on me!\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"Let\'s try again\" + ERRO CHECK HAVINGGOODTIME";
			}
			break;
		case LayingLow:
			if (strength >= 65 && distanceCop >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				actionPerformingText = "\"Let\'s get back to what I was doing\"";
			}
			else if (strength < 65 && wealth >= 40 && wealth <= 70) {
				//GetBored
				outputState = BeingTipsy;
				actionPerformingText = "\"A bar is a good safe spot, right? Right??\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"Let\'s try again\" + ERROR CHECK LAYINGLOW";
			}
			break;
		case Fleeing:
			if (distanceCop < 7 && distanceCop > 0) {
				//GetTired
				outputState = LayingLow;
				actionPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
			}
			else if (distanceCop >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				actionPerformingText = "\"Let\'s get back to what I was doing\"";
			}
			else if (distanceCop == 0 && strength < 50) {
				//GetCaught
				outputState = BeingInJail;
				actionPerformingText = "\"Oh no, the cops are bringing me to my new home!\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"Let\'s try again\" + ERROR CHECK FLEEING";
			}
			break;
		case BeingInJail:
			if (strength >= 70) {
				//Escape
				outputState = Fleeing;
				actionPerformingText = "\"RUUUNNN!! RUUUUUUUUNNNN!!!\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"Let\'s try again\" + ERROR CHECK BEINGINJAIL";
			}
			break;
		case BeingTipsy:
			if (strength < 40) {
				//Tell too much
				outputState = BeingInJail;
				actionPerformingText = "\"No! Wait! Those were just stories!\"";
			}
			else if (strength >= 40 && wealth < 40) {
				//Get bored
				outputState = RobbingBank;
				actionPerformingText = "\"You know what would be funny..? If we go \'borrow\' something from the bank…hehe\"";
			}
			else {
				outputState = beforeState;
				actionPerformingText = "\"Let\'s try again\"";
			}
			break;
		default:
			outputState = StartGame;
			actionPerformingText = "\"Let\'s start over\" + ERROR CHECK DEFAULTSTATE";
	}

	return outputState;
}

string stateMessage() {
	string outputStateMessage;

	switch (currentState) {
		case RobbingBank:
			outputStateMessage = "\"'I\'m robbing banks and getting loads of money! Pew pew!\"";
			break;
		case HavingGoodTime:
			outputStateMessage = "\"I\'m having a good time spending my money\"";
			break;
		case LayingLow:
			outputStateMessage = "\"I\'ll just wait a minute..\"";
			break;
		case Fleeing:
			outputStateMessage = "\"Who said being a thief isn\'t a sport? I\'m running all day!\"";
			break;
		case BeingInJail:
			outputStateMessage = "\"So when can I go home? Jail is stupid.\"";
			break;
		case BeingTipsy:
			outputStateMessage = "\"Glug..glug.... hmmmm..... beeeer\"";
			break;
		default:
			outputStateMessage = "\"I am so confused\" + ERROR CHECK DEFAULTMESSAGE";
	}

	return outputStateMessage;
}
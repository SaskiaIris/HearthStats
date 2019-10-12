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

typedef enum RobberStateIDs{
	StartGameRobber,
	RobbingBank,
	HavingGoodTime,
	LayingLow,
	Fleeing,
	BeingInJail,
	BeingTipsy
} RobberState;

typedef enum CopStateIDs {
	StartGameCop,
	OnStakeOut,
	Chasing,
	OffDuty
} CopState;

int fps = 60;

//Max value = 100 for wealth and strength, 10 for distanceCop, values for the thief
int wealth;
int distanceCop;
int strength;

//The dutyTime has a max value of 10
int dutyTime;

RobberState currentRobberState = StartGameRobber;
CopState currentCopState = StartGameCop;

string actionRobberPerformingText;
string actionCopPerformingText;

void playGame();

void checkMax(); //Check whether the variables haven't exceeded their maximum and minimum values

void varChangeByStateRobber();
RobberState stateRobberManager(RobberState beforeState, int currentWealth, int currentStrength, int currentDistanceCop);
string stateRobberMessage();


void varChangeByStateCop();
CopState stateCopManager(CopState beforeState, int currentDutyTime);
string stateCopMessage();

int main() {
	srand(time(NULL));

	cout << "Press enter to begin." << endl;
	if (cin.get() == '\n') {
		varChangeByStateRobber();
		checkMax();

		currentRobberState = stateRobberManager(currentRobberState, wealth, strength, distanceCop);
		cout << actionRobberPerformingText << endl;
		cout << stateRobberMessage() << endl << endl << endl;
		playGame();
	}

	
}

void playGame() {
	for (int i = 1; i < fps; i++) {
		varChangeByStateRobber();
		checkMax(); //Check whether the variables haven't exceeded their maximum and minimum values
	}

	sleep_until(system_clock::now() + 2.5s);

	currentRobberState = stateRobberManager(currentRobberState, wealth, strength, distanceCop);
	cout << "Wealth: " << wealth << endl << "Strength: " << strength << endl << "Distance to cop: " << distanceCop << endl << endl;
	cout << actionRobberPerformingText << endl << endl;
	cout << stateRobberMessage() << endl << endl << endl << endl;

	if (cin.get() == 'exit') {
		return;
	}
	else {
		//cout << "What's next? (type exit to quit the game)" << endl;
		playGame();
	}
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

	if (dutyTime > 10) {
		dutyTime = 10;
	}
	else if(dutyTime < 0) {
		dutyTime = 0;
	}
}

void varChangeByStateRobber(){
	switch(currentRobberState) {
		case StartGameRobber:
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

RobberState stateRobberManager(RobberState beforeState, int currentWealth, int currentStrength, int currentDistanceCop) {
	RobberState outputState;

	switch (beforeState) {
		case StartGameRobber:
			if (currentWealth == 0 && currentStrength == 100) {
				outputState = RobbingBank;
				actionRobberPerformingText = "\"Let\'s start this\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"I\'m so confused\" + ERROR CHECK STARTGAME";
			}
			break;
		case RobbingBank:
			if (currentWealth >= 60 && currentDistanceCop > 5) {
				//GetRich
				outputState = HavingGoodTime;
				actionRobberPerformingText = "\"I\'m rich enough to have a good time\"";
			}
			else if (currentDistanceCop > 0 && currentDistanceCop <= 5) {
				//SpotCop
				outputState = Fleeing;
				actionRobberPerformingText = "\"I see a cop, so I have to start running\"";
			}
			else if (currentDistanceCop == 0) {
				//GetCaught
				outputState = BeingInJail;
				actionRobberPerformingText = "\"Oh no, the cops are bringing me to my new home!\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"Let\'s try again\" + ERROR CHECK ROBBINGBANK";
			}
			break;
		case HavingGoodTime:
			if (currentWealth >= 40 && currentWealth <= 70 && currentStrength < 40) {
				//GetTired
				outputState = LayingLow;
				actionRobberPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
			}
			else if (currentDistanceCop == 0) {
				//SpotCop
				outputState = Fleeing;
				actionRobberPerformingText = "\"I see a cop, so I have to start running\"";
			}
			else if (currentStrength < 40) {
				//GoToBar
				outputState = BeingTipsy;
				actionRobberPerformingText = "\"All drinks on me!\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"Let\'s try again\" + ERRO CHECK HAVINGGOODTIME";
			}
			break;
		case LayingLow:
			if (currentStrength >= 65 && currentDistanceCop >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				actionRobberPerformingText = "\"Let\'s get back to what I was doing\"";
			}
			else if (currentStrength < 65 && currentWealth >= 40 && currentWealth <= 70) {
				//GetBored
				outputState = BeingTipsy;
				actionRobberPerformingText = "\"A bar is a good safe spot, right? Right??\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"Let\'s try again\" + ERROR CHECK LAYINGLOW";
			}
			break;
		case Fleeing:
			if (currentDistanceCop < 7 && currentDistanceCop > 0) {
				//GetTired
				outputState = LayingLow;
				actionRobberPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
			}
			else if (currentDistanceCop >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				actionRobberPerformingText = "\"Let\'s get back to what I was doing\"";
			}
			else if (currentDistanceCop == 0 && currentStrength < 50) {
				//GetCaught
				outputState = BeingInJail;
				actionRobberPerformingText = "\"Oh no, the cops are bringing me to my new home!\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"Let\'s try again\" + ERROR CHECK FLEEING";
			}
			break;
		case BeingInJail:
			if (currentStrength >= 70) {
				//Escape
				outputState = Fleeing;
				actionRobberPerformingText = "\"RUUUNNN!! RUUUUUUUUNNNN!!!\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"Let\'s try again\" + ERROR CHECK BEINGINJAIL";
			}
			break;
		case BeingTipsy:
			if (currentStrength < 40) {
				//Tell too much
				outputState = BeingInJail;
				actionRobberPerformingText = "\"No! Wait! Those were just stories!\"";
			}
			else if (currentStrength >= 40 && currentWealth < 40) {
				//Get bored
				outputState = RobbingBank;
				actionRobberPerformingText = "\"You know what would be funny..? If we go \'borrow\' something from the bank…hehe\"";
			}
			else {
				outputState = beforeState;
				actionRobberPerformingText = "\"Let\'s try again\"";
			}
			break;
		default:
			outputState = StartGameRobber;
			actionRobberPerformingText = "\"Let\'s start over\" + ERROR CHECK DEFAULTSTATE";
	}

	return outputState;
}

string stateRobberMessage() {
	string outputStateMessage;

	switch (currentRobberState) {
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
			outputStateMessage = "\"I am so confused\" + ERROR CHECK DEFAULTMESSAGEROBBER";
	}

	return outputStateMessage;
}

void varChangeByStateCop() {
	switch (currentCopState) {
		case StartGameCop:
			dutyTime = 0;

			break;
		case OffDuty:
			dutyTime -= rand() % 2;

			break;
		case OnStakeOut:
			dutyTime += rand() % 2;

			break;
		case Chasing:
			dutyTime += rand() % 2;
			distanceCop--;

			break;
		default:
			dutyTime = 0;
	}
}

CopState stateCopManager(CopState beforeState, int currentDutyTime) {
	CopState outputState;

	switch (beforeState) {
	case StartGameCop:
		if (dutyTime < 1) {
			outputState = OnStakeOut;
			actionCopPerformingText = "\"Let\'s start this\"";
		}
		else {
			outputState = beforeState;
			actionCopPerformingText = "\"I\'m so confused\" + ERROR CHECK STARTGAME";
		}
		break;
	case OnStakeOut:
		if (distanceCop < 5) {
			outputState = Chasing;
			actionCopPerformingText = "\"I\'m rich enough to have a good time\"";
		}
		else {
			outputState = beforeState;
			actionCopPerformingText = "\"Let\'s try again\" + ERROR CHECK ROBBINGBANK";
		}
		break;
	case Chasing:
		if (dutyTime > 9 && currentRobberState != BeingInJail) {
			outputState = OffDuty;
			actionCopPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
		}
		else if (dutyTime > 9 && currentRobberState == BeingInJail) {
			outputState = OffDuty;
			actionCopPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
		}
		else {
			outputState = beforeState;
			actionCopPerformingText = "\"Let\'s try again\" + ERRO CHECK HAVINGGOODTIME";
		}
		break;
	case OffDuty:
		if (dutyTime = 0) {
			outputState = OnStakeOut;
			actionCopPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
		}
		else {
			outputState = beforeState;
			actionCopPerformingText = "\"Let\'s try again\" + ERRO CHECK HAVINGGOODTIME";
		}
		break;
	default:
		outputState = StartGameCop;
		actionCopPerformingText = "\"Let\'s start over\" + ERROR CHECK DEFAULTSTATE";
	}

	return outputState;
}

string stateCopMessage() {
	string outputStateMessage;

	switch (currentCopState) {
		case OffDuty:
			outputStateMessage = "\"'I\'m robbing banks and getting loads of money! Pew pew!\"";
			break;
		case OnStakeOut:
			outputStateMessage = "\"I\'m having a good time spending my money\"";
			break;
		case Chasing:
			outputStateMessage = "\"Hold it right there buddy\"";
			break;
		default:
			outputStateMessage = "\"I need a donut\" + ERROR CHECK DEFAULTMESSAGECOP";
	}

	return outputStateMessage;
}
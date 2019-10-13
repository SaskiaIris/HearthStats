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

//Max value = 100 for robberWealth and robberStrength, 10 for distanceCopToRobber, values for the thief
int robberWealth;
int distanceCopToRobber;
int robberStrength;

//The dutyTimeCop has a max value of 10
int dutyTimeCop;

RobberState activeRobberState = StartGameRobber;
CopState activeCopState = StartGameCop;

string robberActionPerformingText;
string copActionPerformingText;

void playGame();

int randomNumberGenerator(int minimumValue, int maximumValue); //both inclusive
void checkMinMax(); //Check whether the variables haven't exceeded their maximum and minimum values

void varChangeByState();

RobberState robberStateManager(RobberState currentState, int currentWealth, int currentStrength, int currentDistanceCopToRobber);
string stateRobberMessage();

CopState copStateManager(CopState currentState, RobberState currentRobberState, int currentDutyTime, int currentDistanceCopToRobber);
string stateCopMessage();

int main() {
	srand(time(NULL));

	cout << "Press enter to begin." << endl;
	if (cin.get() == '\n') {
		playGame();
	}

}

void playGame() {
	varChangeByState();
	checkMinMax(); //Check whether the variables haven't exceeded their maximum and minimum values

	sleep_until(system_clock::now() + 2.5s);

	activeRobberState = robberStateManager(activeRobberState, robberWealth, robberStrength, distanceCopToRobber);
	activeCopState = copStateManager(activeCopState, activeRobberState, dutyTimeCop, distanceCopToRobber);

	cout << "_______________________________ ROBBER _______________________________" << endl;
	cout << "| Wealth: " << robberWealth << endl <<
		"| Strength: " << robberStrength << endl <<
		"| Distance to cop: " << distanceCopToRobber << endl << endl;
	cout << "| Robber: " << robberActionPerformingText << endl;
	cout << "| Robber while " << stateRobberMessage() << endl << endl << endl << endl;

	cout << "_________________________________ COP ________________________________" << endl;
	cout << "| Duty time: " << dutyTimeCop << endl << endl;
	cout << "| Cop: " << copActionPerformingText << endl;
	cout << "| Cop while " << stateCopMessage() << endl << endl << endl << endl;

	if (activeRobberState == BeingInJail && activeCopState == OffDuty) {
		return;
	}
	else {
		playGame();
	}
}

int randomNumberGenerator(int minimumValue, int maximumValue) {
	int randomNumberOutput = 0;

	randomNumberOutput = rand() % ((maximumValue-minimumValue)+1) + minimumValue;

	return randomNumberOutput;
}

void checkMinMax() {
	if(robberStrength > 100) {
		robberStrength = 100;
	} else if(robberStrength < 0) {
		robberStrength = 0;
	}

	if(distanceCopToRobber > 10) {
		distanceCopToRobber = 10;
	} else if(distanceCopToRobber < 0) {
		distanceCopToRobber = 0;
	}

	if(robberWealth > 100) {
		robberWealth = 100;
	} else if(robberWealth < 0) {
		robberWealth = 0;
	}

	if (dutyTimeCop > 10) {
		dutyTimeCop = 10;
	}
	else if(dutyTimeCop < 0) {
		dutyTimeCop = 0;
	}
}

void varChangeByState(){
	switch(activeRobberState) {
		case StartGameRobber:
			robberWealth = 0;
			robberStrength = 100;
			distanceCopToRobber = randomNumberGenerator(0, 10);

			break;
		case RobbingBank:
			robberWealth = randomNumberGenerator(40, 100);
			robberStrength -= randomNumberGenerator(15, 60);
			distanceCopToRobber = randomNumberGenerator(0, 10);

			break;
		case HavingGoodTime:
			robberWealth -= randomNumberGenerator(15, 60);
			robberStrength -= randomNumberGenerator(15, 60);
			distanceCopToRobber = randomNumberGenerator(0, 10);

			break;
		case LayingLow:
			robberWealth = robberWealth;
			robberStrength += randomNumberGenerator(40, 100);
			distanceCopToRobber = randomNumberGenerator(4, 7);

			break;
		case Fleeing:
			robberWealth--;
			robberStrength -= randomNumberGenerator(60, 80);
			distanceCopToRobber = randomNumberGenerator(0, 10);

			break;
		case BeingInJail:
			robberWealth = 0;
			robberStrength = randomNumberGenerator(40, 100);
			distanceCopToRobber = 0;

			break;
		case BeingTipsy:
			robberWealth -= randomNumberGenerator(15, 60);
			robberStrength -= randomNumberGenerator(15, 60);
			distanceCopToRobber = randomNumberGenerator(0, 4);

			break;
		default:
			robberWealth = 0;
			robberStrength = 100;
			distanceCopToRobber = randomNumberGenerator(0, 10);
	}

	switch (activeCopState) {
		case StartGameCop:
			dutyTimeCop = 0;

			break;
		case OffDuty:
			dutyTimeCop -= randomNumberGenerator(3, 5);

			break;
		case OnStakeOut:
			dutyTimeCop += randomNumberGenerator(3, 5);

			break;
		case Chasing:
			dutyTimeCop += randomNumberGenerator(3, 5);
			distanceCopToRobber--;

			break;
		default:
			dutyTimeCop = 0;
	}
}

RobberState robberStateManager(RobberState currentState, int currentWealth, int currentStrength, int currentDistanceCopToRobber) {
	RobberState outputState;

	switch (currentState) {
		case StartGameRobber:
			if (currentWealth == 0 && currentStrength == 100) {
				outputState = RobbingBank;
				robberActionPerformingText = "\"Let\'s start this\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"I\'m so confused\" + ERROR CHECK STARTGAME";
			}
			break;
		case RobbingBank:
			if (currentWealth >= 60 && currentDistanceCopToRobber > 5) {
				//GetRich
				outputState = HavingGoodTime;
				robberActionPerformingText = "\"I\'m rich enough to have a good time\"";
			}
			else if (currentDistanceCopToRobber > 0 && currentDistanceCopToRobber <= 5) {
				//SpotCop
				outputState = Fleeing;
				robberActionPerformingText = "\"I see a cop, so I have to start running\"";
			}
			else if (currentDistanceCopToRobber == 0) {
				//GetCaught
				outputState = BeingInJail;
				robberActionPerformingText = "\"Oh no, the cops are bringing me to my new home!\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"Let\'s try again\" + ERROR CHECK ROBBINGBANK";
			}
			break;
		case HavingGoodTime:
			if (currentWealth >= 40 && currentWealth <= 70 && currentStrength < 40) {
				//GetTired
				outputState = LayingLow;
				robberActionPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
			}
			else if (currentDistanceCopToRobber == 0) {
				//SpotCop
				outputState = Fleeing;
				robberActionPerformingText = "\"I see a cop, so I have to start running\"";
			}
			else if (currentStrength < 40) {
				//GoToBar
				outputState = BeingTipsy;
				robberActionPerformingText = "\"All drinks on me!\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"Let\'s try again\" + ERRO CHECK HAVINGGOODTIME";
			}
			break;
		case LayingLow:
			if (currentStrength >= 65 && currentDistanceCopToRobber >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				robberActionPerformingText = "\"Let\'s get back to what I was doing\"";
			}
			else if (currentStrength < 65 && currentWealth >= 40 && currentWealth <= 70) {
				//GetBored
				outputState = BeingTipsy;
				robberActionPerformingText = "\"A bar is a good safe spot, right? Right??\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"It would be better if I keep hiding for now\"";
				//robberActionPerformingText = "\"Let\'s try again\" + ERROR CHECK LAYINGLOW";
			}
			break;
		case Fleeing:
			if (currentDistanceCopToRobber < 7 && currentDistanceCopToRobber > 0) {
				//GetTired
				outputState = LayingLow;
				robberActionPerformingText = "\"I\'m getting very tired, so I better lay low for a while\"";
			}
			else if (currentDistanceCopToRobber >= 7) {
				//FeelSafe
				outputState = RobbingBank;
				robberActionPerformingText = "\"Let\'s get back to what I was doing\"";
			}
			else if (currentDistanceCopToRobber == 0 && currentStrength < 50) {
				//GetCaught
				outputState = BeingInJail;
				robberActionPerformingText = "\"Oh no, the cops are bringing me to my new home!\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"Let\'s try again\" + ERROR CHECK FLEEING";
			}
			break;
		case BeingInJail:
			if (currentStrength >= 70) {
				//Escape
				outputState = Fleeing;
				robberActionPerformingText = "\"RUUUNNN!! RUUUUUUUUNNNN!!!\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"...still not out of jail :(\"";
				//robberActionPerformingText = "\"Let\'s try again\" + ERROR CHECK BEINGINJAIL";
			}
			break;
		case BeingTipsy:
			if (currentStrength < 40) {
				//Tell too much
				outputState = BeingInJail;
				robberActionPerformingText = "\"No! Wait! Those were just stories!\"";
			}
			else if (currentStrength >= 40 && currentWealth < 40) {
				//Get bored
				outputState = RobbingBank;
				robberActionPerformingText = "\"You know what would be funny..? If we go \'borrow\' something from the bank…hehe\"";
			}
			else {
				outputState = currentState;
				robberActionPerformingText = "\"Let\'s try again\"";
			}
			break;
		default:
			outputState = StartGameRobber;
			robberActionPerformingText = "\"Let\'s start over\" + ERROR CHECK DEFAULTSTATE";
	}

	return outputState;
}

CopState copStateManager(CopState currentState, RobberState currentRobberState, int currentDutyTime, int currentDistanceCopToRobber) {
	CopState outputState;

	switch (currentState) {
	case StartGameCop:
		if (currentDutyTime < 1) {
			outputState = OnStakeOut;
			copActionPerformingText = "\"Time to go to work!\"";
		}
		else {
			outputState = currentState;
			copActionPerformingText = "\"I need a donut..\" + ERROR CHECK STARTGAMECOP";
		}
		break;
	case OnStakeOut:
		if (currentDutyTime > 9) {
			outputState = OffDuty;
			copActionPerformingText = "\"Duty time\'s over, I\'m headed home\"";
		}
		else if (currentDistanceCopToRobber < 5) {
			outputState = Chasing;
			copActionPerformingText = "\"There he is! The thief, I found him!\"";
		}
		else if(currentDistanceCopToRobber >= 5) {
			outputState = currentState;
			copActionPerformingText = "\"Let\'s keep working\"";
		}
		else {
			outputState = currentState;
			copActionPerformingText = "\"I need a donut..\" + ERROR CHECK ONSTAKEOUT";
		}
		break;
	case Chasing:
		if (currentDutyTime > 9 && currentRobberState != BeingInJail) {
			outputState = OffDuty;
			copActionPerformingText = "\"Duty time\'s over, I\'m headed home\"";
		}
		else if (currentDutyTime > 9 && currentRobberState == BeingInJail) {
			outputState = OffDuty;
			copActionPerformingText = "\"Justice served, he\'s behind bars\"";
		}
		else if (currentDutyTime < 10) {
			outputState = currentState;
			copActionPerformingText = "\"Let\'s keep chasing him\"";
		}
		else {
			outputState = currentState;
			copActionPerformingText = "\"I need a donut..\" + ERRO CHECK CHASING";
		}
		break;
	case OffDuty:
		if (currentDutyTime < 1) {
			outputState = OnStakeOut;
			copActionPerformingText = "\"Time to go to work!\"";
		}
		else {
			outputState = currentState;
			copActionPerformingText = "\"Let\'s sleep some more\"";
			//copActionPerformingText = "\"I need a donut..\" + ERRO CHECK OFFDUTY";
		}
		break;
	default:
		outputState = StartGameCop;
		copActionPerformingText = "\"Let\'s start over and find the thief\" + ERROR CHECK DEFAULTSTATECOP";
	}

	return outputState;
}

string stateRobberMessage() {
	string outputStateMessage;

	switch (activeRobberState) {
	case RobbingBank:
		outputStateMessage = "robbing a bank: \"'I\'m robbing banks and getting loads of money! Pew pew!\"";
		break;
	case HavingGoodTime:
		outputStateMessage = "having a good time: \"I\'m having a good time spending my money\"";
		break;
	case LayingLow:
		outputStateMessage = "laying low: \"I\'ll just wait a minute..\"";
		break;
	case Fleeing:
		outputStateMessage = "fleeing: \"Who said being a thief isn\'t a sport? I\'m running all day!\"";
		break;
	case BeingInJail:
		outputStateMessage = "being in jail: \"So when can I go home? Jail is stupid.\"";
		break;
	case BeingTipsy:
		outputStateMessage = "being tipsy: \"Glug..glug.... hmmmm..... beeeer\"";
		break;
	default:
		outputStateMessage = "being confused: \"I am so confused\" + ERROR CHECK DEFAULTMESSAGEROBBER";
	}

	return outputStateMessage;
}

string stateCopMessage() {
	string outputStateMessage;

	switch (activeCopState) {
		case OffDuty:
			outputStateMessage = "being off duty: \"'Relaxing on the sofa, watching Crime Scene Investigation\"";
			break;
		case OnStakeOut:
			outputStateMessage = "on stake out: \"On the hunt for crime\"";
			break;
		case Chasing:
			outputStateMessage = "chasing: \"Hold it right there buddy\"";
			break;
		default:
			outputStateMessage = "being confused: \"I need a donut!!!\" + ERROR CHECK DEFAULTMESSAGECOP";
	}

	return outputStateMessage;
}
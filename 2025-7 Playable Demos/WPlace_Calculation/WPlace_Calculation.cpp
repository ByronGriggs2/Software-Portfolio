#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <cmath>
#include <tuple>
#include <cassert>
#include <limits>

using std::cin, std::cout, std::endl, std::getline, std::string, std::istringstream, std::tuple;

constexpr int UPGRADE_POWER = 5;
constexpr int UPGRADE_COST = 500;
constexpr int RECHARGE_TIME = 30;

enum simulationMode {
    timeIsGoal,
    freePixelsIsGoal,
    constrainedPixelsIsGoal
};

void rechargeTime();
void pictureSessions();
int getMaxPixelsFast(int startVal, int pixels) { return startVal + UPGRADE_POWER * static_cast<int>(std::floor(static_cast<double>(pixels) / static_cast<double>(UPGRADE_COST))); }
tuple<int, int, int> simulateSessions(int startVal, int targetPixels, int targetTime, simulationMode);

int timeToSeconds(const string&);
string secondsToTime(int);
string getValidInput(const std::function<bool(string)>&);
bool validInput_main(const string&);
bool validInput_time(const string&);
bool validInput_pixelCount(const string&);
bool isInt(const string&);

int promptDesiredTime();
int promptCurrentMax();
int promptPictureSize();
int promptExpectedTime();

int main()
{
    cout << "This program will help you decide how much of your time to dedicate to WPlace, and how big the projects you tackle should be." << endl;
    cout << "A 'session' as used here is defined as logging in after pixels have fully recharged, spending all pixels, and logging out. " << endl;
    cout << "Please select one of the below options." << endl;
    cout << "Option (1): How many sessions will it take to increase my recharge time to X hours?" << endl;
    cout << "Option (2): How many sessions will it take to finish my picture?" << endl;
    cout << "Quit (Q)" << endl;

    const string input = getValidInput(validInput_main);
    switch (input[0]) {
    case 'Q':
    case 'q':
        return 0;
        break;
    case '1':
        rechargeTime();
        break;
    case '2':
        pictureSessions();
        break;
    default:
        return 0;
        break;
    }
    cout << "Press enter to exit" << endl;
    string temp;
    getline(cin, temp);
}

void rechargeTime() {
    const int seconds = promptDesiredTime();
    const int currentMax = promptCurrentMax();

    const tuple<int, int, int> result = simulateSessions(currentMax, 0, seconds, timeIsGoal);
    const int sessions = std::get<2>(result);
    const int pixels = std::get<0>(result);
    const int newMax = getMaxPixelsFast(currentMax, pixels);
    cout << "You will need to play " << sessions << " sessions to reach a recharge time of " << secondsToTime(seconds) << "." << endl;
    cout << "While painting, you will place a total of " << pixels << " pixels and reach a new maximum pixel count of " << newMax << "!" << endl;
}

void pictureSessions() {
    const int picturePixels = promptPictureSize();
    const int currentMax = promptCurrentMax();
    const int maxTime = promptExpectedTime();
    const int maximumPixelsPerSession = static_cast<int>(std::floor(static_cast<double>(maxTime) / static_cast<double>(RECHARGE_TIME)));
    simulationMode mode = maxTime == 0 ? freePixelsIsGoal : constrainedPixelsIsGoal;

    const tuple<int, int, int> result = simulateSessions(currentMax, picturePixels, maxTime, mode);
    const int sessions = std::get<2>(result);
    const int recharge = std::get<1>(result);
    const int newMax = getMaxPixelsFast(currentMax, picturePixels);
    cout << "Completing your next picture will take " << sessions << " sessions." << endl;
    cout << "After completion, your new maximum pixels will be " << newMax << " and will take " << secondsToTime(recharge) << " to recharge!" << endl;
}

tuple<int, int, int> simulateSessions(int startVal, int targetPixels, int targetTime, simulationMode mode) {
    const int maximumPixelsPerSession = mode == constrainedPixelsIsGoal ? static_cast<int>(std::floor(static_cast<double>(targetTime) / static_cast<double>(RECHARGE_TIME))) : std::numeric_limits<int>::max();

    int sessions = 1;
    int cumulativePixels = 0;
    bool maxPixelsReached = false;
    while (true) {
        const int sessionPixels = getMaxPixelsFast(startVal, cumulativePixels);
        const int recharge = RECHARGE_TIME * sessionPixels;
        cumulativePixels += std::min(sessionPixels, maximumPixelsPerSession);

        if ((mode == constrainedPixelsIsGoal || mode == freePixelsIsGoal) && cumulativePixels >= targetPixels)
            return tuple<int, int, int>(cumulativePixels, recharge, sessions);
        if (mode == timeIsGoal && recharge >= targetTime)
            return tuple<int, int, int>(cumulativePixels, recharge, sessions);
        sessions++;
    }
}

string secondsToTime(int seconds) {
    int days = 0;
    int hours = 0;
    int minutes = 0;
    while (seconds >= 3600 * 24) {
        days++;
        seconds -= 3600 * 24;
    }
    while (seconds >= 3600) {
        hours++;
        seconds -= 3600;
    }
    while (seconds >= 60) {
        minutes++;
        seconds -= 60;
    }
    return std::to_string(days) + "d:" + std::to_string(hours) + "h:" + std::to_string(minutes) + "m:" + std::to_string(seconds) + "s";
}
int timeToSeconds(const string& input) {
    istringstream iss(input);
    int days, hours, minutes;
    char unused;
    iss >> days;
    iss >> unused;
    iss >> hours;
    iss >> unused;
    iss >> minutes;
    return 60 * (minutes + 60 * (hours + 24 * days));
}
string getValidInput(const std::function<bool(string)>& validCheck) {
    while (true) {
        string result;
        getline(cin, result);
        if (validCheck(result))
            return result;
        cout << "Invalid input. Please try again." << endl;
    }
}
bool validInput_main(const string& in) {
    switch (in[0]) {
    case 'Q':
    case 'q':
    case '1':
    case '2':
        return true;
        break;
    default:
        return false;
        break;
    }
}
bool validInput_time(const string& in) {
    constexpr int exampleLength = string("DD:HH:MM").length();
    if (in.length() != exampleLength)
        return false;
    int val;
    char delim;
    istringstream iss(in);
    return
        iss >> val &&
        iss >> delim &&
        iss >> val &&
        iss >> delim &&
        iss >> val;
}
bool validInput_pixelCount(const string& in) {
    istringstream iss(in);
    int val = -1;
    char delim = '/n';
    if (!iss >> val)
        return false;
    if (!iss >> delim)
        return true;
    if (!iss >> val)
        return false;
    return true;
}
bool isInt(const string& in) {
    auto isNum = [](char c) {
        return c >= '0' && c <= '9';
        };
    if (!isNum(in[0]) && in[0] != '+' && in[0] != '-')
        return false;
    for (int i = 1; i < in.length(); i++) {
        if (!isNum(in[0]))
            return false;
    }
    return true;
}

int promptDesiredTime() {
    cout << "How long would you like to wait between sessions? (DD:HH:MM)" << endl;
    string input = getValidInput(validInput_time);
    return timeToSeconds(input);
}
int promptCurrentMax() {
    cout << "What is your current maximum pixels?" << endl;
    string input = getValidInput(isInt);
    istringstream iss(input);
    int currentMax;
    iss >> currentMax;
    return currentMax;
}
int promptPictureSize() {
    cout << "What is the size of your picture in pixels? (integer) Alternatively, input rectangular dimensions (integerXinteger)." << endl;
    string input = getValidInput(validInput_pixelCount);
    istringstream iss(input);
    int in1;
    int in2;
    char delim = '\n';
    iss >> in1;
    iss >> delim;
    iss >> in2;
    if (delim == '\n')
        return in1;
    return in1 * in2;
}
int promptExpectedTime() {
    cout << "What is the typical time you have between sessions? (DD:HH:MM). Enter 00:00:00 if you don't care." << endl;
    string input = getValidInput(validInput_time);
    int maxTime = timeToSeconds(input);
    return maxTime;
}
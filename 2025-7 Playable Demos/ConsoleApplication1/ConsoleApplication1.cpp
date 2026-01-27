// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    const long double factor = pow(2, 0.25);
    const int targetTime = 1000;
    const int delta = 1;

    long double currentGrowth = 1;
    int currentTime = 0;
    long double value = 0;

    int currentLog = 2;
    while (currentTime < targetTime) {
        currentTime += delta;
        value += currentGrowth;
        currentGrowth *= factor;
        if (log(value) > currentLog) {
            currentGrowth *= 0.5;
            currentLog++;
        }
        std::cout << "(" << currentTime << ", " << value << ")" << std::endl;
    }
    long double base = pow(value, 1.0 / (long double)targetTime);

    std::cout << base;
}
#include "pch.h"
#include "definitions.h"
#include "Control.h"

void parseCommandLineArguments(int argc, char* argv[], Control& control);

using namespace helpers;
int main(int argc, char* argv[])
{
#ifdef _OPENMP
    std::cout << "OpenMP enabled" << std::endl;
#endif
    srand(static_cast<unsigned int>(time(0)));
    Control control;
	parseCommandLineArguments(argc, argv, control);

    return 0;
}

void parseCommandLineArguments(int argc, char* argv[], Control& control) {
    if (argc < 2) {
        throw std::runtime_error("Invalid command line argument");
        return;
    }
    else if (std::strcmp(argv[1], "startTraining") == 0) {
		control.startTraining();
    }
    else if (std::strcmp(argv[1], "resumeTraining") == 0) {
        if (argc >= 3) {
            if (argc == 3 && std::strcmp(argv[2], "error") == 0)
                control.resumeTraining(true, false);
            else if (argc == 3 && std::strcmp(argv[2], "resetEpisodeCounter") == 0)
                control.resumeTraining(false, true);
            else if (argc == 4 && std::strcmp(argv[2], "error") == 0 && std::strcmp(argv[2], "resetEpisodeCounter") == 0)
				control.resumeTraining(true, true);
            else
				throw std::runtime_error("Invalid resumeTraining argument");
        }
        else {
            control.resumeTraining();
		}
    }
    else if (std::strcmp(argv[1], "load") == 0) {
        if (argc < 3)
            throw std::runtime_error("Load command requires a filename argument");
        control.loadAndEvaluateNetwork(argv[2]);
    }
    else if (std::strcmp(argv[1], "networkTest") == 0) {
        control.neuralNetworkTest();
    }
    else if (std::strcmp(argv[1], "cheatTest") == 0) {
        control.cheatCodeTest();
    }
    else if (std::strcmp(argv[1], "inOutTest") == 0) {
        control.inOutTest();
    }
    else if (std::strcmp(argv[1], "responsivenessTest") == 0) {
        control.responsivenessTest(true);
    }
    else if (std::strcmp(argv[1], "desyncTest") == 0) {
        control.desyncTest();
    }
    else if (std::strcmp(argv[1], "trainingStateTest") == 0) {
        control.trainingStateTest();
    }
    else if (std::strcmp(argv[1], "maximumRewardTest") == 0) {
        control.maximumRewardTest();
    }
    else
        throw std::runtime_error("Invalid command line argument");
}
#include "Definitions.h"
#include "i_love_vampires_2.h"

void LOGERROR(const char* inputString) {
	UE_LOG(LogMyGame, Warning, TEXT("% s"), ANSI_TO_TCHAR(inputString));
	checkNoEntry();
}
void LOGWARNING(const char* inputString) {
	UE_LOG(LogMyGame, Warning, TEXT("% s"), ANSI_TO_TCHAR(inputString));
}
void LOGINFO(const std::string& inputString) {
	UE_LOG(LogMyGame, Log, TEXT("% s"), ANSI_TO_TCHAR(inputString.c_str()));
}
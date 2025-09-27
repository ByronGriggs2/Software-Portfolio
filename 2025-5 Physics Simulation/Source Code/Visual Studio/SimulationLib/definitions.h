#pragma once
#define DISABLE_NETWORKING
#define NOT_DISTRIBUTED
//#define VERBOSE

constexpr float EPSILON = 1e-5f;
constexpr float GRAVITY = -9.81f;
constexpr float SLEEP_RESOLUTION = 18.0f;
typedef unsigned short WORD;
typedef unsigned long long ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;
constexpr int LANBROADSEND = 6000;
constexpr int LANBROADRECEIVE = 6001;
constexpr int LANLISTENING = 6002;
constexpr const char* HOMEIP = "10.228.223.255";
constexpr const char* LABIP = "10.140.9.255";

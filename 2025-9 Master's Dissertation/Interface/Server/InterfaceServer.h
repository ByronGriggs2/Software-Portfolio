#pragma once
#include <windows.h>
#include <thread>

namespace helpers {
	enum playerAction;
}

class InterfaceServer
{
#pragma pack(push,1)
	struct InputsPacket {
		float _x = 0.0f;
		float _y = 0.0f;
		BYTE _primary = 0;
		BYTE _secondary = 0;
		BYTE _cancel = 0;
	};
#pragma pack(pop)
	
	HANDLE _pipe;

	void testInputs();
	void sendAction(helpers::playerAction);
	void sendPacket(const InputsPacket&);
public:
	int initialize();
	void messageLoop();
};


#pragma once
class UGameInstance;

class GIRefSingleton {
	UGameInstance* _GI = nullptr;

	GIRefSingleton() = default;
	GIRefSingleton(const GIRefSingleton& other) = delete;
	GIRefSingleton(GIRefSingleton&& other) = delete;
	GIRefSingleton& operator=(const GIRefSingleton& other) = delete;
	GIRefSingleton& operator=(GIRefSingleton&& other) = delete;
	
public:
	static UGameInstance* getGameInstance();
	static GIRefSingleton& getInstance(UGameInstance* GI = nullptr);
};
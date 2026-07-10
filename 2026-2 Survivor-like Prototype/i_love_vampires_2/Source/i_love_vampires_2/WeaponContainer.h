#pragma once
#include <vector>
#include <memory>
#include "ActiveSocket.h"
#include "ActiveEnum.h"
#include "MyPlayer.h"
struct FSocketData;

class WeaponContainer {
	friend class UWeaponContainer;
	std::vector<ActiveSocket> _sockets;
	bool slotValid(int index) const;

public:
	WeaponContainer() = delete;
	WeaponContainer(int socketCount);
	virtual ~WeaponContainer() = default;
	// Returns 0 if successful, returns -1 for error
	[[nodiscard]] int addWeapon(std::shared_ptr<Active> active, int index);
	[[nodiscard]] int replaceWeapon(std::shared_ptr<Active> active, int index);
	int getIndexOfActive(std::shared_ptr<Active> active) const;
	virtual EWeaponType getType() const = 0;
	const std::vector<ActiveSocket>& getSockets() const { return _sockets; }
	bool hasEmptySocket() const;
	int getFirstEmptySocket() const;
	bool getSocketData(TArray<FSocketData>& out) const;
	int size() const { return _sockets.size(); }
};

class Sword : public WeaponContainer {
public:
	Sword() = delete;
	virtual ~Sword() = default;
	Sword(int socketCount) : WeaponContainer(socketCount) {}
	virtual EWeaponType getType() const override { return EWeaponType::sword; }
};
class Gun : public WeaponContainer {
public:
	Gun() = delete;
	virtual ~Gun() = default;
	Gun(int socketCount) : WeaponContainer(socketCount) {}
	virtual EWeaponType getType() const override { return EWeaponType::gun; }
};
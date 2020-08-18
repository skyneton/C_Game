#pragma once
#include "GameObject.h"
#include "BulletManager.h"
#include "RadiusCollider.h"


class Player : public GameObject
{
	
public:
	Player(const wchar_t* imagePath, float hp);
	~Player();

	BulletManager* bm;

	float moveSpeed;

	virtual void Update();
	void Move();
	void Shoot();
	void Healing();

	float hp;

	RadiusCollider* col;
	bool Hit(float damage);
	void CheckOutOfScreen();
	void StageChange(int stage);
	static int GetStage();
	static int GetHealingChance();
	void SetHealingChance(int n);
	void SetMaxHP(float hp);
private:
	float moveLock;
	DWORD shootDelay;
	DWORD doubleClickUPDelay;
	DWORD doubleClickLeftDelay;
	DWORD doubleClickDownDelay;
	DWORD doubleClickRightDelay;
	static int stage;
	static int healingChance;
	float MaxHP;
};


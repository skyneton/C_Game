#include "stdafx.h"
#include "Player.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "Bullet.h"
#include "Scene.h"

#define TELEPORT_AMOUNT 100

int Player::stage;

Player::Player(const wchar_t* imagePath, float hp)
:GameObject(imagePath), hp(hp)
{
	col = new RadiusCollider(transform, renderer->GetWidth() * 0.5f);
	moveSpeed = 180.f;
	moveLock = 5;
	bm = new BulletManager(this);
	Scene::GetCurrentScene()->PushBackGameObject(bm);

	StageChange(1);
}


Player::~Player()
{
}

void Player::StageChange(int i) {
	stage = i;

	printf("Stage Changed: %d\n", stage);

	for (int i = 0; i < stage; i++) {
		Enemy* enemy = bm->PushBackEnemy(new Enemy(L"resources/enemy.png", stage*50.f, this));
		Scene::GetCurrentScene()->PushBackGameObject(enemy);
		enemy->transform->SetPosition(400.0f, 100.0f);
		enemy->transform->SetScale(0.25f, 0.25f);
	}
}

int Player::GetStage() {
	return stage;
}

void Player::Update() {
	//매 프레임 호출
	Move();
	Shoot();
}

bool Player::Hit(float damage) {
	hp -= damage;
	printf("PlayerDamaged: %.2lf -> %.2lf\n", hp + damage, hp);
	return hp <= 0;
}

void Player::Move() {
	//움직임을 담당
	float y = transform->position.y;
	float x = transform->position.x;
	if (InputManager::GetKeyState('W') || InputManager::GetKeyState(VK_UP)) {
		transform->position.y -= moveSpeed * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('W') || InputManager::GetKeyDown(VK_UP)) {
			DWORD now = timeGetTime();
			if (now - doubleClickUPDelay < 200) {
				printf("UP DOUBLE CLICK\n");
				transform->position.y -= TELEPORT_AMOUNT;
			}
			doubleClickUPDelay = now;
		}
	}

	if (InputManager::GetKeyState('S') || InputManager::GetKeyState(VK_DOWN)) {
		transform->position.y += moveSpeed * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('S') || InputManager::GetKeyDown(VK_DOWN)) {
			DWORD now = timeGetTime();
			if (now - doubleClickDownDelay < 200) {
				printf("DOWN DOUBLE CLICK\n");
				transform->position.y += TELEPORT_AMOUNT;
			}
			doubleClickDownDelay = now;
		}
	}

	if (InputManager::GetKeyState('A') || InputManager::GetKeyState(VK_LEFT)) {
		transform->position.x -= moveSpeed * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('A') || InputManager::GetKeyDown(VK_LEFT)) {
			DWORD now = timeGetTime();
			if (now - doubleClickLeftDelay < 200) {
				printf("LEFT DOUBLE CLICK\n");
				transform->position.x -= TELEPORT_AMOUNT;
			}
			doubleClickLeftDelay = now;
		}
	}

	if (InputManager::GetKeyState('D') || InputManager::GetKeyState(VK_RIGHT)) {
		transform->position.x += moveSpeed * TimerManager::GetDeltaTime();

		if (InputManager::GetKeyDown('D') || InputManager::GetKeyDown(VK_RIGHT)) {
			DWORD now = timeGetTime();
			if (now - doubleClickRightDelay < 200) {
				printf("RIGHT DOUBLE CLICK\n");
				transform->position.x += TELEPORT_AMOUNT;
			}
			doubleClickRightDelay = now;
		}
	}

	CheckOutOfScreen();

	if (y == transform->position.y && x == transform->position.x) {
		if (moveLock > 0) {
			transform->position.y += moveLock * 7 * TimerManager::GetDeltaTime();
			if (moveLock < 1)
				moveLock -= 0.05f;
			else
				moveLock -= 0.1f;
			if (moveLock < 0) moveLock = -5;
		}
		else {
			transform->position.y += moveLock * 7 * TimerManager::GetDeltaTime();
			if (moveLock > -1)
				moveLock += 0.05f;
			else
				moveLock += 0.1f;
			if (moveLock > 0) moveLock = 5;
		}
	}
	else moveLock = 5;
}

void Player::CheckOutOfScreen()
{
	//총알의 포지션과, 총알의 이미지 크기를 고려해서
	//총알이 밖에 나갔는 지 판정해주어야 함.
	int sw = WinApp::GetScreenWidth();					//윈도우의 가로 길이
	int sh = WinApp::GetScreenHeight();					//윈도우의 세로 길이

	if (transform->position.x <= 0)	//플레이어가 왼쪽 끝을 나갔거나
		transform->position.x = 0;

	if (transform->position.x >= sw)		//플레이어가 오른 끝을 나갔거나
		transform->position.x = sw;

	if (transform->position.y <= 0)	//플레이어가 위쪽 끝을 나갔거나
		transform->position.y = 0;

	if (transform->position.y >= sh)		//플레이어가 아래 끝을 나갔거나
		transform->position.y = sh;
}

void Player::Shoot() {
	//총알 발사
	if (InputManager::GetKeyState(VK_SPACE)) {
		DWORD delayCheck = timeGetTime();
		if (delayCheck - shootDelay >= 400 || InputManager::GetKeyDown(VK_SPACE)) { //0.4초마다 총알 발사
			shootDelay = delayCheck;
			Bullet* b;
			srand(timeGetTime());
			if (rand() % 101 <= 70) {
				b = new Bullet(L"resources/arrow2.png");
				b->damage = 5.0f * stage;
				b->speed = 1500.0f;
			}
			else {
				b = new Bullet(L"resources/arrow1.png");
				b->damage = 6.5f * stage;
				b->speed = 1000.0f;
			}
			b->transform->SetScale(0.1f, 0.1f);
			Scene::GetCurrentScene()->PushBackGameObject(b);
			bm->PushBackPlayerBullet(b);

			b->transform->position = this->transform->position;
			// b->angleRate = 0.2f;
			b->angle = 0.75f;
			b->transform->SetRotation(90.0f + b->angle);
		}
	}
}

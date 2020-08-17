#include "stdafx.h"
#include "Enemy.h"
#include "TimerManager.h"
#include "Player.h"
#include "Scene.h"
#include "BulletManager.h"

#define PI 3.141592f

Enemy::Enemy(const wchar_t* imagePath, float hp, GameObject* pl) :GameObject(imagePath)
{
	player = (Player*) pl;
	col = new RadiusCollider(transform, renderer->GetWidth() * 0.5f);
	this->hp = hp;
	verti = rand() % 101;
	hori = rand() % 101;
	if (rand() % 101 < 50)
		verti *= -1;

	if (rand() % 101 < 50)
		hori *= -1;
}


Enemy::~Enemy()
{
	SAFE_DELETE(col);
}

void Enemy::Update() {
	Move();
	Shoot();
}

void Enemy::Move() {
	if (verti < 0) {
		transform->position.y -= (rand() % 101 + 100) * TimerManager::GetDeltaTime();
		verti++;
		if (verti >= 0) {
			verti = rand() % 101;
			if (rand() % 101 < 50)
				verti *= -1;
		}
	}
	else {
		transform->position.y += (rand() % 101 + 100) * TimerManager::GetDeltaTime();
		verti--;
		if (verti <= 0) {
			verti = rand() % 101;
			if (rand() % 101 < 50)
				verti *= -1;
		}
	}

	if (hori < 0) {
		transform->position.x -= (rand() % 101 + 100) * TimerManager::GetDeltaTime();
		hori++;
		if (hori >= 0) {
			hori = rand() % 101;
			if (rand() % 101 < 50)
				hori *= -1;
		}
	}
	else {
		transform->position.x += (rand() % 101 + 100) * TimerManager::GetDeltaTime();
		hori--;
		if (hori <= 0) {
			hori = rand() % 101;
			if (rand() % 101 < 50)
				hori *= -1;
		}
	}
	CheckOutOfScreen();
}

void Enemy::Shoot() {
	DWORD now = timeGetTime();
	if (now - shootDelay > 800 * Player::GetStage()) {
		shootDelay = now;
		switch (Player::GetStage()) {
			case 1: {
				Bullet* b;
				srand(timeGetTime());
				if (rand() % 101 <= 70) {
					b = new Bullet(L"resources/arrow4.png");
					b->damage = 5.0f * (Player::GetStage() * 0.8f);
					b->speed = 900.0f;
				}
				else {
					b = new Bullet(L"resources/arrow3.png");
					b->damage = 6.5f * (Player::GetStage() * 0.8f);
					b->speed = 600.0f;
				}
				b->transform->SetScale(0.1f, 0.1f);
				Scene::GetCurrentScene()->PushBackGameObject(b);
				((Player*) player)->bm->PushBackEnemyBullet(b);

				b->transform->position = this->transform->position;
				// b->angleRate = 0.2f;
				b->angle = 0.25f;
				b->transform->SetRotation(90.0f + b->angle);
				break;
			}
			case 2: {
				float myX = transform->position.x;
				float myY = transform->position.y;

				float otherX = player->transform->position.x;
				float otherY = player->transform->position.y;

				Bullet* b;
				srand(timeGetTime());
				if (rand() % 101 <= 70) {
					b = new Bullet(L"resources/arrow4.png");
					b->damage = 5.0f * (Player::GetStage() * 0.8f);
					b->speed = 600.0f;
				}
				else {
					b = new Bullet(L"resources/arrow3.png");
					b->damage = 6.5f * (Player::GetStage() * 0.8f);
					b->speed = 400.0f;
				}
				b->transform->SetScale(0.1f, 0.1f);
				Scene::GetCurrentScene()->PushBackGameObject(b);
				((Player*)player)->bm->PushBackEnemyBullet(b);

				b->transform->position = this->transform->position;
				// b->angleRate = 0.2f;
				b->angle = atan2f(myY - otherY, myX - otherX) / (2.0f * PI) + 0.5f;
				b->transform->SetRotation(90.0f + b->angle);
				break;
			}
			case 3: {
				for (float i = 0; i < 1; i += 0.2) {

					Bullet* b;
					srand(timeGetTime());
					if (rand() % 101 <= 70) {
						b = new Bullet(L"resources/arrow4.png");
						b->damage = 5.0f * (Player::GetStage() * 0.8f);
						b->speed = 150.0f;
					}
					else {
						b = new Bullet(L"resources/arrow3.png");
						b->damage = 6.5f * (Player::GetStage() * 0.8f);
						b->speed = 100.0f;
					}
					b->transform->SetScale(0.1f, 0.1f);
					Scene::GetCurrentScene()->PushBackGameObject(b);
					((Player*)player)->bm->PushBackEnemyBullet(b);

					b->transform->position = this->transform->position;
					// b->angleRate = 0.2f;
					b->angle = i;
					b->transform->SetRotation(90.0f + b->angle);
				}
				break;
			}
			case 4: {
				for (float i = 0; i < 1; i += 0.2) {

					Bullet* b;
					srand(timeGetTime());
					if (rand() % 101 <= 70) {
						b = new Bullet(L"resources/arrow4.png");
						b->damage = 5.0f * (Player::GetStage() * 0.8f);
						b->speed = 150.0f;
					}
					else {
						b = new Bullet(L"resources/arrow3.png");
						b->damage = 6.5f * (Player::GetStage() * 0.8f);
						b->speed = 100.0f;
					}
					b->transform->SetScale(0.1f, 0.1f);
					Scene::GetCurrentScene()->PushBackGameObject(b);
					((Player*)player)->bm->PushBackEnemyBullet(b);

					b->transform->position = this->transform->position;
					b->angleRate = 0.2f;
					b->angle = i;
					b->transform->SetRotation(90.0f + b->angle);
				}
				break;
			}

			default: {
				for (float i = 0; i < 1; i += 0.15) {

					Bullet* b;
					srand(timeGetTime());
					if (rand() % 101 <= 70) {
						b = new Bullet(L"resources/arrow4.png");
						b->damage = 5.0f * (Player::GetStage() * 0.8f);
						b->speed = 100.0f;
					}
					else {
						b = new Bullet(L"resources/arrow3.png");
						b->damage = 6.5f * (Player::GetStage() * 0.8f);
						b->speed = 50.0f;
					}
					b->transform->SetScale(0.1f, 0.1f);
					Scene::GetCurrentScene()->PushBackGameObject(b);
					((Player*)player)->bm->PushBackEnemyBullet(b);

					b->transform->position = this->transform->position;
					// b->angleRate = 0.2f;
					b->angle = i;
					b->transform->SetRotation(90.0f + b->angle);
				}
				break;
			}
		}
	}
}

bool Enemy::Hit(float damage) {
	hp -= damage;

	return hp <= 0;
}

void Enemy::CheckOutOfScreen()
{
	//총알의 포지션과, 총알의 이미지 크기를 고려해서
	//총알이 밖에 나갔는 지 판정해주어야 함.
	float sw = WinApp::GetScreenWidth();					//윈도우의 가로 길이
	float sh = WinApp::GetScreenHeight();					//윈도우의 세로 길이
	float iw = renderer->GetWidth() * transform->scale.x * 0.5f;
	float ih = renderer->GetHeight() * transform->scale.y * 0.5f;

	if (transform->position.x <= iw)	//플레이어가 왼쪽 끝을 나갔거나
		transform->position.x = iw;

	if (transform->position.x >= sw - iw)		//플레이어가 오른 끝을 나갔거나
		transform->position.x = sw - iw;

	if (transform->position.y <= ih)	//플레이어가 위쪽 끝을 나갔거나
		transform->position.y = ih;

	if (transform->position.y >= sh - ih - 100)		//플레이어가 아래 끝을 나갔거나
		transform->position.y = sh - ih - 100;
}

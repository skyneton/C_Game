#include "stdafx.h"
#include "BulletManager.h"
#include "Scene.h"
#include "Player.h"
#include "GameScene.h"

Player* player;

BulletManager::BulletManager(GameObject* pl)
{
	player = (Player*) pl;
}


BulletManager::~BulletManager()
{
}

Bullet* BulletManager::PushBackEnemyBullet(Bullet* b) {
	enemyBulletList.push_back(b);
	return b;
}

Bullet* BulletManager::PushBackPlayerBullet(Bullet* b) {
	playerBulletList.push_back(b);
	return b;
}

Enemy* BulletManager::PushBackEnemy(Enemy* b) {
	GameScene::enemy.push_back(b);
	return b;
}

void BulletManager::DestroyPlayerBullet(Bullet* b) {
	destroyedPlayerBullet.push_back(b);
}

void BulletManager::DestroyEnemyBullet(Bullet* b) {
	destroyedEnemyBullet.push_back(b);
}

void BulletManager::DestroyEnemy(Enemy* e) {
	destroyedEnemy.push_back(e);
}

void BulletManager::RemoveDestroyed() {
	if (destroyedPlayer) {
		destroyedPlayerBullet = playerBulletList;
		destroyedEnemyBullet = enemyBulletList;
		destroyedEnemy = GameScene::enemy;
	}

	for (auto& i : destroyedPlayerBullet) {
		playerBulletList.remove(i);
		Scene::GetCurrentScene()->Destroy(i);
	}
	for (auto& i : destroyedEnemyBullet) {
		enemyBulletList.remove(i);
		Scene::GetCurrentScene()->Destroy(i);
	}
	for (auto& i : destroyedEnemy) {
		GameScene::enemy.remove(i);
		Scene::GetCurrentScene()->Destroy(i);
	}

	if (GameScene::enemy.size() == 0) {
		if (destroyedPlayer) {
			player->StageChange(1);
			player->transform->SetPosition(400.f, 300.f);
			player->hp = 200.f;
			destroyedPlayer = false;
		}
		else player->StageChange(player->GetStage() + 1);
	}

	destroyedPlayerBullet.clear();
	destroyedEnemyBullet.clear();
	destroyedEnemy.clear();
}

void BulletManager::LateUpdate() {
	CheckCollision();
	RemoveDestroyed();
}

void BulletManager::CheckCollision() {
	for (auto& i : playerBulletList) {
		if (i->CheckOutOfScreen()) {
			DestroyPlayerBullet(i);
		}
		else {
			for (auto& enemy : GameScene::enemy) {
				if (i->col->Intersected(enemy->col)) {
					if (enemy->Hit(i->damage)) {
						DestroyEnemy(enemy);
					}
					DestroyPlayerBullet(i);

					break;
				}
			}
		}
	}

	for (auto& i : enemyBulletList) {
		if (i->CheckOutOfScreen())
			DestroyEnemyBullet(i);
		else if (i->col->Intersected(player->col)) {
			if (player->Hit(i->damage)) {
				destroyedPlayer = true;
				break;
			}
			DestroyEnemyBullet(i);
		}
	}
}

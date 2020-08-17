#include "stdafx.h"
#include "GameScene.h"

std::list<Enemy*> GameScene::enemy;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	background = PushBackGameObject(new GameObject(L"resources/background.png"));
	background->transform->SetPosition(400.0f, 300.0f);
	background->transform->SetScale(0.9f, 1.15f);

	player = (Player*)PushBackGameObject(new Player(L"resources/player.png", 200.f));
	player->transform->SetPosition(400.f, 300.f);
	player->transform->SetScale(0.3f, 0.3f);
}

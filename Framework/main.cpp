#include "stdafx.h"
#include "Framework.h"
#include "GameScene.h"

int main()
{
	Framework* f = new Framework;
	f->Run(new GameScene(),L"Ÿ��Ʋ", 800, 600, false);
	SAFE_DELETE(f);

	return 0;
}
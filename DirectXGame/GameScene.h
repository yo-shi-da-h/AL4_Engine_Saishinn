#pragma once
#include "KamataEngine.h" 
#include "Player.h"
#include "Enemy.h"
#include "AABB.h"
#include "Skydome.h"
#include "RailCamera.h"
using namespace KamataEngine;

class GameScene {
public: 

	GameScene();
	~GameScene();

	void Initialize();
	void Update();
	void Draw();
	// 衝突判定と応答
	void CheckAllCollisions();

	// 弾を追加
	//void AddEnemyBullet(EnemyBullet* enemyBullet);

private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	RailCamera* railCamera_ = nullptr;

	//Vector3 playerPos = {};
	Vector3 enemyPos = {0, 3, 100};
	Vector3 RailCamerPos = {0, 0, 0};
	Vector3 RailCamerRad = {0, 0, 0};

	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;

	//std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform worldTransform_;
	Camera camera_;

	Vector3 railcameraPos = {0,0,0};
	Vector3 railcameraRad = {0,0,0};


};
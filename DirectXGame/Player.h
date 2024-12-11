
#pragma once
#include "PlayerBullet.h"
#include <list>
#include <3d/Model.h>
#include <3d/Camera.h>
#include "AABB.h"
#include "MT.h"
#include <KamataEngine.h>

namespace KamataEngine { class Input; };

class Enemy;

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);
	void Update();
	void Draw();
	~Player();
	void Attack();
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	KamataEngine::Vector3 GetWorldPosition(); 

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// AABBを取得
	AABB GetAABB();

	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

	void SetParent(const WorldTransform* parent);

private:
	
	KamataEngine::WorldTransform worldtransfrom_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Input* input_ = nullptr;
	
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* modelbullet_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	bool isDead_ = false;
	bool isParry_ = false;

	Enemy* enemy_ = nullptr;

	// 発射タイマー
	int32_t spawnTimer = 0;

};

#pragma once
#include <3d/Model.h>
#include <3d/WorldTransform.h>
#include <3d/Camera.h>
#include "EnemyBullet.h"
#include "MT.h"

class Player;

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {
public:

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& pos);
	void Update();
	void Draw();
	~Enemy();
	void Fire();

	KamataEngine::Vector3 GetWorldPosition();

	void SetPlayer(Player* player) { player_ = player; }
    // 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 発射間隔
	static const int kFireInterval = 30;

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:

	KamataEngine::WorldTransform worldtransfrom_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* modelbullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t spawnTimer = 0;

	Player* player_ = nullptr;

	Phase phase_ = Phase::Approach;

	Phase Bulletphase_ = Phase::Approach;

};
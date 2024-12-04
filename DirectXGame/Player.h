#pragma once
#include "PlayerBullet.h"
#include <list>
#include <3d/Model.h>
#include <3d/Camera.h>
#include <KamataEngine.h>

namespace KamataEngine { class Input; };


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

private:
	
	KamataEngine::WorldTransform worldtransfrom_;

	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Input* input_ = nullptr;
	
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::Model* modelbullet_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

};
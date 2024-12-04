#include "EnemyBullet.h"
#include <cassert>

EnemyBullet::~EnemyBullet() { model_ = nullptr; }

void EnemyBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	assert(model);
	model_ = model;
	worldtransfrom_.translation_ = position;
	worldtransfrom_.Initialize();
	velocity_ = velocity;
}

void EnemyBullet::Update() {

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldtransfrom_.translation_.x += velocity_.x;
	worldtransfrom_.translation_.y += velocity_.y;
	worldtransfrom_.translation_.z += velocity_.z;

	worldtransfrom_.UpdateMatarix();

}

void EnemyBullet::Draw(const KamataEngine::Camera& camera) {

	// モデルの描画
	model_->Draw(worldtransfrom_, camera);

}
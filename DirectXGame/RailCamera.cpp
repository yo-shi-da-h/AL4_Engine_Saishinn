#include "RailCamera.h"

void RailCamera::Initialize(const KamataEngine::Vector3& pos, const KamataEngine::Vector3& rad) {

	worldtransfrom_.translation_ = pos;
	worldtransfrom_.rotation_ = rad;

	worldtransfrom_.Initialize();
	camera_.Initialize();
}

void RailCamera::Update() {


    //camera_.rotation_.y += 0.11f;
	//camera_.translation_.x+= 0.01f;
	
	worldtransfrom_.translation_.z -= 0.11f;
	worldtransfrom_.rotation_.y -= 0.001f;


	worldtransfrom_.UpdateMatarix();
	camera_.UpdateMatrix();
	camera_.matView = Inverse(worldtransfrom_.matWorld_);
	camera_.TransferMatrix();
}

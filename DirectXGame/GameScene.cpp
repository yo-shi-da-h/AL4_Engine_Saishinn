#include "GameScene.h"
#include <cassert>
#include "3d/AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelEnemy_;
	delete modelSkydome_;
	delete player_;
	delete enemy_;
	delete skydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
	
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	enemy_ = new Enemy();
	skydome_ = new Skydome();
	// 3Dモデルの生成
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクションの初期化
	//camera_.farZ = 1500.0f;

	Vector3 playerPos = {0, 0, 25};
	camera_.Initialize();
	player_->Initialize(modelPlayer_, &camera_, playerPos);
	enemy_->Initialize(modelEnemy_, &camera_, enemyPos);
	skydome_->Initialize(modelSkydome_, &camera_);

	// 軸方向表示の表示を有効にする
	KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	// Camera
	railCamera_ = new RailCamera();
	railCamera_->Initialize(railcameraPos, railcameraRad);
	player_->SetParent(&railCamera_->GetWorldTransform());

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	player_->SetEnemy(enemy_);
}

void GameScene::Update() { 
	
	enemy_->Update();
	CheckAllCollisions();

	railCamera_->Update();
	player_->Update();
	camera_.matView = railCamera_->GetViewProjection().matView;
	camera_.matProjection = railCamera_->GetViewProjection().matProjection;
	camera_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
	dxCommon_->ClearDepthBuffer();
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	
	player_->Draw(); 
	enemy_->Draw();
	skydome_->Draw();

	 
	/// </summary>
	
	KamataEngine::Model::PostDraw();
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}

void GameScene::CheckAllCollisions() {

	KamataEngine::Vector3 posA[3]{}, posB[3]{};
	float radiusA[3] = {0.8f, 2.0f, 0.8f}; // プレイヤーの半径（固定値）
	float radiusB[3] = {0.8f, 2.0f, 0.8f}; // 敵弾の半径（固定値）

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();
	// 自弾
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラの座標
	posA[0] = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB[0] = bullet->GetWorldPosition();

		// 2つの球の中心間の距離の二乗を計算
		float distanceSquared = (posA[0].x - posB[0].x) * (posA[0].x - posB[0].x) + (posA[0].y - posB[0].y) * (posA[0].y - posB[0].y) + (posA[0].z - posB[0].z) * (posA[0].z - posB[0].z);

		// 半径の合計の二乗
		float combinedRadiusSquared = (radiusA[0] + radiusB[0]) * (radiusA[0] + radiusB[0]);

		// 衝突判定 (距離の二乗が半径の合計の二乗以下なら衝突)
		if (distanceSquared <= combinedRadiusSquared) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定	

	// 敵
	posA[1] = enemy_->GetWorldPosition();

	for (PlayerBullet* bullet : playerBullets) {

		posB[1] = bullet->GetWorldPosition();
		float distanceSquared = (posA[1].x - posB[1].x) * (posA[1].x - posB[1].x) + (posA[1].y - posB[1].y) * (posA[1].y - posB[1].y) + (posA[1].z - posB[1].z) * (posA[1].z - posB[1].z);
		float combinedRadiusSquared = (radiusA[2] + radiusB[2]) * (radiusA[2] + radiusB[2]);

		if (distanceSquared <= combinedRadiusSquared) {
			enemy_->OnCollision();
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* bullet : enemyBullets) {
		for (PlayerBullet* bullet2 : playerBullets) {

			posA[2] = bullet->GetWorldPosition();
			posB[2] = bullet2->GetWorldPosition();

			float distanceSquared = (posA[2].x - posB[2].x) * (posA[2].x - posB[2].x) + (posA[2].y - posB[2].y) * (posA[2].y - posB[2].y) + (posA[2].z - posB[2].z) * (posA[2].z - posB[2].z);
			float combinedRadiusSquared = (radiusA[1] + radiusB[1]) * (radiusA[1] + radiusB[1]);

			if (distanceSquared <= combinedRadiusSquared) {
				bullet->OnCollision();
				bullet2->OnCollision();
			}
		}
	}

#pragma endregion

}

/*/
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	enemyBullets


}
/*/
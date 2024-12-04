#include "GameScene.h"
#include <cassert>
#include "3d/AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete modelPlayer_;
	delete modelEnemy_;
	delete player_;
	delete enemy_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	player_ = new Player();
	enemy_ = new Enemy();
	// 3Dモデルの生成
	modelPlayer_ = KamataEngine::Model::CreateFromOBJ("cube", true);
	modelEnemy_ = KamataEngine::Model::CreateFromOBJ("cube", true);

	// ビュープロジェクションの初期化
	camera_.Initialize();

	//playerPos.z = 0; 
	player_->Initialize(modelPlayer_, &camera_, playerPos);
	enemy_->Initialize(modelEnemy_, &camera_, enemyPos);

	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	KamataEngine::AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	KamataEngine::AxisIndicator::GetInstance()->SetTargetCamera(&camera_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

}

void GameScene::Update() { 
	player_->Update();
	enemy_->Update();
	debugCamera_->Update();
    
#ifdef _DEBUG

	    if (input_->TriggerKey(DIK_1)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		camera_.TransferMatrix();

	} else {
	    camera_.UpdateMatrix();
	}

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
	 
	/// </summary>
	
	KamataEngine::Model::PostDraw();
	Sprite::PreDraw(commandList);
	Sprite::PostDraw();
}
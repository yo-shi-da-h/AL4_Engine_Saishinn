#include "MT.h"
#include <KamataEngine.h>
#include "3d/WorldTransform.h"

void KamataEngine::WorldTransform::UpdateMatarix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	// 定数バッファに転送する
	TransferMatrix();
}
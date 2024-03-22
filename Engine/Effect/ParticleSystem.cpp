#include "ParticleSystem.h"
#include"TextureManager.h"

/*=====================================*/
/* 　　　　   パブリックメソッド　　　	　 */
/*=====================================*/

ParticleSystem::~ParticleSystem()
{
	delete emitter_;

}

void ParticleSystem::Initialize(uint32_t textureHandle, Camera* camera, Vector3 velocity, bool isRandomPosition, bool isRandomVelocity)
{

	dxCommon_ = DirectXCommon::GetInstance();
	srvManager_ = SrvManager::GetInstance();
	psoManager_ = GraphicsPipelineManager::GetInstance();
	texture_ = TextureManager::GetInstance();

	textureHandle_ = textureHandle;
	camera_ = camera;
	isRandomPosition_ = isRandomPosition;
	velocity_ = velocity;
	isRandomVelocity_ = isRandomVelocity;

	//emitter_ = emitter;

	uvTransform =
	{
		{1.0f,1.0f,1.0f,},
		{0.0f,0.0f,0.0f,},
		{0.0f,0.0f,0.0f},
	};

	VertexBuffer();
	MaterialBuffer();
	IndexBuffer();
	InstancingBuffer();

	AdjustTextureSize();

	SetSRV();

	left = 0.0f * textureSize_.x;
	right = 1.0f * textureSize_.x;
	top = 0.0f * textureSize_.y;
	bottom = 1.0f * textureSize_.y;

	texLeft = textureLeftTop.x / textureSize_.x;
	texRight = (textureLeftTop.x + textureSize_.x) / textureSize_.x;
	texTop = textureLeftTop.y / textureSize_.y;
	texBottom = (textureLeftTop.y + textureSize_.y) / textureSize_.y;


	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };

	//頂点の設定
	vertexData_[LB].position = { left,bottom,0.0f,1.0f };
	vertexData_[LB].texcoord = { texLeft,texBottom };

	vertexData_[LT].position = { left,top,0.0f,1.0f };
	vertexData_[LT].texcoord = { texLeft,texTop };

	vertexData_[RB].position = { right,bottom,0.0f,1.0f };
	vertexData_[RB].texcoord = { texRight,texBottom };

	vertexData_[RT].position = { right,top,0.0f,1.0f };
	vertexData_[RT].texcoord = { texRight,texTop };


	SetMaterialData(color);



	indexData_[0] = 0;
	indexData_[1] = 1;
	indexData_[2] = 2;
	indexData_[3] = 1;
	indexData_[4] = 3;
	indexData_[5] = 2;

	emitter_->count = 10;
	emitter_->frequency = 1.0f;
	emitter_->frequencyTime = 0.0f;
	emitter_->transform.translate = { 0.0f,0.2f,0.0f };
	emitter_->transform.rotate = { 0.0f,0.0f,0.0f };
	emitter_->transform.scale = { 1.0f,1.0f,1.0f };
}

void ParticleSystem::Update()
{

	UpdateVertexBuffer();

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	emitter_->frequencyTime += kDeltaTime;
	if (isMakeParticle == true)
	{
		if (emitter_->frequency <= emitter_->frequencyTime)
		{
			particles.splice(particles.end(), Emission(emitter_, randomEngine, velocity_, isRandomPosition_, isRandomVelocity_));
			emitter_->frequencyTime -= emitter_->frequency;
		}
	}

	Matrix4x4 uvTransformMatrix_ = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix_ = Multiply(uvTransformMatrix_, MakeTranselateMatrix(uvTransform.translate));
	materialData_->uvTransform = uvTransformMatrix_;

	if (isInvisible_ == true)
	{
		return;
	}

	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 cameraMatrix = Inverse(camera_->matView);
	
	Matrix4x4 billboardMatrix;


	numInstance = 0;
	for (std::list<Particle>::iterator particleIterator = particles.begin(); particleIterator != particles.end(); )
	{
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime)
		{
			particleIterator = particles.erase(particleIterator);
			continue;
		}

		Matrix4x4 Scale = MakeScaleMatrix((*particleIterator).transform.scale);
		Matrix4x4 Translate = MakeTranselateMatrix((*particleIterator).transform.translate);
		Matrix4x4 worldMatrix;

	
		float alpha = 1.0f - particleIterator->currentTime / particleIterator->lifeTime;


		if (isBillboard_ == true)
		{
			billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
			billboardMatrix.m[3][0] = 0.0f;
			billboardMatrix.m[3][1] = 0.0f;
			billboardMatrix.m[3][2] = 0.0f;
			worldMatrix = Multiply(Scale, Multiply(billboardMatrix, Translate));
		}
		else
		{
			worldMatrix = MakeAffinMatrix(particleIterator->transform.scale, particleIterator->transform.rotate, particleIterator->transform.translate);
		}

	

		particleIterator->transform.translate.x += (*particleIterator).velocity.x * kDeltaTime;
		particleIterator->transform.translate.y += (*particleIterator).velocity.y * kDeltaTime;
		particleIterator->transform.translate.z += (*particleIterator).velocity.z * kDeltaTime;

		particleIterator->currentTime += kDeltaTime;


		if (numInstance < kNumMaxInstance)
		{
			instancingData[numInstance].WVP = worldMatrix;
			instancingData[numInstance].color = (*particleIterator).color;
			instancingData[numInstance].color.w = alpha;
			++numInstance;
		}

		++particleIterator;
	}

}

void ParticleSystem::Draw()
{


	dxCommon_->GetCommandList()->SetGraphicsRootSignature(psoManager_->GetPsoMember().particle.rootSignature.Get());
	dxCommon_->GetCommandList()->SetPipelineState(psoManager_->GetPsoMember().particle.graphicPipelineState.Get());

	//VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//ind
	dxCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferView_);
	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定する
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCbufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	//カメラ用のCBufferの場所を設定
	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, camera_->constBuffer_->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定。3はrootParamater[3]である。
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(3, texture_->GetSrvGPUHandle(textureHandle_));
	//描画
	/*dxCommon_->GetCommandList()->DrawInstanced(6, 1, 0, 0);*/
	dxCommon_->GetCommandList()->DrawIndexedInstanced(6, numInstance, 0, 0, 0);
}


void ParticleSystem::SetMaterialData(const Vector4 color)
{
	materialData_[0].color = color;
}

ParticleSystem* ParticleSystem::Create(uint32_t textureHandle, Camera* camera, Vector3 velocity, bool isRandomPosition, bool isRandomVelocity)
{
	ParticleSystem* sprite = new ParticleSystem();
	sprite->Initialize(textureHandle, camera, velocity, isRandomPosition, isRandomVelocity);
	return sprite;
}

void ParticleSystem::Debug(const char* name)
{
#ifdef _DEBUG
	ImGui::Begin("particle");
	if (ImGui::TreeNode(name))
	{
		if (ImGui::TreeNode("uv"))
		{
			ImGui::DragFloat2("UVTransform", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat2("UVScale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
			ImGui::SliderAngle("UVRotate", &uvTransform.rotate.z);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("emitter"))
		{
			float translate[3] = { emitter_->transform.translate.x,emitter_->transform.translate.y,emitter_->transform.translate.z };
			ImGui::DragFloat3("transform", translate, 0.1f);
			emitter_->transform.translate = { translate[0],translate[1],translate[2] };

			float scale[3] = { emitter_->transform.scale.x,emitter_->transform.scale.y,emitter_->transform.scale.z };
			ImGui::DragFloat3("scale", scale, 0.1f);
			emitter_->transform.scale = { scale[0],scale[1],scale[2] };

			ImGui::Checkbox("isRandomPosition", &isRandomPosition_);
			ImGui::Checkbox("isRandomVelocity", &isRandomVelocity_);
			ImGui::Checkbox("isBillboard", &isBillboard_);

			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::End();
#endif // _DEBUG

}



/*=====================================*/
/* 　　　　   プライベートメソッド　　　    */
/*=====================================*/


void ParticleSystem::VertexBuffer()
{
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 6);	//頂点用のデータ

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void ParticleSystem::MaterialBuffer()
{
	materialResource_ = dxCommon_->CreateBufferResource(sizeof(Material));	//マテリアル用のデータ

	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->uvTransform = MakeIdentity4x4();

}

void ParticleSystem::IndexBuffer()
{
	indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 6);
	//リソースの先頭のアドレスから使う
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	//インデックスリソースにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));
}

void ParticleSystem::UpdateVertexBuffer()
{


	//テクスチャのサイズを合わせる
	left = (0.0f - anchorPoint_.x) * cutSize_.x;
	right = (1.0f - anchorPoint_.x) * cutSize_.x;
	top = (0.0f - anchorPoint_.y) * cutSize_.y;
	bottom = (1.0f - anchorPoint_.y) * cutSize_.y;

	texLeft = textureLeftTop.x / textureSize_.x;
	texRight = (textureLeftTop.x + cutSize_.x) / textureSize_.x;
	texTop = textureLeftTop.y / textureSize_.y;
	texBottom = (textureLeftTop.y + cutSize_.y) / textureSize_.y;



	//頂点の設定
	vertexData_[LB].position = { left,bottom,0.0f,1.0f };
	vertexData_[LB].texcoord = { texLeft,texBottom };

	vertexData_[LT].position = { left,top,0.0f,1.0f };
	vertexData_[LT].texcoord = { texLeft,texTop };

	vertexData_[RB].position = { right,bottom,0.0f,1.0f };
	vertexData_[RB].texcoord = { texRight,texBottom };

	vertexData_[RT].position = { right,top,0.0f,1.0f };
	vertexData_[RT].texcoord = { texRight,texTop };


}


void ParticleSystem::AdjustTextureSize() {
	//テクスチャの情報を取得
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
	//テクスチャサイズの初期化

	textureSize_ = { float(resourceDesc_.Width),float(resourceDesc_.Height) };
	cutSize_ = textureSize_;

}

void ParticleSystem::InstancingBuffer()
{
	instancingResources_ = dxCommon_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	instancingResources_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
	{
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].color = { 1.0f,1.0f,1.0f,1.0f };
	}
}

void ParticleSystem::SetSRV()
{
	uint32_t srvIndex = srvManager_->Allocate();
	srvManager_->CreateSRVforStructuredBuffer(srvIndex, instancingResources_.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
	instancingSrvHandleGPU = srvManager_->GetGPUDescriptorHandle(srvIndex);
}

Particle ParticleSystem::MakeNewParticle(std::mt19937& randomEngine, Emitter* emitter, Vector3 velocity, bool isRandamTranslate, bool isRandamVelocity)
{
	std::uniform_real_distribution<float>distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float>distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float>distTime(lifeTime.min, lifeTime.max);

	// エミッターのスケールを取得
	Vector3 emitterScale = emitter->transform.scale;

	// パーティクルのランダムな位置を生成（エミッターのスケールを考慮）
	Vector3 randomTranslate = {
		distribution(randomEngine) ,
		distribution(randomEngine) ,
		distribution(randomEngine)
	};

	Particle particle;
	particle.transform.scale = { 0.005f,0.005f,0.005f };
	particle.transform.rotate = { 0.0f,3.14f,3.14f };

	if (isRandamTranslate == true)
	{
		//ランダムな位置に出現させる場合
		particle.transform.translate =
		{
			emitter->transform.translate.x + randomTranslate.x * emitterScale.x,
			emitter->transform.translate.y + randomTranslate.y * emitterScale.y,
			emitter->transform.translate.z + randomTranslate.z * emitterScale.z
		};
	}
	else
	{
		//一定の位置に出現させる場合
		particle.transform.translate =
		{
			emitter->transform.translate.x ,
			emitter->transform.translate.y ,
			emitter->transform.translate.z
		};
	}

	if (isRandamVelocity == true)
	{
		//ランダムな速度で動かす場合
		particle.velocity.x = velocity.x + randomTranslate.x;
		particle.velocity.y = velocity.y + randomTranslate.y;
		particle.velocity.z = velocity.z + randomTranslate.z;
	}
	else
	{
		//一定方向に動かす場合
		particle.velocity = velocity;
	}

	particle.color = { distColor(randomEngine) ,distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}

std::list<Particle> ParticleSystem::Emission(Emitter* emitter, std::mt19937& randomEngine, Vector3 velocity, bool isRandamTranslate, bool isRandomVelocity)
{
	std::list<Particle>particle;

	for (uint32_t count = 0; count < emitter->count; ++count)
	{
		particle.push_back(MakeNewParticle(randomEngine, emitter, velocity, isRandamTranslate, isRandomVelocity));
	}

	return particle;
}

/// <summary>
/// 粒を生成するのを防ぐ
/// </summary>
void ParticleSystem::StopMakeParticle()
{
	isMakeParticle = false;
}
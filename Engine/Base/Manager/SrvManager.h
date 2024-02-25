#pragma once
#include"DirectXCommon.h"
#include<wrl.h>



class SrvManager
{
public:
	void Initialize();
	void PreDraw();

	uint32_t Allocate();

	/*ゲッター*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	//確保可能チェック
	bool SRVAvailabilityCheck();

	/*セッター*/
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);



public:

private:

	
	//SRV生成(テクスチャ用
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	//SRV生成(ストラクチャーバッファ用
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);


private:
	DirectXCommon* dxCommon_ = nullptr;

	//最大SRV数
	static const uint32_t kMaxSRVCount;
	//SRV用のディスクリプタサイズ
	uint32_t descriptorSize;
	//SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap;

	//次に使用するSRVインデックス
	uint32_t useIndex = 0;


};


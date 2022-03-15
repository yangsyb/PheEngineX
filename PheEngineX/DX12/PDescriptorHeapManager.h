#pragma once
#include "pch.h"
// Allocate Strategy:
// 
namespace Phe
{
	class PDescriptorHeapManager
	{
	public:
		PDescriptorHeapManager();
		~PDescriptorHeapManager();

		void InitialzeHeap();

		void AllocateDescriptors();

		void DeAllocateDescriptors();



	private:
		UINT PRtvDescriptorSize;
		UINT PDsvDescriptorSize;
		UINT PCbvSrvUavDescriptorSize;

		ComPtr<ID3D12DescriptorHeap> PRtvHeap;
		ComPtr<ID3D12DescriptorHeap> PDsvHeap;
		ComPtr<ID3D12DescriptorHeap> PCbvSrvUavHeap;


	};
}
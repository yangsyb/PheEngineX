#pragma once
#include <guiddef.h>
#include <wincodec.h>
#include <wrl.h>
#include <d3d11_1.h>
#include "d3dx12.h"
#include <functional>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <assert.h>
#include "DDS.h"
#pragma comment(lib,"uuid.lib")
#include <tuple>
#include "DDSTextureSaver.h"

// HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW)
#define HRESULT_E_ARITHMETIC_OVERFLOW static_cast<HRESULT>(0x80070216L)

// HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED)
#define HRESULT_E_NOT_SUPPORTED static_cast<HRESULT>(0x80070032L)

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using WICPixelFormatGUID = GUID;


class auto_delete_file_wic
{
public:
	auto_delete_file_wic(ComPtr<IWICStream>& hFile, const wchar_t* szFile) noexcept : m_filename(szFile), m_handle(hFile) {}
	~auto_delete_file_wic()
	{
		if (m_filename)
		{
			m_handle.Reset();
			DeleteFileW(m_filename);
		}
	}

	auto_delete_file_wic(const auto_delete_file_wic&) = delete;
	auto_delete_file_wic& operator=(const auto_delete_file_wic&) = delete;

	auto_delete_file_wic(const auto_delete_file_wic&&) = delete;
	auto_delete_file_wic& operator=(const auto_delete_file_wic&&) = delete;

	void clear() noexcept { m_filename = nullptr; }

private:
	const wchar_t* m_filename;
	ComPtr<IWICStream>& m_handle;
};

BOOL WINAPI InitializeWICFactory(PINIT_ONCE, PVOID, PVOID* ifactory) noexcept
{
	return SUCCEEDED(CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		__uuidof(IWICImagingFactory2),
		ifactory)) ? TRUE : FALSE;
}

IWICImagingFactory2* GetWIC() noexcept
{
	static INIT_ONCE s_initOnce = INIT_ONCE_STATIC_INIT;

	IWICImagingFactory2* factory = nullptr;
	if (!InitOnceExecuteOnce(&s_initOnce,
		InitializeWICFactory,
		nullptr,
		reinterpret_cast<LPVOID*>(&factory)))
	{
		return nullptr;
	}

	return factory;
}


static HRESULT SaveWICTextureToFile(
	ID3D12CommandQueue* pCommandQ,
	ID3D12Resource* pSource,
	REFGUID guidContainerFormat,
	const wchar_t* fileName,
	const GUID* targetFormat,
	std::function<void(IPropertyBag2*)> setCustomProps,
	bool forceSRGB)
{
	if (!fileName)
		return E_INVALIDARG;

	ComPtr<ID3D12Device> device;
	pCommandQ->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(device.GetAddressOf()));

	// Get the size of the image
	const auto desc = pSource->GetDesc();

	if (desc.Width > UINT32_MAX)
		return E_INVALIDARG;

	UINT64 totalResourceSize = 0;
	UINT64 fpRowPitch = 0;
	UINT fpRowCount = 0;
	// Get the rowcount, pitch and size of the top mip
	device->GetCopyableFootprints(
		&desc,
		0,
		1,
		0,
		nullptr,
		&fpRowCount,
		&fpRowPitch,
		&totalResourceSize);

	// Round up the srcPitch to multiples of 256
	const UINT64 dstRowPitch = (fpRowPitch + 255) & ~0xFFu;

	if (dstRowPitch > UINT32_MAX)
		return HRESULT_E_ARITHMETIC_OVERFLOW;

	ComPtr<ID3D12Resource> pStaging;
	HRESULT hr = CaptureTexture(device.Get(), pCommandQ, pSource, dstRowPitch, desc, pStaging);
	if (FAILED(hr))
		return hr;

	// Determine source format's WIC equivalent
	WICPixelFormatGUID pfGuid = {};
	bool sRGB = forceSRGB;
	switch (desc.Format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:            pfGuid = GUID_WICPixelFormat128bppRGBAFloat; break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:            pfGuid = GUID_WICPixelFormat64bppRGBAHalf; break;
	case DXGI_FORMAT_R16G16B16A16_UNORM:            pfGuid = GUID_WICPixelFormat64bppRGBA; break;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:    pfGuid = GUID_WICPixelFormat32bppRGBA1010102XR; break;
	case DXGI_FORMAT_R10G10B10A2_UNORM:             pfGuid = GUID_WICPixelFormat32bppRGBA1010102; break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:                pfGuid = GUID_WICPixelFormat16bppBGRA5551; break;
	case DXGI_FORMAT_B5G6R5_UNORM:                  pfGuid = GUID_WICPixelFormat16bppBGR565; break;
	case DXGI_FORMAT_R32_FLOAT:                     pfGuid = GUID_WICPixelFormat32bppGrayFloat; break;
	case DXGI_FORMAT_R16_FLOAT:                     pfGuid = GUID_WICPixelFormat16bppGrayHalf; break;
	case DXGI_FORMAT_R16_UNORM:                     pfGuid = GUID_WICPixelFormat16bppGray; break;
	case DXGI_FORMAT_R8_UNORM:                      pfGuid = GUID_WICPixelFormat8bppGray; break;
	case DXGI_FORMAT_A8_UNORM:                      pfGuid = GUID_WICPixelFormat8bppAlpha; break;
	/// <summary>
	case DXGI_FORMAT_D24_UNORM_S8_UINT: pfGuid = GUID_WICPixelFormat32bppGrayFloat; break;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		pfGuid = GUID_WICPixelFormat32bppRGBA;
		break;

	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		pfGuid = GUID_WICPixelFormat32bppRGBA;
		sRGB = true;
		break;

	case DXGI_FORMAT_B8G8R8A8_UNORM:
		pfGuid = GUID_WICPixelFormat32bppBGRA;
		break;

	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		pfGuid = GUID_WICPixelFormat32bppBGRA;
		sRGB = true;
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM:
		pfGuid = GUID_WICPixelFormat32bppBGR;
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
		pfGuid = GUID_WICPixelFormat32bppBGR;
		sRGB = true;
		break;

	default:
		return HRESULT_E_NOT_SUPPORTED;
	}

	auto pWIC = GetWIC();
	if (!pWIC)
		return E_NOINTERFACE;

	ComPtr<IWICStream> stream;
	hr = pWIC->CreateStream(stream.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = stream->InitializeFromFilename(fileName, GENERIC_WRITE);
	if (FAILED(hr))
		return hr;

	auto_delete_file_wic delonfail(stream, fileName);

	ComPtr<IWICBitmapEncoder> encoder;
	hr = pWIC->CreateEncoder(guidContainerFormat, nullptr, encoder.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache);
	if (FAILED(hr))
		return hr;

	ComPtr<IWICBitmapFrameEncode> frame;
	ComPtr<IPropertyBag2> props;
	hr = encoder->CreateNewFrame(frame.GetAddressOf(), props.GetAddressOf());
	if (FAILED(hr))
		return hr;

	if (targetFormat && memcmp(&guidContainerFormat, &GUID_ContainerFormatBmp, sizeof(WICPixelFormatGUID)) == 0)
	{
		// Opt-in to the WIC2 support for writing 32-bit Windows BMP files with an alpha channel
		PROPBAG2 option = {};
		option.pstrName = const_cast<wchar_t*>(L"EnableV5Header32bppBGRA");

		VARIANT varValue;
		varValue.vt = VT_BOOL;
		varValue.boolVal = VARIANT_TRUE;
		std::ignore = props->Write(1, &option, &varValue);
	}

	if (setCustomProps)
	{
		setCustomProps(props.Get());
	}

	hr = frame->Initialize(props.Get());
	if (FAILED(hr))
		return hr;

	hr = frame->SetSize(static_cast<UINT>(desc.Width), desc.Height);
	if (FAILED(hr))
		return hr;

	hr = frame->SetResolution(72, 72);
	if (FAILED(hr))
		return hr;

	// Pick a target format
	WICPixelFormatGUID targetGuid = {};
	if (targetFormat)
	{
		targetGuid = *targetFormat;
	}
	else
	{
		// Screenshots don't typically include the alpha channel of the render target
		switch (desc.Format)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			targetGuid = GUID_WICPixelFormat96bppRGBFloat; // WIC 2
			break;

		case DXGI_FORMAT_R16G16B16A16_UNORM: targetGuid = GUID_WICPixelFormat48bppBGR; break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:     targetGuid = GUID_WICPixelFormat16bppBGR555; break;
		case DXGI_FORMAT_B5G6R5_UNORM:       targetGuid = GUID_WICPixelFormat16bppBGR565; break;

		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_A8_UNORM:
			targetGuid = GUID_WICPixelFormat8bppGray;
			break;

		default:
			targetGuid = GUID_WICPixelFormat24bppBGR;
			break;
		}
	}

	hr = frame->SetPixelFormat(&targetGuid);
	if (FAILED(hr))
		return hr;

	if (targetFormat && memcmp(targetFormat, &targetGuid, sizeof(WICPixelFormatGUID)) != 0)
	{
		// Requested output pixel format is not supported by the WIC codec
		return E_FAIL;
	}

	// Encode WIC metadata
	ComPtr<IWICMetadataQueryWriter> metawriter;
	if (SUCCEEDED(frame->GetMetadataQueryWriter(metawriter.GetAddressOf())))
	{
		PROPVARIANT value;
		PropVariantInit(&value);

		value.vt = VT_LPSTR;
		value.pszVal = const_cast<char*>("DirectXTK");

		if (memcmp(&guidContainerFormat, &GUID_ContainerFormatPng, sizeof(GUID)) == 0)
		{
			// Set Software name
			std::ignore = metawriter->SetMetadataByName(L"/tEXt/{str=Software}", &value);

			// Set sRGB chunk
			if (sRGB)
			{
				value.vt = VT_UI1;
				value.bVal = 0;
				std::ignore = metawriter->SetMetadataByName(L"/sRGB/RenderingIntent", &value);
			}
			else
			{
				// add gAMA chunk with gamma 1.0
				value.vt = VT_UI4;
				value.uintVal = 100000; // gama value * 100,000 -- i.e. gamma 1.0
				std::ignore = metawriter->SetMetadataByName(L"/gAMA/ImageGamma", &value);

				// remove sRGB chunk which is added by default.
				std::ignore = metawriter->RemoveMetadataByName(L"/sRGB/RenderingIntent");
			}
		}
		else
		{
			// Set Software name
			std::ignore = metawriter->SetMetadataByName(L"System.ApplicationName", &value);

			if (sRGB)
			{
				// Set EXIF Colorspace of sRGB
				value.vt = VT_UI2;
				value.uiVal = 1;
				std::ignore = metawriter->SetMetadataByName(L"System.Image.ColorSpace", &value);
			}
		}
	}

	const UINT64 imageSize = dstRowPitch * UINT64(desc.Height);
	if (imageSize > UINT32_MAX)
		return HRESULT_E_ARITHMETIC_OVERFLOW;

	void* pMappedMemory = nullptr;
	D3D12_RANGE readRange = { 0, static_cast<SIZE_T>(imageSize) };
	D3D12_RANGE writeRange = { 0, 0 };
	hr = pStaging->Map(0, &readRange, &pMappedMemory);
	if (FAILED(hr))
		return hr;

	if (memcmp(&targetGuid, &pfGuid, sizeof(WICPixelFormatGUID)) != 0)
	{
		// Conversion required to write
		ComPtr<IWICBitmap> source;
		hr = pWIC->CreateBitmapFromMemory(static_cast<UINT>(desc.Width), desc.Height,
			pfGuid,
			static_cast<UINT>(dstRowPitch), static_cast<UINT>(imageSize),
			static_cast<BYTE*>(pMappedMemory), source.GetAddressOf());
		if (FAILED(hr))
		{
			pStaging->Unmap(0, &writeRange);
			return hr;
		}

		ComPtr<IWICFormatConverter> FC;
		hr = pWIC->CreateFormatConverter(FC.GetAddressOf());
		if (FAILED(hr))
		{
			pStaging->Unmap(0, &writeRange);
			return hr;
		}

		BOOL canConvert = FALSE;
		hr = FC->CanConvert(pfGuid, targetGuid, &canConvert);
		if (FAILED(hr) || !canConvert)
		{
			pStaging->Unmap(0, &writeRange);
			return E_UNEXPECTED;
		}

		hr = FC->Initialize(source.Get(), targetGuid, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr))
		{
			pStaging->Unmap(0, &writeRange);
			return hr;
		}

		WICRect rect = { 0, 0, static_cast<INT>(desc.Width), static_cast<INT>(desc.Height) };
		hr = frame->WriteSource(FC.Get(), &rect);
	}
	else
	{
		// No conversion required
		hr = frame->WritePixels(desc.Height, static_cast<UINT>(dstRowPitch), static_cast<UINT>(imageSize), static_cast<BYTE*>(pMappedMemory));
	}

	pStaging->Unmap(0, &writeRange);

	if (FAILED(hr))
		return hr;

	hr = frame->Commit();
	if (FAILED(hr))
		return hr;

	hr = encoder->Commit();
	if (FAILED(hr))
		return hr;

	delonfail.clear();

	return S_OK;
}
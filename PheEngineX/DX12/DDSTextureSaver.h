#pragma once

#include <wrl.h>
#include <d3d11_1.h>
#include "d3dx12.h"

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <assert.h>
#include "DDS.h"
#pragma comment(lib,"uuid.lib")
#include <tuple>

struct handle_closer { void operator()(HANDLE h) noexcept { if (h) CloseHandle(h); } };

using ScopedHandle = std::unique_ptr<void, handle_closer>;

inline HANDLE safe_handle(HANDLE h) noexcept { return (h == INVALID_HANDLE_VALUE) ? nullptr : h; }

using Microsoft::WRL::ComPtr;
using namespace DirectX;

inline DXGI_FORMAT EnsureNotTypeless(DXGI_FORMAT fmt) noexcept
{
	// Assumes UNORM or FLOAT; doesn't use UINT or SINT
	switch (fmt)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case DXGI_FORMAT_R32G32B32_TYPELESS:    return DXGI_FORMAT_R32G32B32_FLOAT;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS: return DXGI_FORMAT_R16G16B16A16_UNORM;
	case DXGI_FORMAT_R32G32_TYPELESS:       return DXGI_FORMAT_R32G32_FLOAT;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:  return DXGI_FORMAT_R10G10B10A2_UNORM;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:     return DXGI_FORMAT_R8G8B8A8_UNORM;
	case DXGI_FORMAT_R16G16_TYPELESS:       return DXGI_FORMAT_R16G16_UNORM;
	case DXGI_FORMAT_R32_TYPELESS:          return DXGI_FORMAT_R32_FLOAT;
	case DXGI_FORMAT_R8G8_TYPELESS:         return DXGI_FORMAT_R8G8_UNORM;
	case DXGI_FORMAT_R16_TYPELESS:          return DXGI_FORMAT_R16_UNORM;
	case DXGI_FORMAT_R8_TYPELESS:           return DXGI_FORMAT_R8_UNORM;
	case DXGI_FORMAT_BC1_TYPELESS:          return DXGI_FORMAT_BC1_UNORM;
	case DXGI_FORMAT_BC2_TYPELESS:          return DXGI_FORMAT_BC2_UNORM;
	case DXGI_FORMAT_BC3_TYPELESS:          return DXGI_FORMAT_BC3_UNORM;
	case DXGI_FORMAT_BC4_TYPELESS:          return DXGI_FORMAT_BC4_UNORM;
	case DXGI_FORMAT_BC5_TYPELESS:          return DXGI_FORMAT_BC5_UNORM;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:     return DXGI_FORMAT_B8G8R8A8_UNORM;
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:     return DXGI_FORMAT_B8G8R8X8_UNORM;
	case DXGI_FORMAT_BC7_TYPELESS:          return DXGI_FORMAT_BC7_UNORM;
	default:                                return fmt;
	}
}

inline size_t BitsPerPixel(_In_ DXGI_FORMAT fmt) noexcept
{
	switch (fmt)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 128;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 96;

	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
	case DXGI_FORMAT_Y416:
	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		return 64;

	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:
	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
	case DXGI_FORMAT_AYUV:
	case DXGI_FORMAT_Y410:
	case DXGI_FORMAT_YUY2:
#if (defined(_XBOX_ONE) && defined(_TITLE)) || defined(_GAMING_XBOX)
	case DXGI_FORMAT_R10G10B10_7E3_A2_FLOAT:
	case DXGI_FORMAT_R10G10B10_6E4_A2_FLOAT:
	case DXGI_FORMAT_R10G10B10_SNORM_A2_UNORM:
#endif
		return 32;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	case DXGI_FORMAT_V408:
#endif
#if (defined(_XBOX_ONE) && defined(_TITLE)) || defined(_GAMING_XBOX)
	case DXGI_FORMAT_D16_UNORM_S8_UINT:
	case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
#endif
		return 24;

	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
	case DXGI_FORMAT_A8P8:
	case DXGI_FORMAT_B4G4R4A4_UNORM:
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	case DXGI_FORMAT_P208:
	case DXGI_FORMAT_V208:
#endif
		return 16;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
	case DXGI_FORMAT_NV11:
		return 12;

	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
	case DXGI_FORMAT_AI44:
	case DXGI_FORMAT_IA44:
	case DXGI_FORMAT_P8:
#if (defined(_XBOX_ONE) && defined(_TITLE)) || defined(_GAMING_XBOX)
	case DXGI_FORMAT_R4G4_UNORM:
#endif
		return 8;

	case DXGI_FORMAT_R1_UNORM:
		return 1;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		return 4;

	case DXGI_FORMAT_UNKNOWN:
	case DXGI_FORMAT_FORCE_UINT:
	default:
		return 0;
	}
}

inline bool IsCompressed(_In_ DXGI_FORMAT fmt) noexcept
{
	switch (fmt)
	{
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		return true;

	default:
		return false;
	}
}

 class auto_delete_file
 {
 public:
 	auto_delete_file(HANDLE hFile) noexcept : m_handle(hFile) {}
 
 	auto_delete_file(const auto_delete_file&) = delete;
 	auto_delete_file& operator=(const auto_delete_file&) = delete;
 
 	auto_delete_file(const auto_delete_file&&) = delete;
 	auto_delete_file& operator=(const auto_delete_file&&) = delete;
 
 	~auto_delete_file()
 	{
 		if (m_handle)
 		{
 			FILE_DISPOSITION_INFO info = {};
 			info.DeleteFile = TRUE;
 			std::ignore = SetFileInformationByHandle(m_handle, FileDispositionInfo, &info, sizeof(info));
 		}
 	}
 
 	void clear() noexcept { m_handle = nullptr; }
 
 private:
 	HANDLE m_handle;
 };
 

inline HRESULT GetSurfaceInfo(
	_In_ size_t width,
	_In_ size_t height,
	_In_ DXGI_FORMAT fmt,
	_Out_opt_ size_t* outNumBytes,
	_Out_opt_ size_t* outRowBytes,
	_Out_opt_ size_t* outNumRows) noexcept
{
	uint64_t numBytes = 0;
	uint64_t rowBytes = 0;
	uint64_t numRows = 0;

	bool bc = false;
	bool packed = false;
	bool planar = false;
	size_t bpe = 0;
	switch (fmt)
	{
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		bc = true;
		bpe = 8;
		break;

	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		bc = true;
		bpe = 16;
		break;

	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
	case DXGI_FORMAT_YUY2:
		packed = true;
		bpe = 4;
		break;

	case DXGI_FORMAT_Y210:
	case DXGI_FORMAT_Y216:
		packed = true;
		bpe = 8;
		break;

	case DXGI_FORMAT_NV12:
	case DXGI_FORMAT_420_OPAQUE:
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
	case DXGI_FORMAT_P208:
#endif
		planar = true;
		bpe = 2;
		break;

	case DXGI_FORMAT_P010:
	case DXGI_FORMAT_P016:
		planar = true;
		bpe = 4;
		break;

#if (defined(_XBOX_ONE) && defined(_TITLE)) || defined(_GAMING_XBOX)

	case DXGI_FORMAT_D16_UNORM_S8_UINT:
	case DXGI_FORMAT_R16_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X16_TYPELESS_G8_UINT:
		planar = true;
		bpe = 4;
		break;

#endif

	default:
		break;
	}

	if (bc)
	{
		uint64_t numBlocksWide = 0;
		if (width > 0)
		{
			numBlocksWide = std::max<uint64_t>(1u, (uint64_t(width) + 3u) / 4u);
		}
		uint64_t numBlocksHigh = 0;
		if (height > 0)
		{
			numBlocksHigh = std::max<uint64_t>(1u, (uint64_t(height) + 3u) / 4u);
		}
		rowBytes = numBlocksWide * bpe;
		numRows = numBlocksHigh;
		numBytes = rowBytes * numBlocksHigh;
	}
	else if (packed)
	{
		rowBytes = ((uint64_t(width) + 1u) >> 1) * bpe;
		numRows = uint64_t(height);
		numBytes = rowBytes * height;
	}
	else if (fmt == DXGI_FORMAT_NV11)
	{
		rowBytes = ((uint64_t(width) + 3u) >> 2) * 4u;
		numRows = uint64_t(height) * 2u; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
		numBytes = rowBytes * numRows;
	}
	else if (planar)
	{
		rowBytes = ((uint64_t(width) + 1u) >> 1) * bpe;
		numBytes = (rowBytes * uint64_t(height)) + ((rowBytes * uint64_t(height) + 1u) >> 1);
		numRows = height + ((uint64_t(height) + 1u) >> 1);
	}
	else
	{
		const size_t bpp = BitsPerPixel(fmt);
		if (!bpp)
			return E_INVALIDARG;

		rowBytes = (uint64_t(width) * bpp + 7u) / 8u; // round up to nearest byte
		numRows = uint64_t(height);
		numBytes = rowBytes * height;
	}

#if defined(_M_IX86) || defined(_M_ARM) || defined(_M_HYBRID_X86_ARM64)
	static_assert(sizeof(size_t) == 4, "Not a 32-bit platform!");
	if (numBytes > UINT32_MAX || rowBytes > UINT32_MAX || numRows > UINT32_MAX)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
#else
	static_assert(sizeof(size_t) == 8, "Not a 64-bit platform!");
#endif

	if (outNumBytes)
	{
		*outNumBytes = static_cast<size_t>(numBytes);
	}
	if (outRowBytes)
	{
		*outRowBytes = static_cast<size_t>(rowBytes);
	}
	if (outNumRows)
	{
		*outNumRows = static_cast<size_t>(numRows);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
#define ISBITMASK( r,g,b,a ) ( ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a )

inline DXGI_FORMAT GetDXGIFormat(const DDS_PIXELFORMAT& ddpf) noexcept
{
	if (ddpf.flags & DDS_RGB)
	{
		// Note that sRGB formats are written using the "DX10" extended header

		switch (ddpf.RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
			{
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
			{
				return DXGI_FORMAT_B8G8R8A8_UNORM;
			}

			if (ISBITMASK(0x00ff0000, 0x0000ff00, 0x000000ff, 0))
			{
				return DXGI_FORMAT_B8G8R8X8_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0) aka D3DFMT_X8B8G8R8

			// Note that many common DDS reader/writers (including D3DX) swap the
			// the RED/BLUE masks for 10:10:10:2 formats. We assume
			// below that the 'backwards' header mask is being used since it is most
			// likely written by D3DX. The more robust solution is to use the 'DX10'
			// header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

			// For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
			if (ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
			{
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

			if (ISBITMASK(0x0000ffff, 0xffff0000, 0, 0))
			{
				return DXGI_FORMAT_R16G16_UNORM;
			}

			if (ISBITMASK(0xffffffff, 0, 0, 0))
			{
				// Only 32-bit color channel format in D3D9 was R32F
				return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
			}
			break;

		case 24:
			// No 24bpp DXGI formats aka D3DFMT_R8G8B8
			break;

		case 16:
			if (ISBITMASK(0x7c00, 0x03e0, 0x001f, 0x8000))
			{
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			}
			if (ISBITMASK(0xf800, 0x07e0, 0x001f, 0))
			{
				return DXGI_FORMAT_B5G6R5_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0) aka D3DFMT_X1R5G5B5

			if (ISBITMASK(0x0f00, 0x00f0, 0x000f, 0xf000))
			{
				return DXGI_FORMAT_B4G4R4A4_UNORM;
			}

			// NVTT versions 1.x wrote this as RGB instead of LUMINANCE
			if (ISBITMASK(0x00ff, 0, 0, 0xff00))
			{
				return DXGI_FORMAT_R8G8_UNORM;
			}
			if (ISBITMASK(0xffff, 0, 0, 0))
			{
				return DXGI_FORMAT_R16_UNORM;
			}

			// No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0) aka D3DFMT_X4R4G4B4

			// No 3:3:2:8 or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_A8P8, etc.
			break;

		case 8:
			// NVTT versions 1.x wrote this as RGB instead of LUMINANCE
			if (ISBITMASK(0xff, 0, 0, 0))
			{
				return DXGI_FORMAT_R8_UNORM;
			}

			// No 3:3:2 or paletted DXGI formats aka D3DFMT_R3G3B2, D3DFMT_P8
			break;
		}
	}
	else if (ddpf.flags & DDS_LUMINANCE)
	{
		switch (ddpf.RGBBitCount)
		{
		case 16:
			if (ISBITMASK(0xffff, 0, 0, 0))
			{
				return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
			if (ISBITMASK(0x00ff, 0, 0, 0xff00))
			{
				return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}
			break;

		case 8:
			if (ISBITMASK(0xff, 0, 0, 0))
			{
				return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
			}

			// No DXGI format maps to ISBITMASK(0x0f,0,0,0xf0) aka D3DFMT_A4L4

			if (ISBITMASK(0x00ff, 0, 0, 0xff00))
			{
				return DXGI_FORMAT_R8G8_UNORM; // Some DDS writers assume the bitcount should be 8 instead of 16
			}
			break;
		}
	}
	else if (ddpf.flags & DDS_ALPHA)
	{
		if (8 == ddpf.RGBBitCount)
		{
			return DXGI_FORMAT_A8_UNORM;
		}
	}
	else if (ddpf.flags & DDS_BUMPDUDV)
	{
		switch (ddpf.RGBBitCount)
		{
		case 32:
			if (ISBITMASK(0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
			{
				return DXGI_FORMAT_R8G8B8A8_SNORM; // D3DX10/11 writes this out as DX10 extension
			}
			if (ISBITMASK(0x0000ffff, 0xffff0000, 0, 0))
			{
				return DXGI_FORMAT_R16G16_SNORM; // D3DX10/11 writes this out as DX10 extension
			}

			// No DXGI format maps to ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000) aka D3DFMT_A2W10V10U10
			break;

		case 16:
			if (ISBITMASK(0x00ff, 0xff00, 0, 0))
			{
				return DXGI_FORMAT_R8G8_SNORM; // D3DX10/11 writes this out as DX10 extension
			}
			break;
		}

		// No DXGI format maps to DDPF_BUMPLUMINANCE aka D3DFMT_L6V5U5, D3DFMT_X8L8V8U8
	}
	else if (ddpf.flags & DDS_FOURCC)
	{
		if (MAKEFOURCC('D', 'X', 'T', '1') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC1_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '3') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '5') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		// While pre-multiplied alpha isn't directly supported by the DXGI formats,
		// they are basically the same as these BC formats so they can be mapped
		if (MAKEFOURCC('D', 'X', 'T', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC2_UNORM;
		}
		if (MAKEFOURCC('D', 'X', 'T', '4') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC3_UNORM;
		}

		if (MAKEFOURCC('A', 'T', 'I', '1') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '4', 'U') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '4', 'S') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC4_SNORM;
		}

		if (MAKEFOURCC('A', 'T', 'I', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '5', 'U') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_UNORM;
		}
		if (MAKEFOURCC('B', 'C', '5', 'S') == ddpf.fourCC)
		{
			return DXGI_FORMAT_BC5_SNORM;
		}

		// BC6H and BC7 are written using the "DX10" extended header

		if (MAKEFOURCC('R', 'G', 'B', 'G') == ddpf.fourCC)
		{
			return DXGI_FORMAT_R8G8_B8G8_UNORM;
		}
		if (MAKEFOURCC('G', 'R', 'G', 'B') == ddpf.fourCC)
		{
			return DXGI_FORMAT_G8R8_G8B8_UNORM;
		}

		if (MAKEFOURCC('Y', 'U', 'Y', '2') == ddpf.fourCC)
		{
			return DXGI_FORMAT_YUY2;
		}

		// Check for D3DFORMAT enums being set here
		switch (ddpf.fourCC)
		{
		case 36: // D3DFMT_A16B16G16R16
			return DXGI_FORMAT_R16G16B16A16_UNORM;

		case 110: // D3DFMT_Q16W16V16U16
			return DXGI_FORMAT_R16G16B16A16_SNORM;

		case 111: // D3DFMT_R16F
			return DXGI_FORMAT_R16_FLOAT;

		case 112: // D3DFMT_G16R16F
			return DXGI_FORMAT_R16G16_FLOAT;

		case 113: // D3DFMT_A16B16G16R16F
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		case 114: // D3DFMT_R32F
			return DXGI_FORMAT_R32_FLOAT;

		case 115: // D3DFMT_G32R32F
			return DXGI_FORMAT_R32G32_FLOAT;

		case 116: // D3DFMT_A32B32G32R32F
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

			// No DXGI format maps to D3DFMT_CxV8U8
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}




// static HRESULT SaveDDSTextureToFile(
// 	D3D12_RESOURCE_DESC TextureDesc,
// 	ComPtr<ID3D12Resource> StagingResource,
// 	UINT64 DstRowPitch,
// 	const wchar_t* fileName) noexcept
// {
// 	if (!fileName)
// 		return E_INVALIDARG;
// 
// 	// Create file
// 	ScopedHandle hFile(safe_handle(CreateFile2(
// 		fileName,
// 		GENERIC_WRITE, 0, CREATE_ALWAYS,
// 		nullptr)));
// 	if (!hFile)
// 		return HRESULT_FROM_WIN32(GetLastError());
// 
// 	auto_delete_file delonfail(hFile.get());
// 
// 	// Setup header
// 	constexpr size_t MAX_HEADER_SIZE = sizeof(uint32_t) + sizeof(DDS_HEADER) + sizeof(DDS_HEADER_DXT10);
// 	uint8_t fileHeader[MAX_HEADER_SIZE] = {};
// 
// 	*reinterpret_cast<uint32_t*>(&fileHeader[0]) = DDS_MAGIC;
// 
// 	auto header = reinterpret_cast<DDS_HEADER*>(&fileHeader[0] + sizeof(uint32_t));
// 	size_t headerSize = sizeof(uint32_t) + sizeof(DDS_HEADER);
// 	header->size = sizeof(DDS_HEADER);
// 	header->flags = DDS_HEADER_FLAGS_TEXTURE | DDS_HEADER_FLAGS_MIPMAP;
// 	header->height = TextureDesc.Height;
// 	header->width = static_cast<uint32_t>(TextureDesc.Width);
// 	header->mipMapCount = 1;
// 	header->caps = DDS_SURFACE_FLAGS_TEXTURE;
// 
// 	DDS_HEADER_DXT10* extHeader = nullptr;
// 	switch (TextureDesc.Format)
// 	{
// 	case DXGI_FORMAT_R8G8B8A8_UNORM:        memcpy(&header->ddspf, &DDSPF_A8B8G8R8, sizeof(DDS_PIXELFORMAT));    break;
// 	case DXGI_FORMAT_R16G16_UNORM:          memcpy(&header->ddspf, &DDSPF_G16R16, sizeof(DDS_PIXELFORMAT));      break;
// 	case DXGI_FORMAT_R8G8_UNORM:            memcpy(&header->ddspf, &DDSPF_A8L8, sizeof(DDS_PIXELFORMAT));        break;
// 	case DXGI_FORMAT_R16_UNORM:             memcpy(&header->ddspf, &DDSPF_L16, sizeof(DDS_PIXELFORMAT));         break;
// 	case DXGI_FORMAT_R8_UNORM:              memcpy(&header->ddspf, &DDSPF_L8, sizeof(DDS_PIXELFORMAT));          break;
// 	case DXGI_FORMAT_A8_UNORM:              memcpy(&header->ddspf, &DDSPF_A8, sizeof(DDS_PIXELFORMAT));          break;
// 	case DXGI_FORMAT_R8G8_B8G8_UNORM:       memcpy(&header->ddspf, &DDSPF_R8G8_B8G8, sizeof(DDS_PIXELFORMAT));   break;
// 	case DXGI_FORMAT_G8R8_G8B8_UNORM:       memcpy(&header->ddspf, &DDSPF_G8R8_G8B8, sizeof(DDS_PIXELFORMAT));   break;
// 	case DXGI_FORMAT_BC1_UNORM:             memcpy(&header->ddspf, &DDSPF_DXT1, sizeof(DDS_PIXELFORMAT));        break;
// 	case DXGI_FORMAT_BC2_UNORM:             memcpy(&header->ddspf, &DDSPF_DXT3, sizeof(DDS_PIXELFORMAT));        break;
// 	case DXGI_FORMAT_BC3_UNORM:             memcpy(&header->ddspf, &DDSPF_DXT5, sizeof(DDS_PIXELFORMAT));        break;
// 	case DXGI_FORMAT_BC4_UNORM:             memcpy(&header->ddspf, &DDSPF_BC4_UNORM, sizeof(DDS_PIXELFORMAT));   break;
// 	case DXGI_FORMAT_BC4_SNORM:             memcpy(&header->ddspf, &DDSPF_BC4_SNORM, sizeof(DDS_PIXELFORMAT));   break;
// 	case DXGI_FORMAT_BC5_UNORM:             memcpy(&header->ddspf, &DDSPF_BC5_UNORM, sizeof(DDS_PIXELFORMAT));   break;
// 	case DXGI_FORMAT_BC5_SNORM:             memcpy(&header->ddspf, &DDSPF_BC5_SNORM, sizeof(DDS_PIXELFORMAT));   break;
// 	case DXGI_FORMAT_B5G6R5_UNORM:          memcpy(&header->ddspf, &DDSPF_R5G6B5, sizeof(DDS_PIXELFORMAT));      break;
// 	case DXGI_FORMAT_B5G5R5A1_UNORM:        memcpy(&header->ddspf, &DDSPF_A1R5G5B5, sizeof(DDS_PIXELFORMAT));    break;
// 	case DXGI_FORMAT_R8G8_SNORM:            memcpy(&header->ddspf, &DDSPF_V8U8, sizeof(DDS_PIXELFORMAT));        break;
// 	case DXGI_FORMAT_R8G8B8A8_SNORM:        memcpy(&header->ddspf, &DDSPF_Q8W8V8U8, sizeof(DDS_PIXELFORMAT));    break;
// 	case DXGI_FORMAT_R16G16_SNORM:          memcpy(&header->ddspf, &DDSPF_V16U16, sizeof(DDS_PIXELFORMAT));      break;
// 	case DXGI_FORMAT_B8G8R8A8_UNORM:        memcpy(&header->ddspf, &DDSPF_A8R8G8B8, sizeof(DDS_PIXELFORMAT));    break;
// 	case DXGI_FORMAT_B8G8R8X8_UNORM:        memcpy(&header->ddspf, &DDSPF_X8R8G8B8, sizeof(DDS_PIXELFORMAT));    break;
// 	case DXGI_FORMAT_YUY2:                  memcpy(&header->ddspf, &DDSPF_YUY2, sizeof(DDS_PIXELFORMAT));        break;
// 	case DXGI_FORMAT_B4G4R4A4_UNORM:        memcpy(&header->ddspf, &DDSPF_A4R4G4B4, sizeof(DDS_PIXELFORMAT));    break;
// 
// 		// Legacy D3DX formats using D3DFMT enum value as FourCC
// 	case DXGI_FORMAT_R32G32B32A32_FLOAT:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 116; break; // D3DFMT_A32B32G32R32F
// 	case DXGI_FORMAT_R16G16B16A16_FLOAT:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 113; break; // D3DFMT_A16B16G16R16F
// 	case DXGI_FORMAT_R16G16B16A16_UNORM:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 36;  break; // D3DFMT_A16B16G16R16
// 	case DXGI_FORMAT_R16G16B16A16_SNORM:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 110; break; // D3DFMT_Q16W16V16U16
// 	case DXGI_FORMAT_R32G32_FLOAT:          header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 115; break; // D3DFMT_G32R32F
// 	case DXGI_FORMAT_R16G16_FLOAT:          header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 112; break; // D3DFMT_G16R16F
// 	case DXGI_FORMAT_R32_FLOAT:             header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 114; break; // D3DFMT_R32F
// 	case DXGI_FORMAT_R16_FLOAT:             header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 111; break; // D3DFMT_R16F
// 
// 	case DXGI_FORMAT_AI44:
// 	case DXGI_FORMAT_IA44:
// 	case DXGI_FORMAT_P8:
// 	case DXGI_FORMAT_A8P8:
// 		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
// 
// 	default:
// 		memcpy(&header->ddspf, &DDSPF_DX10, sizeof(DDS_PIXELFORMAT));
// 
// 		headerSize += sizeof(DDS_HEADER_DXT10);
// 		extHeader = reinterpret_cast<DDS_HEADER_DXT10*>(fileHeader + sizeof(uint32_t) + sizeof(DDS_HEADER));
// 		extHeader->dxgiFormat = TextureDesc.Format;
// 		extHeader->resourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
// 		extHeader->arraySize = 1;
// 		break;
// 	}
// 
// 	size_t rowPitch, slicePitch, rowCount;
// 	HRESULT hr = GetSurfaceInfo(static_cast<size_t>(TextureDesc.Width), TextureDesc.Height, TextureDesc.Format, &slicePitch, &rowPitch, &rowCount);
// 	if (FAILED(hr))
// 		return hr;
// 
// 	if (rowPitch > UINT32_MAX || slicePitch > UINT32_MAX)
// 		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
// 
// 	if (IsCompressed(TextureDesc.Format))
// 	{
// 		header->flags |= DDS_HEADER_FLAGS_LINEARSIZE;
// 		header->pitchOrLinearSize = static_cast<uint32_t>(slicePitch);
// 	}
// 	else
// 	{
// 		header->flags |= DDS_HEADER_FLAGS_PITCH;
// 		header->pitchOrLinearSize = static_cast<uint32_t>(rowPitch);
// 	}
// 
// 	// Setup pixels
// 	std::unique_ptr<uint8_t[]> pixels(new (std::nothrow) uint8_t[slicePitch]);
// 	if (!pixels)
// 		return E_OUTOFMEMORY;
// 
// 
// 	const UINT64 imageSize = DstRowPitch * UINT64(rowCount);
// 	if (imageSize > UINT32_MAX)
// 		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
// 
// 	void* pMappedMemory = nullptr;
// 	D3D12_RANGE readRange = { 0, static_cast<SIZE_T>(imageSize) };
// 	D3D12_RANGE writeRange = { 0, 0 };
// 	hr = StagingResource->Map(0, &readRange, &pMappedMemory);
// 	if (FAILED(hr))
// 		return hr;
// 
// 	auto sptr = static_cast<const uint8_t*>(pMappedMemory);
// 	if (!sptr)
// 	{
// 		StagingResource->Unmap(0, &writeRange);
// 		return E_POINTER;
// 	}
// 
// 	uint8_t* dptr = pixels.get();
// 
// 	const size_t msize = std::min<size_t>(rowPitch, size_t(DstRowPitch));
// 	for (size_t h = 0; h < rowCount; ++h)
// 	{
// 		memcpy(dptr, sptr, msize);
// 		sptr += DstRowPitch;
// 		dptr += rowPitch;
// 	}
// 
// 	StagingResource->Unmap(0, &writeRange);
// 
// 	// Write header & pixels
// 	DWORD bytesWritten;
// 	if (!WriteFile(hFile.get(), fileHeader, static_cast<DWORD>(headerSize), &bytesWritten, nullptr))
// 		return HRESULT_FROM_WIN32(GetLastError());
// 
// 	if (bytesWritten != headerSize)
// 		return E_FAIL;
// 
// 	if (!WriteFile(hFile.get(), pixels.get(), static_cast<DWORD>(slicePitch), &bytesWritten, nullptr))
// 		return HRESULT_FROM_WIN32(GetLastError());
// 
// 	if (bytesWritten != slicePitch)
// 		return E_FAIL;
// 
// 	delonfail.clear();
// 
// 	return S_OK;
// }


static HRESULT CaptureTexture(_In_ ID3D12Device* device,
	_In_ ID3D12CommandQueue* pCommandQ,
	_In_ ID3D12Resource* pSource,
	UINT64 srcPitch,
	const D3D12_RESOURCE_DESC& desc,
	ComPtr<ID3D12Resource>& pStaging) noexcept
{

	if (!pCommandQ || !pSource)
		return E_INVALIDARG;

	if (desc.Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
	}

	if (desc.DepthOrArraySize > 1 || desc.MipLevels > 1)
	{
	}

	if (srcPitch > UINT32_MAX)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

 	const UINT numberOfPlanes = D3D12GetFormatPlaneCount(device, desc.Format);
 	if (numberOfPlanes != 1)
 		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

	D3D12_HEAP_PROPERTIES sourceHeapProperties;
	HRESULT hr = pSource->GetHeapProperties(&sourceHeapProperties, nullptr);
	if (SUCCEEDED(hr) && sourceHeapProperties.Type == D3D12_HEAP_TYPE_READBACK)
	{
		// Handle case where the source is already a staging texture we can use directly
		pStaging = pSource;
		return S_OK;
	}

	// Create a command allocator
	ComPtr<ID3D12CommandAllocator> commandAlloc;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAlloc.GetAddressOf()));
	if (FAILED(hr))
		return hr;


	// Spin up a new command list
	ComPtr<ID3D12GraphicsCommandList> commandList;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAlloc.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf()));
	if (FAILED(hr))
		return hr;

	// Create a fence
	ComPtr<ID3D12Fence> fence;
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
	if (FAILED(hr))
		return hr;


	assert((srcPitch & 0xFF) == 0);

	const CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	const CD3DX12_HEAP_PROPERTIES readBackHeapProperties(D3D12_HEAP_TYPE_READBACK);

	// Readback resources must be buffers
	D3D12_RESOURCE_DESC bufferDesc = {};
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.Height = 1;
	bufferDesc.Width = srcPitch * desc.Height;
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc.Count = 1;

	ComPtr<ID3D12Resource> copySource(pSource);
	if (desc.SampleDesc.Count > 1)
	{
		// MSAA content must be resolved before being copied to a staging texture
		auto descCopy = desc;
		descCopy.SampleDesc.Count = 1;
		descCopy.SampleDesc.Quality = 0;
		descCopy.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;

		ComPtr<ID3D12Resource> pTemp;
		hr = device->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&descCopy,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(pTemp.GetAddressOf()));
		if (FAILED(hr))
			return hr;

		assert(pTemp);


		const DXGI_FORMAT fmt = EnsureNotTypeless(desc.Format);

		D3D12_FEATURE_DATA_FORMAT_SUPPORT formatInfo = { fmt, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };
		hr = device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatInfo, sizeof(formatInfo));
		if (FAILED(hr))
			return hr;

		if (!(formatInfo.Support1 & D3D12_FORMAT_SUPPORT1_TEXTURE2D))
			return E_FAIL;

		for (UINT item = 0; item < desc.DepthOrArraySize; ++item)
		{
			for (UINT level = 0; level < desc.MipLevels; ++level)
			{
				const UINT index = D3D12CalcSubresource(level, item, 0, desc.MipLevels, desc.DepthOrArraySize);
				commandList->ResolveSubresource(pTemp.Get(), index, pSource, index, fmt);
			}
		}

		copySource = pTemp;
	}

	// Create a staging texture
	hr = device->CreateCommittedResource(
		&readBackHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(pStaging.ReleaseAndGetAddressOf()));
	if (FAILED(hr))
		return hr;


	assert(pStaging);

	// Transition the resource if necessary
//	TransitionResource(commandList.Get(), pSource, beforeState, D3D12_RESOURCE_STATE_COPY_SOURCE);

	CD3DX12_RESOURCE_BARRIER PreResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pSource, D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_SOURCE);
	commandList.Get()->ResourceBarrier(1, &PreResourceBarrier);

	// Get the copy target location
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT bufferFootprint = {};
	bufferFootprint.Footprint.Width = static_cast<UINT>(desc.Width);
	bufferFootprint.Footprint.Height = desc.Height;
	bufferFootprint.Footprint.Depth = 1;
	bufferFootprint.Footprint.RowPitch = static_cast<UINT>(srcPitch);
	bufferFootprint.Footprint.Format = desc.Format;

	const CD3DX12_TEXTURE_COPY_LOCATION copyDest(pStaging.Get(), bufferFootprint);
	const CD3DX12_TEXTURE_COPY_LOCATION copySrc(copySource.Get(), 0);

	// Copy the texture
	commandList->CopyTextureRegion(&copyDest, 0, 0, 0, &copySrc, nullptr);

	// Transition the resource to the next state
//	TransitionResource(commandList.Get(), pSource, D3D12_RESOURCE_STATE_COPY_SOURCE, afterState);

	CD3DX12_RESOURCE_BARRIER AfterResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(pSource, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_GENERIC_READ);
	commandList.Get()->ResourceBarrier(1, &AfterResourceBarrier);

	hr = commandList->Close();
	if (FAILED(hr))
		return hr;

	// Execute the command list
	pCommandQ->ExecuteCommandLists(1, CommandListCast(commandList.GetAddressOf()));

	// Signal the fence
	hr = pCommandQ->Signal(fence.Get(), 1);
	if (FAILED(hr))
		return hr;

	// Block until the copy is complete
	while (fence->GetCompletedValue() < 1)
		SwitchToThread();

	return S_OK;
}
// anonymous namespace



static HRESULT SaveDDSTextureToFile(
	ID3D12CommandQueue* pCommandQ,
	ID3D12Resource* pSource,
	const wchar_t* fileName) noexcept
{
	if (!fileName)
		return E_INVALIDARG;

	ComPtr<ID3D12Device> device;
	pCommandQ->GetDevice(IID_ID3D12Device, reinterpret_cast<void**>(device.GetAddressOf()));

	// Get the size of the image
	auto desc = pSource->GetDesc();

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


	ComPtr<ID3D12Resource> pStaging;
	desc.Format= DXGI_FORMAT_R32_TYPELESS;
	HRESULT hr = CaptureTexture(device.Get(), pCommandQ, pSource, dstRowPitch, desc, pStaging);
	if (FAILED(hr))
		return hr;

	// Create file
	ScopedHandle hFile(safe_handle(CreateFile2(fileName, GENERIC_WRITE, 0, CREATE_ALWAYS, nullptr)));
	if (!hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	auto_delete_file delonfail(hFile.get());


	// Setup header
	constexpr size_t MAX_HEADER_SIZE = sizeof(uint32_t) + sizeof(DDS_HEADER) + sizeof(DDS_HEADER_DXT10);
	uint8_t fileHeader[MAX_HEADER_SIZE] = {};

	*reinterpret_cast<uint32_t*>(&fileHeader[0]) = DDS_MAGIC;

	auto header = reinterpret_cast<DDS_HEADER*>(&fileHeader[0] + sizeof(uint32_t));
	size_t headerSize = sizeof(uint32_t) + sizeof(DDS_HEADER);
	header->size = sizeof(DDS_HEADER);
	header->flags = DDS_HEADER_FLAGS_TEXTURE | DDS_HEADER_FLAGS_MIPMAP;
	header->height = desc.Height;
	header->width = static_cast<uint32_t>(desc.Width);
	header->mipMapCount = 1;
	header->caps = DDS_SURFACE_FLAGS_TEXTURE;

	// Try to use a legacy .DDS pixel format for better tools support, otherwise fallback to 'DX10' header extension
	DDS_HEADER_DXT10* extHeader = nullptr;
	switch (desc.Format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM:        memcpy(&header->ddspf, &DDSPF_A8B8G8R8, sizeof(DDS_PIXELFORMAT));    break;
	case DXGI_FORMAT_R16G16_UNORM:          memcpy(&header->ddspf, &DDSPF_G16R16, sizeof(DDS_PIXELFORMAT));      break;
	case DXGI_FORMAT_R8G8_UNORM:            memcpy(&header->ddspf, &DDSPF_A8L8, sizeof(DDS_PIXELFORMAT));        break;
	case DXGI_FORMAT_R16_UNORM:             memcpy(&header->ddspf, &DDSPF_L16, sizeof(DDS_PIXELFORMAT));         break;
	case DXGI_FORMAT_R8_UNORM:              memcpy(&header->ddspf, &DDSPF_L8, sizeof(DDS_PIXELFORMAT));          break;
	case DXGI_FORMAT_A8_UNORM:              memcpy(&header->ddspf, &DDSPF_A8, sizeof(DDS_PIXELFORMAT));          break;
	case DXGI_FORMAT_R8G8_B8G8_UNORM:       memcpy(&header->ddspf, &DDSPF_R8G8_B8G8, sizeof(DDS_PIXELFORMAT));   break;
	case DXGI_FORMAT_G8R8_G8B8_UNORM:       memcpy(&header->ddspf, &DDSPF_G8R8_G8B8, sizeof(DDS_PIXELFORMAT));   break;
	case DXGI_FORMAT_BC1_UNORM:             memcpy(&header->ddspf, &DDSPF_DXT1, sizeof(DDS_PIXELFORMAT));        break;
	case DXGI_FORMAT_BC2_UNORM:             memcpy(&header->ddspf, &DDSPF_DXT3, sizeof(DDS_PIXELFORMAT));        break;
	case DXGI_FORMAT_BC3_UNORM:             memcpy(&header->ddspf, &DDSPF_DXT5, sizeof(DDS_PIXELFORMAT));        break;
	case DXGI_FORMAT_BC4_UNORM:             memcpy(&header->ddspf, &DDSPF_BC4_UNORM, sizeof(DDS_PIXELFORMAT));   break;
	case DXGI_FORMAT_BC4_SNORM:             memcpy(&header->ddspf, &DDSPF_BC4_SNORM, sizeof(DDS_PIXELFORMAT));   break;
	case DXGI_FORMAT_BC5_UNORM:             memcpy(&header->ddspf, &DDSPF_BC5_UNORM, sizeof(DDS_PIXELFORMAT));   break;
	case DXGI_FORMAT_BC5_SNORM:             memcpy(&header->ddspf, &DDSPF_BC5_SNORM, sizeof(DDS_PIXELFORMAT));   break;
	case DXGI_FORMAT_B5G6R5_UNORM:          memcpy(&header->ddspf, &DDSPF_R5G6B5, sizeof(DDS_PIXELFORMAT));      break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:        memcpy(&header->ddspf, &DDSPF_A1R5G5B5, sizeof(DDS_PIXELFORMAT));    break;
	case DXGI_FORMAT_R8G8_SNORM:            memcpy(&header->ddspf, &DDSPF_V8U8, sizeof(DDS_PIXELFORMAT));        break;
	case DXGI_FORMAT_R8G8B8A8_SNORM:        memcpy(&header->ddspf, &DDSPF_Q8W8V8U8, sizeof(DDS_PIXELFORMAT));    break;
	case DXGI_FORMAT_R16G16_SNORM:          memcpy(&header->ddspf, &DDSPF_V16U16, sizeof(DDS_PIXELFORMAT));      break;
	case DXGI_FORMAT_B8G8R8A8_UNORM:        memcpy(&header->ddspf, &DDSPF_A8R8G8B8, sizeof(DDS_PIXELFORMAT));    break;
	case DXGI_FORMAT_B8G8R8X8_UNORM:        memcpy(&header->ddspf, &DDSPF_X8R8G8B8, sizeof(DDS_PIXELFORMAT));    break;
	case DXGI_FORMAT_YUY2:                  memcpy(&header->ddspf, &DDSPF_YUY2, sizeof(DDS_PIXELFORMAT));        break;
	case DXGI_FORMAT_B4G4R4A4_UNORM:        memcpy(&header->ddspf, &DDSPF_A4R4G4B4, sizeof(DDS_PIXELFORMAT));    break;

		// Legacy D3DX formats using D3DFMT enum value as FourCC
	case DXGI_FORMAT_R32G32B32A32_FLOAT:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 116; break; // D3DFMT_A32B32G32R32F
	case DXGI_FORMAT_R16G16B16A16_FLOAT:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 113; break; // D3DFMT_A16B16G16R16F
	case DXGI_FORMAT_R16G16B16A16_UNORM:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 36;  break; // D3DFMT_A16B16G16R16
	case DXGI_FORMAT_R16G16B16A16_SNORM:    header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 110; break; // D3DFMT_Q16W16V16U16
	case DXGI_FORMAT_R32G32_FLOAT:          header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 115; break; // D3DFMT_G32R32F
	case DXGI_FORMAT_R16G16_FLOAT:          header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 112; break; // D3DFMT_G16R16F
	case DXGI_FORMAT_R32_FLOAT:             header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 114; break; // D3DFMT_R32F
	case DXGI_FORMAT_R16_FLOAT:             header->ddspf.size = sizeof(DDS_PIXELFORMAT); header->ddspf.flags = DDS_FOURCC; header->ddspf.fourCC = 111; break; // D3DFMT_R16F

	case DXGI_FORMAT_AI44:
	case DXGI_FORMAT_IA44:
	case DXGI_FORMAT_P8:
	case DXGI_FORMAT_A8P8:
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

	default:
		memcpy(&header->ddspf, &DDSPF_DX10, sizeof(DDS_PIXELFORMAT));

		headerSize += sizeof(DDS_HEADER_DXT10);
		extHeader = reinterpret_cast<DDS_HEADER_DXT10*>(fileHeader + sizeof(uint32_t) + sizeof(DDS_HEADER));
		extHeader->dxgiFormat = desc.Format;
		extHeader->resourceDimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		extHeader->arraySize = 1;
		break;
	}

	size_t rowPitch, slicePitch, rowCount;
	hr = GetSurfaceInfo(static_cast<size_t>(desc.Width), desc.Height, desc.Format, &slicePitch, &rowPitch, &rowCount);
	if (FAILED(hr))
		return hr;

	if (rowPitch > UINT32_MAX || slicePitch > UINT32_MAX)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

	if (IsCompressed(desc.Format))
	{
		header->flags |= DDS_HEADER_FLAGS_LINEARSIZE;
		header->pitchOrLinearSize = static_cast<uint32_t>(slicePitch);
	}
	else
	{
		header->flags |= DDS_HEADER_FLAGS_PITCH;
		header->pitchOrLinearSize = static_cast<uint32_t>(rowPitch);
	}

	// Setup pixels
	std::unique_ptr<uint8_t[]> pixels(new (std::nothrow) uint8_t[slicePitch]);
	if (!pixels)
		return E_OUTOFMEMORY;

	assert(fpRowCount == rowCount);
	assert(fpRowPitch == rowPitch);

	const UINT64 imageSize = dstRowPitch * UINT64(rowCount);
	if (imageSize > UINT32_MAX)
		return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

	void* pMappedMemory = nullptr;
	D3D12_RANGE readRange = { 0, static_cast<SIZE_T>(imageSize) };
	D3D12_RANGE writeRange = { 0, 0 };
	hr = pStaging->Map(0, &readRange, &pMappedMemory);
	if (FAILED(hr))
		return hr;

	auto sptr = static_cast<const uint8_t*>(pMappedMemory);
	if (!sptr)
	{
		pStaging->Unmap(0, &writeRange);
		return E_POINTER;
	}

	uint8_t* dptr = pixels.get();

	const size_t msize = std::min<size_t>(rowPitch, size_t(dstRowPitch));
	for (size_t h = 0; h < rowCount; ++h)
	{
		memcpy(dptr, sptr, msize);
		sptr += dstRowPitch;
		dptr += rowPitch;
	}

	pStaging->Unmap(0, &writeRange);

	// Write header & pixels
	DWORD bytesWritten;
	if (!WriteFile(hFile.get(), fileHeader, static_cast<DWORD>(headerSize), &bytesWritten, nullptr))
		return HRESULT_FROM_WIN32(GetLastError());

	if (bytesWritten != headerSize)
		return E_FAIL;

	if (!WriteFile(hFile.get(), pixels.get(), static_cast<DWORD>(slicePitch), &bytesWritten, nullptr))
		return HRESULT_FROM_WIN32(GetLastError());

	if (bytesWritten != slicePitch)
		return E_FAIL;

	delonfail.clear();


	return S_OK;
}

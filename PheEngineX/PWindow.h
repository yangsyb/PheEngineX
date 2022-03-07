#pragma once
#include "pch.h"
namespace Phe
{
	class PWindow
	{
	public:
		static PWindow* Create(const std::string& Name, UINT32 Width, UINT32 Height);
		static void DestroyWindow(PWindow* Window);

		virtual bool Run() = 0;

	protected:
		PWindow(UINT32 Width, UINT32 Height);
		virtual ~PWindow();

	protected:
		UINT32 PWidth;
		UINT32 PHeight;

	};

#ifdef PlatformWin32
	class PWindowWin32 : public PWindow
	{
	public:
		PWindowWin32(UINT32 Width, UINT32 Height, void* Handle, const std::string& Name);
		virtual ~PWindowWin32();
		virtual bool Run();

		void GenerateWindow(const std::string& Name);

	protected:
		HWND HWnd;
	};


#elif defined(PlatformIOS)
	class PWindowIOS : public PWindow
	{};


#elif defined (PlatformAndroid)
	class PWindowAndroid : public PWindow
	{};
#endif
}
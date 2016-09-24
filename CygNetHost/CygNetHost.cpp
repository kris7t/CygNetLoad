// CygNetHost.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "ClrHost.h"
#include "InitializeCygwin.h"

namespace cygnet
{
	static constexpr LPCWSTR kAppDomainFriendlyName = L"CygNetHostedAppDomain";

	class CoInitializeScope
	{
	public:
		explicit CoInitializeScope(DWORD coInit)
		{
			auto hr = CoInitializeEx(nullptr, coInit);
			if (FAILED(hr))
				throw _com_error(hr);
		}

		~CoInitializeScope() noexcept
		{
			CoUninitialize();
		}
	};

#define CYGNET_LOAD_ERROR "CygNetHost Error"

	static void ShowErrorA(const char *error) noexcept
	{
		MessageBoxA(nullptr, error, CYGNET_LOAD_ERROR, MB_ICONERROR | MB_OK);
	}

	static void ShowError(const TCHAR *error) noexcept
	{
		MessageBox(nullptr, error, _T(CYGNET_LOAD_ERROR), MB_ICONERROR | MB_OK);
	}

#undef CYGNET_LOAD_ERROR
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	try
	{
		cygnet::InitializeCygwin();

		try
		{
			cygnet::CoInitializeScope co_initialize_scope(COINIT_APARTMENTTHREADED);

			cygnet::ClrHost clr_host;
			clr_host.Start();

			return clr_host.RunCommandLine(lpCmdLine, cygnet::kAppDomainFriendlyName);
		}
		catch (const _com_error &e)
		{
			auto error_message = e.ErrorMessage();
			cygnet::ShowError(error_message);
			LocalFree(HLOCAL(error_message));
			return EXIT_FAILURE;
		}
	}
	catch (const std::exception &e)
	{
		cygnet::ShowErrorA(e.what());
		return EXIT_FAILURE;
	}
}

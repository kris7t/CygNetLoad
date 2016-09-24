#include "stdafx.h"

#include "ClrHost.h"

#import "CygNet.Manager.tlb" no_smart_pointers raw_interfaces_only rename_namespace("cygnet")

namespace cygnet
{
	static constexpr LPCWSTR kRuntimeVersion = L"v4.0.30319";
	static constexpr LPCWSTR kAppDomainManagerAssembly = L"CygNet.Manager";
	static constexpr LPCWSTR kAppDomainManagerType = L"CygNet.Manager.CygNetAppDomainManager";
	static constexpr DWORD kDefaultAppDomainId = 1;

	template <typename T>
	static release_ptr<T> make_release_ptr(T *ptr)
	{
		return release_ptr<T>(ptr);
	}

	static void CheckComError(HRESULT hr)
	{
		if (FAILED(hr)) throw _com_error(hr);
	}

	class CygNetHostControl : public IHostControl
	{
	public:
		virtual ~CygNetHostControl()
		{
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) override
		{
			if (ppvObject == nullptr) return E_INVALIDARG;

			if (riid == IID_IUnknown || riid == IID_IHostControl)
			{
				*ppvObject = this;
				AddRef();
				return NOERROR;
			}

			return E_NOINTERFACE;
		}

		ULONG STDMETHODCALLTYPE AddRef() override
		{
			InterlockedIncrement(&ref_count_);
			return ref_count_;
		}

		ULONG STDMETHODCALLTYPE Release() override
		{
			InterlockedDecrement(&ref_count_);
			if (ref_count_ == 0)
				delete this;
			return ref_count_;
		}

		HRESULT STDMETHODCALLTYPE GetHostManager(REFIID riid, void **ppObject) override
		{
			UNREFERENCED_PARAMETER(riid);

			*ppObject = nullptr;
			return E_NOINTERFACE;
		}
		
		HRESULT STDMETHODCALLTYPE SetAppDomainManager(
			DWORD dwAppDomainID,
			IUnknown *pUnkAppDomainManager) override
		{
			HRESULT hr = NOERROR;
			if (dwAppDomainID == kDefaultAppDomainId)
			{
				ICygNetManager *default_manager_raw;
				hr = pUnkAppDomainManager->QueryInterface(IID_PPV_ARGS(&default_manager_raw));
				if (SUCCEEDED(hr))
					default_manager_ = make_release_ptr(default_manager_raw);
			}
			return hr;
		}

		long RunCommandLine(BSTR command_line, BSTR friendly_name)
		{
			if (default_manager_ == nullptr)
				throw std::runtime_error("ICygNetManager was no loaded in the default AppDomain.");
			long ret_val;
			CheckComError(default_manager_->RunCommandLine(command_line, friendly_name, &ret_val));
			return ret_val;
		}

	private:
		ULONG ref_count_ = 1;
		release_ptr<ICygNetManager> default_manager_;
	};

	static auto MakeMetaHost()
	{
		ICLRMetaHost *meta_host_raw;
		CheckComError(CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&meta_host_raw)));
		return make_release_ptr(meta_host_raw);
	}

	static auto MakeRuntimeHost(ICLRMetaHost *meta_host)
	{
		ICLRRuntimeInfo *runtime_info_raw;
		CheckComError(meta_host->GetRuntime(kRuntimeVersion, IID_PPV_ARGS(&runtime_info_raw)));
		auto runtime_info = make_release_ptr(runtime_info_raw);

		ICLRRuntimeHost *runtime_host_raw;
		CheckComError(runtime_info->GetInterface(CLSID_CLRRuntimeHost, IID_PPV_ARGS(&runtime_host_raw)));
		return make_release_ptr(runtime_host_raw);
	}

	static void SetAppDomainManagerType(ICLRRuntimeHost *runtime_host, LPCWSTR assembly, LPCWSTR type)
	{
		ICLRControl *clr_control_raw;
		CheckComError(runtime_host->GetCLRControl(&clr_control_raw));
		auto clr_control = make_release_ptr(clr_control_raw);

		CheckComError(clr_control->SetAppDomainManagerType(assembly, type));
	}

	ClrHost::ClrHost()
	{
		meta_host_ = MakeMetaHost();
		runtime_host_ = MakeRuntimeHost(meta_host_.get());
		host_control_ = make_release_ptr(new CygNetHostControl);
		CheckComError(runtime_host_->SetHostControl(host_control_.get()));
		SetAppDomainManagerType(runtime_host_.get(), kAppDomainManagerAssembly, kAppDomainManagerType);
	}

	ClrHost::~ClrHost()
	{
		if (runtime_host_ != nullptr)
			runtime_host_->Stop();
	}

	void ClrHost::Start() const
	{
		CheckComError(runtime_host_->Start());
	}

	long ClrHost::RunCommandLine(LPCWSTR command_line, LPCWSTR friendly_name) const
	{
		auto command_line_bstr = _bstr_t{ command_line };
		auto friendly_name_bstr = _bstr_t{ friendly_name };
		return host_control_->RunCommandLine(command_line_bstr, friendly_name_bstr);
	}
}
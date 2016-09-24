#pragma once

#include "stdafx.h"

namespace cygnet
{
	struct ReleaseIUnknown
	{
		void operator()(IUnknown *unknown) const
		{
			if (unknown != nullptr)
				unknown->Release();
		}
	};

	template <typename T>
	using release_ptr = std::unique_ptr<T, ReleaseIUnknown>;

	class CygNetHostControl;

	class ClrHost
	{
	public:
		ClrHost();
		~ClrHost();

		void Start() const;
		long RunCommandLine(LPCWSTR command_line, LPCWSTR friendly_name) const;

	private:
		release_ptr<ICLRMetaHost> meta_host_;
		release_ptr<ICLRRuntimeHost> runtime_host_;
		release_ptr<CygNetHostControl> host_control_;
	};
}
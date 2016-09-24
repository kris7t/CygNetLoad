#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cygnet
{
	class CygtlsStackGuard
	{
	public:
		static void Check();

		CygtlsStackGuard() noexcept;
		CygtlsStackGuard(const CygtlsStackGuard &) = delete;
		CygtlsStackGuard(CygtlsStackGuard &&) = delete;
		CygtlsStackGuard &operator=(const CygtlsStackGuard &) = delete;
		CygtlsStackGuard &operator=(CygtlsStackGuard &&) = delete;
		~CygtlsStackGuard() noexcept;

	private:
		static constexpr size_t kPadSize = 12800;
		static const CygtlsStackGuard *gInstance;

		DWORD guarded_thread_id_;
		char *stack_base_;
		bool backup_valid_;
		ptrdiff_t backup_size_;
		// CYGTLS_PADSIZE with amd64 stack alignment requirements.
		// Must come at the end of the class, towards the base (high address) of the stack.
		char buffer_[kPadSize];

		void Backup() noexcept;
		void CheckInstance() const;
	};
}

#include "stdafx.h"
#include "CygtlsStackGuard.h"

namespace cygnet
{
	const CygtlsStackGuard *CygtlsStackGuard::gInstance = nullptr;

	void CygtlsStackGuard::Check()
	{
		if (gInstance == nullptr)
		{
			throw std::runtime_error("Stack guard was not initialized.");
		}
		gInstance->CheckInstance();
	}

	CygtlsStackGuard::CygtlsStackGuard() noexcept : backup_valid_(0), backup_size_(0)
	{
		auto tib = reinterpret_cast<PNT_TIB>(NtCurrentTeb());
		stack_base_ = static_cast<char *>(tib->StackBase);
		guarded_thread_id_ = GetCurrentThreadId();
		Backup();
		gInstance = this;
	}

	CygtlsStackGuard::~CygtlsStackGuard() noexcept
	{
		if (!backup_valid_) return;
		auto high_address_of_our_stack = buffer_ + kPadSize;
		memcpy(stack_base_, high_address_of_our_stack, backup_size_);
	}

	void CygtlsStackGuard::Backup() noexcept
	{
		auto high_address_of_our_stack = buffer_ + kPadSize;
		// This many bytes will get overwritten by Cygwin between the stack pointer
		// which we got at the start of the program and the real stack base address.
		backup_size_ = stack_base_ - high_address_of_our_stack;
		if (backup_size_ > kPadSize || backup_size_ < 0)
		{
			backup_valid_ = false;
			return;
		}
		memcpy(buffer_, high_address_of_our_stack, backup_size_);
		// Fill space reserved to CYGTLS with a bit pattern for easier debugging.
		memset(buffer_ + backup_size_, 0xff, kPadSize);
		backup_valid_ = true;
	}

	void CygtlsStackGuard::CheckInstance() const
	{
		auto current_thread_id = GetCurrentThreadId();
		if (!backup_valid_)
		{
			char error[255];
			sprintf_s(error, "Failed to backup %Id bytes of used stack, only %Iu bytes were reserved.",
				backup_size_, kPadSize);
			throw std::runtime_error(error);
		}
		else if (guarded_thread_id_ != current_thread_id)
		{
			char error[255];
			sprintf_s(error, "Attempted to load Cygwin on thread %I32u, but the stack guard was created for thread %I32u",
				guarded_thread_id_, current_thread_id);
			throw std::runtime_error(error);
		}
	}
}
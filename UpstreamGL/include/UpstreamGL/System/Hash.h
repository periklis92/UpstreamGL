#pragma once

#include <cstdint>
#include <cstddef>

struct Hash
{
	static uint64_t FNV1(const char* str, std::size_t size)
	{
		static uint64_t m_FNVPrime = 1099511628211ull;
		static uint64_t m_FNVOffset = 14695981039346656037ull;
		uint64_t hash = m_FNVOffset;
		for (uint64_t i = 0; i < size; ++i)
		{
			hash *= m_FNVPrime;
			hash ^= str[i];
		}
		return hash;
	}

};

constexpr uint64_t operator ""_h(const char* str, std::size_t size)
{
	uint64_t m_FNVPrime = 1099511628211ull;
	uint64_t m_FNVOffset = 14695981039346656037ull;
	uint64_t hash = m_FNVOffset;
	for (uint64_t i = 0; i < size; ++i)
	{
		hash *= m_FNVPrime;
		hash ^= str[i];
	}
	return hash;
}
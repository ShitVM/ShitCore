#pragma once

#include <string>

#if __has_include(<filesystem>)
#	include <filesystem>
#elif __has_include(<experimental/filesystem>)
#	include <experimental/filesystem>
#else
#	error Failed to include <filesystem>
#endif

namespace svm::detail {
	namespace fs =
#if __has_include(<filesystem>)
		std::filesystem
#elif __has_include(<experimental/filesystem>)
		std::experimental::filesystem
#endif
		;

	std::string GetAbsolutePath(const std::string& path);
	std::string GetRelativePath(const std::string& path);
}
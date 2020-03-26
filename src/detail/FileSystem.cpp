#include <svm/detail/FileSystem.hpp>

namespace svm::detail {
	std::string GetAbsolutePath(const std::string& path) {
		return fs::absolute(path).generic_u8string();
	}
}
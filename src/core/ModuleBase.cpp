#include <svm/core/ModuleBase.hpp>

namespace svm::core {
	std::ostream& operator<<(std::ostream& stream, const ModulePath& path) {
		if (std::holds_alternative<std::filesystem::path>(path)) {
			return stream << std::get<std::filesystem::path>(path).generic_string();
		} else {
			return stream << std::get<std::string>(path);
		}
	}
}
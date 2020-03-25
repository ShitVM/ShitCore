#include <svm/core/Version.hpp>

namespace svm::core {
	const char* Version = SVMCORE_VERSION;
	unsigned int VersionMajor = SVMCORE_VERSION_MAJOR;
	unsigned int VersionMinor = SVMCORE_VERSION_MINOR;
	unsigned int VersionPatch = SVMCORE_VERSION_PATCH;
	const char* VersionIdentifier = SVMCORE_VERSION_IDENTIFIER;
	const char* VersionTag = SVMCORE_VERSION_TAG;
}
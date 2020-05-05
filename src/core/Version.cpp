#include <svm/core/Version.hpp>

#include <climits>
#include <cstddef>
#include <cstdint>

static_assert(CHAR_BIT == 8);
static_assert(NULL == 0);
static_assert(sizeof(void*) >= 4);
static_assert(sizeof(void*) == sizeof(std::size_t));

namespace svm::core {
	const char* Version = SVMCORE_VERSION;
	unsigned int VersionMajor = SVMCORE_VERSION_MAJOR;
	unsigned int VersionMinor = SVMCORE_VERSION_MINOR;
	unsigned int VersionPatch = SVMCORE_VERSION_PATCH;
	const char* VersionIdentifier = SVMCORE_VERSION_IDENTIFIER;
	const char* VersionTag = SVMCORE_VERSION_TAG;
}
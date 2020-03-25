#pragma once

#define SVMCORE_VERSION "0.4.0"
#define SVMCORE_VERSION_MAJOR 0
#define SVMCORE_VERSION_MINOR 4
#define SVMCORE_VERSION_PATCH 0
#define SVMCORE_VERSION_IDENTIFIER ""
#define SVMCORE_VERSION_TAG ""

namespace svm::core {
	extern const char* Version;
	extern unsigned int VersionMajor;
	extern unsigned int VersionMinor;
	extern unsigned int VersionPatch;
	extern const char* VersionIdentifier;
	extern const char* VersionTag;
}
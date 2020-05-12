#pragma once

#include <svm/Structure.hpp>
#include <svm/core/ModuleBase.hpp>
#include <svm/core/virtual/VirtualFunction.hpp>

namespace svm::core {
	template<typename FI>
	using VirtualModule = ModuleBase<VirtualFunction<FI>>;
}
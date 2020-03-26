#pragma once
#include <svm/core/Module.hpp>

#include <cassert>
#include <utility>

namespace svm::core {
	template<typename FI>
	ModuleInfo<FI>::ModuleInfo(ByteFile&& byteFile) noexcept
		: Module(std::move(byteFile)) {}
	template<typename FI>
	ModuleInfo<FI>::ModuleInfo(VirtualModule<FI>&& virtualModule) noexcept
		: Module(std::move(virtualModule)) {}
	template<typename FI>
	ModuleInfo<FI>::ModuleInfo(ModuleInfo&& moduleInfo) noexcept
		: Module(std::move(moduleInfo.Module)) {}

	template<typename FI>
	ModuleInfo<FI>& ModuleInfo<FI>::operator=(ModuleInfo&& moduleInfo) noexcept {
		Module = std::move(moduleInfo.Module);
		return *this;
	}

	template<typename FI>
	bool ModuleInfo<FI>::IsEmpty() const noexcept {
		return std::holds_alternative<std::monostate>(Module);
	}
	template<typename FI>
	bool ModuleInfo<FI>::IsByteFile() const noexcept {
		return std::holds_alternative<ByteFile>(Module);
	}
	template<typename FI>
	bool ModuleInfo<FI>::IsVirtualModule() const noexcept {
		return std::holds_alternative<VirtualModule<FI>>(Module);
	}

	template<typename FI>
	std::string_view ModuleInfo<FI>::GetPath() const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return std::get<ByteFile>(Module).GetPath();
		else return std::get<VirtualModule<FI>>(Module).GetPath();
	}
	template<typename FI>
	Structure ModuleInfo<FI>::GetStructure(std::uint32_t index) const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return std::get<ByteFile>(Module).GetStructures()[index];
		else return std::get<VirtualModule<FI>>(Module).GetStructures()[index];
	}
	template<typename FI>
	std::uint32_t ModuleInfo<FI>::GetStructureCount() const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return static_cast<std::uint32_t>(std::get<ByteFile>(Module).GetStructures().size());
		else return static_cast<std::uint32_t>(std::get<VirtualModule<FI>>(Module).GetStructures().size());
	}
	template<typename FI>
	std::variant<Function, VirtualFunction<FI>> ModuleInfo<FI>::GetFunction(std::uint32_t index) const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return std::get<ByteFile>(Module).GetFunctions()[index];
		else return std::get<VirtualModule<FI>>(Module).GetFunctions()[index];
	}
	template<typename FI>
	std::uint32_t ModuleInfo<FI>::GetFunctionCount() const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return static_cast<std::uint32_t>(std::get<ByteFile>(Module).GetFunctions().size());
		else return static_cast<std::uint32_t>(std::get<VirtualModule<FI>>(Module).GetFunctions().size());
	}

	template<typename FI>
	void ModuleInfo<FI>::UpdateStructureCodes(std::uint32_t offset) noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return std::get<ByteFile>(Module).UpdateStructureCodes(offset);
		else return std::get<VirtualModule<FI>>(Module).UpdateStructureCodes(offset);
	}
}
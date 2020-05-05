#pragma once
#include <svm/core/Module.hpp>

#include <algorithm>
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
	Structure ModuleInfo<FI>::GetStructure(const std::string& name) const noexcept {
		assert(!IsEmpty());

		const Structures* structures = IsByteFile() ? &std::get<ByteFile>(Module).GetStructures()
													: &std::get<VirtualModule<FI>>(Module).GetStructures();
		const auto iter = std::find_if(structures->begin(), structures->end(), [&name](const auto& structure) {
			return name == structure.Name;
		});
		assert(iter != structures->end());
		return *iter;
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
	std::variant<Function, VirtualFunction<FI>> ModuleInfo<FI>::GetFunction(const std::string& name) const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) {
			const Functions& functions = std::get<ByteFile>(Module).GetFunctions();
			const auto iter = std::find_if(functions.begin(), functions.end(), [&name](const auto& function) {
				return name == function.Name;
			});
			assert(iter != functions.end());
			return *iter;
		} else {
			const VirtualFunctions<FI>& functions = std::get<VirtualModule<FI>>(Module).GetFunctions();
			const auto iter = std::find_if(functions.begin(), functions.end(), [&name](const auto& function) {
				return name == function.GetName();
			});
			assert(iter != functions.end());
			return *iter;
		}
	}
	template<typename FI>
	std::uint32_t ModuleInfo<FI>::GetFunctionCount() const noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return static_cast<std::uint32_t>(std::get<ByteFile>(Module).GetFunctions().size());
		else return static_cast<std::uint32_t>(std::get<VirtualModule<FI>>(Module).GetFunctions().size());
	}

	template<typename FI>
	void ModuleInfo<FI>::UpdateStructureInfos(std::uint32_t module) noexcept {
		assert(!IsEmpty());

		if (IsByteFile()) return std::get<ByteFile>(Module).UpdateStructureInfos(module);
		else return std::get<VirtualModule<FI>>(Module).UpdateStructureInfos(module);
	}
}
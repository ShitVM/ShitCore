#pragma once

#include <svm/Structure.hpp>
#include <svm/core/ByteFile.hpp>
#include <svm/core/virtual/VirtualModule.hpp>
#include <svm/detail/ReferenceWrapper.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace svm::core {
	template<typename FI>
	class ModuleInfo final {
	public:
		std::variant<std::monostate, ByteFile, VirtualModule<FI>> Module;

	public:
		ModuleInfo() noexcept = default;
		ModuleInfo(ByteFile&& byteFile) noexcept;
		ModuleInfo(VirtualModule<FI>&& virtualModule) noexcept;
		ModuleInfo(ModuleInfo&& moduleInfo) noexcept;
		~ModuleInfo() = default;

	public:
		ModuleInfo& operator=(ModuleInfo&& moduleInfo) noexcept;
		bool operator==(const ModuleInfo&) = delete;
		bool operator!=(const ModuleInfo&) = delete;

	public:
		bool IsEmpty() const noexcept;
		bool IsByteFile() const noexcept;
		bool IsVirtualModule() const noexcept;

		std::string_view GetPath() const noexcept;
		const std::vector<std::string>& GetDependencies() const noexcept;
		Structure GetStructure(std::uint32_t index) const noexcept;
		Structure GetStructure(const std::string& name) const noexcept;
		StructureInfo& GetStructure(std::uint32_t index) noexcept;
		std::uint32_t GetStructureCount() const noexcept;
		std::variant<Function, VirtualFunction<FI>> GetFunction(std::uint32_t index) const noexcept;
		std::variant<Function, VirtualFunction<FI>> GetFunction(const std::string& name) const noexcept;
		std::uint32_t GetFunctionCount() const noexcept;
		const Mappings& GetMappings() const noexcept;

		void UpdateStructureInfos(std::uint32_t module) noexcept;
	};
}

namespace svm::core {
	template<typename FI>
	class Module final : public detail::ReferenceWrapper<ModuleInfo<FI>> {
	public:
		using detail::ReferenceWrapper<ModuleInfo<FI>>::ReferenceWrapper;
	};

	template<typename FI>
	using Modules = std::vector<std::unique_ptr<ModuleInfo<FI>>>;
}

#include "detail/impl/Module.hpp"
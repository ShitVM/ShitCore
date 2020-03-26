#pragma once
#include <svm/core/Loader.hpp>

#include <svm/core/Parser.hpp>
#include <svm/detail/FileSystem.hpp>

#include <algorithm>

namespace svm::core {
	template<typename FI>
	Loader<FI>::Loader(Loader&& loader) noexcept
		: m_Modules(std::move(loader.m_Modules)), m_StructureOffset(loader.m_StructureOffset) {}

	template<typename FI>
	Loader<FI>& Loader<FI>::operator=(Loader&& loader) noexcept {
		m_Modules = std::move(loader.m_Modules);
		m_StructureOffset = loader.m_StructureOffset;
		return *this;
	}

	template<typename FI>
	void Loader<FI>::Clear() noexcept {
		m_Modules.clear();
		m_StructureOffset = 0;
	}

	template<typename FI>
	Module<FI> Loader<FI>::Load(const std::string& path) {
		Parser parser;
		parser.Open(path);
		parser.Parse();

		ByteFile byteFile = parser.GetResult();
		byteFile.UpdateStructureCodes(m_StructureOffset);
		m_StructureOffset += static_cast<std::uint32_t>(byteFile.GetStructures().size());

		return m_Modules.emplace_back(std::move(byteFile));
	}
	template<typename FI>
	VirtualModule<FI>& Loader<FI>::Create(const std::string& virtualPath) {
		ModuleInfo<FI>& module = m_Modules.emplace_back(VirtualModule<FI>(detail::GetAbsolutePath(virtualPath)));
		return std::get<VirtualModule<FI>>(module.Module);
	}
	template<typename FI>
	void Loader<FI>::LoadDependencies(Module<FI> module) {
		if (!std::holds_alternative<ByteFile>(module->Module)) return;

		const ByteFile& byteFile = std::get<ByteFile>(module->Module);
		const std::vector<std::string>& dependencies = byteFile.GetDependencies();

		for (const std::string& dependency : dependencies) {
			if (GetModule(dependency) != nullptr) continue;

			LoadDependencies(Load(dependency));
		}
	}

	template<typename FI>
	Module<FI> Loader<FI>::GetModule(std::uint32_t index) const noexcept {
		return m_Modules[index];
	}
	template<typename FI>
	Module<FI> Loader<FI>::GetModule(const std::string& path) const noexcept {
		const auto iter = std::find_if(m_Modules.begin(), m_Modules.end(), [path](const auto& module) {
			return module.GetPath() == path;
		});
		if (iter == m_Modules.end()) return nullptr;
		else return *iter;
	}
	template<typename FI>
	std::uint32_t Loader<FI>::GetModuleCount() const noexcept {
		return static_cast<std::uint32_t>(m_Modules.size());
	}
}
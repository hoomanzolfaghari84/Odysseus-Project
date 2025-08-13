#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <optional>
#include <sstream>
#include <filesystem>
#include <iostream>
namespace Odysseus2D
{
    class ConfigManager {
        public:
        
        static ConfigManager& GetConfig();

        const nlohmann::json& GetJson() const {return m_Config; }
        
        bool Load(const std::string& path);
        bool Save(const std::string& path = "");
        
        template <typename T>
        std::optional<T> Get(const std::string& key) const;
        
        template <typename T>
        T GetOr(const std::string& key, const T& defaultValue) const;
        
        template <typename T>
        void Set(const std::string& key, const T& value);
        
        bool Has(const std::string& key) const;
        
        private:
        nlohmann::json m_Config;
        std::string m_CurrentPath;
        
        // Private constructor to prevent external instantiation
        ConfigManager() = default;
        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;
        
        nlohmann::json* Traverse(const std::string& key, bool createIfMissing = false);
        const nlohmann::json* Traverse(const std::string& key) const;
    };
    
} // namespace Odysseus2D
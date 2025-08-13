#include "ConfigManager.hpp"

namespace Odysseus2D
{

    ConfigManager& ConfigManager::GetConfig() {
        static ConfigManager instance;
        return instance;
    }
    
    bool ConfigManager::Load(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "[Config] Failed to open: " << path << '\n';
            return false;
        }
        
        try {
            file >> m_Config;
            m_CurrentPath = path;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "[Config] JSON parse error: " << e.what() << '\n';
        return false;
    }
}

bool ConfigManager::Save(const std::string& path) {
    std::string savePath = path.empty() ? m_CurrentPath : path;
    
    std::ofstream file(savePath);
    if (!file.is_open()) {
        std::cerr << "[Config] Failed to write: " << savePath << '\n';
        return false;
    }
    
    file << m_Config.dump(4); // Pretty print
    return true;
}

nlohmann::json* ConfigManager::Traverse(const std::string& key, bool createIfMissing) {
    std::istringstream iss(key);
    std::string token;
    nlohmann::json* node = &m_Config;
    
    while (std::getline(iss, token, '.')) {
        if (node->contains(token)) {
            node = &(*node)[token];
        } else if (createIfMissing) {
            node = &((*node)[token] = nlohmann::json{});
        } else {
            return nullptr;
        }
    }
    
    return node;
}

const nlohmann::json* ConfigManager::Traverse(const std::string& key) const {
    std::istringstream iss(key);
    std::string token;
    const nlohmann::json* node = &m_Config;
    
    while (std::getline(iss, token, '.')) {
        if (node->contains(token)) {
            node = &(*node)[token];
        } else {
            return nullptr;
        }
    }
    
    return node;
}

bool ConfigManager::Has(const std::string& key) const {
    return Traverse(key) != nullptr;
}

template <typename T>
std::optional<T> ConfigManager::Get(const std::string& key) const {
    const nlohmann::json* node = Traverse(key);
    if (node && node->is_null() == false) {
        try {
            return node->get<T>();
        } catch (...) {}
    }
    return std::nullopt;
}

template <typename T>
T ConfigManager::GetOr(const std::string& key, const T& defaultValue) const {
    auto val = Get<T>(key);
    return val.has_value() ? val.value() : defaultValue;
}

template <typename T>
void ConfigManager::Set(const std::string& key, const T& value) {
    nlohmann::json* node = Traverse(key, true);
    if (node) {
        *node = value;
    }
}

// Explicit instantiations
template std::optional<int> ConfigManager::Get<int>(const std::string&) const;
template std::optional<float> ConfigManager::Get<float>(const std::string&) const;
template std::optional<bool> ConfigManager::Get<bool>(const std::string&) const;
template std::optional<std::string> ConfigManager::Get<std::string>(const std::string&) const;

template int ConfigManager::GetOr<int>(const std::string&, const int&) const;
template float ConfigManager::GetOr<float>(const std::string&, const float&) const;
template bool ConfigManager::GetOr<bool>(const std::string&, const bool&) const;
template std::string ConfigManager::GetOr<std::string>(const std::string&, const std::string&) const;

template void ConfigManager::Set<int>(const std::string&, const int&);
template void ConfigManager::Set<float>(const std::string&, const float&);
template void ConfigManager::Set<bool>(const std::string&, const bool&);
template void ConfigManager::Set<std::string>(const std::string&, const std::string&);

} // namespace Engine
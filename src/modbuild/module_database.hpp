#pragma once

#include "module_type.hpp"
#include "utils/hash.hpp"
#include <chrono>
#include <json/value.h>
#include <filesystem>
#include <shared_mutex>
#include <unordered_map>
#include <vector>


class ModuleDatabase {
public:

    struct Reference {
        ModuleType type;
        std::string name;
        constexpr bool operator==(const Reference &other) const  = default;

        constexpr std::size_t hash() const {
            std::hash<std::string> hasher;
            return hash_combine(hasher(name), static_cast<std::size_t>(type));
        }
    };

    struct State {
        bool rescan = false;            ///<this file needs to be rescanned              
        bool recompile = false;         ///<this file needs to be recompiled
    };

    struct Source {
        std::filesystem::path source_file;
        std::filesystem::path object_path;
        std::filesystem::path bmi_path;
        std::string name;
        ModuleType type;
        std::vector<Reference> references;
        State state;
    };

    using PSource = std::shared_ptr<Source>;

    using FileIndex = std::unordered_map<std::filesystem::path, PSource>;
    using ModuleIndex = std::unordered_map<Reference, std::vector<PSource>, MethodHash >;


    json::value export_db() const;
    void import_db(json::value db);
    void clear();

  
    PSource find(Reference ref) const;
    PSource find(std::filesystem::path file) const;
    ///Inserts src int database
    /**
     * @param src source info
     * @retval nullptr successfuly put
     * @retval valid_pointer conflict, returns pointer to existing item
     * 
     */
    PSource put(Source src);
    void erase(std::filesystem::path file);
    std::vector<PSource> get_all_imps(std::string module_name) const;
    bool is_dirty() const;
    void clear_dirty();

protected:
    FileIndex _fileIndex;
    ModuleIndex _moduleIndex;
    std::chrono::system_clock::time_point _modify_time; //time when database was modified
    std::chrono::system_clock::time_point _import_time;   //time when database was imported
    bool _modified;     //database has been modified
    mutable std::shared_mutex _mx;

    void reindex_references_lk();


};
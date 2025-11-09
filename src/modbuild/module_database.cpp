#include "module_database.hpp"
#include <mutex>

json::value ModuleDatabase::export_db() const {
    std::shared_lock _(_mx);
    json::value data(_fileIndex.begin(), _fileIndex.end(), [&](const auto &kv){
        const PSource src = kv.second;
        return json::value {
            {"source_file",src->source_file.u8string()},
            {"object_path",src->object_path.u8string()},
            {"bmi_path",src->bmi_path.u8string()},
            {"name",src->name},
            {"type",static_cast<int>(src->type)},
            {"references",json::value(src->references.begin(), src->references.end(),
                [&](const Reference &ref) {
                    return json::value{
                        {"type", static_cast<int>(ref.type)},
                        {"name",ref.name},
                    };
                }
            )}        
        };
    });

    return {
        {"timestamp", _import_time.time_since_epoch().count()},
        {"data", data}
    };
}

void ModuleDatabase::import_db(json::value db) {
    json::value data = db["data"];
    json::value tm = db["timestamp"];
    std::unique_lock _(_mx);
    for (auto item: data) {
        Source src;
        src.name = data["name"].as<std::string>();
        src.source_file = data["source_file"].as<std::u8string>();
        src.object_path = data["object_path"].as<std::u8string>();
        src.bmi_path = data["bmi_path"].as<std::u8string>();
        src.type = static_cast<ModuleType>(data.as<int>());
        auto ref = data["references"];
        std::transform(ref.begin(), ref.end(), std::back_inserter(src.references),
            [&](const json::value &val) {
                return Reference{
                    static_cast<ModuleType>(val["type"].as<int>()),
                    val["name"].as<std::string>()
                };
            });
        put(std::move(src));
    }
    _modify_time = std::chrono::system_clock::time_point(
        std::chrono::system_clock::duration(tm.as<std::chrono::system_clock::duration::rep>()));
    _import_time = std::chrono::system_clock::now();                    
}

void ModuleDatabase::clear() {
    std::unique_lock _(_mx);
    _fileIndex.clear();
    _moduleIndex.clear();
    _modify_time = {};
    _import_time = std::chrono::system_clock::now();
    _modified = false;
}

ModuleDatabase::PSource ModuleDatabase::find(Reference ref) const { 

    std::shared_lock _(_mx);
    auto iter = _moduleIndex.find(Reference{ref});
    if (iter == _moduleIndex.end()) return {};
    return iter->second.front();

}

ModuleDatabase::PSource ModuleDatabase::find(std::filesystem::path file) const {
      std::shared_lock _(_mx);
    auto iter = _fileIndex.find(file);
    if (iter == _fileIndex.end()) return {};
    return iter->second;

}

ModuleDatabase::PSource ModuleDatabase::put(Source src) {

  std::unique_lock _(_mx);
  auto iter = _fileIndex.find(src.source_file);
  if (iter != _fileIndex.end())
    return iter->second;;

  PSource psrc = std::make_unique<Source>(src);
  _fileIndex.emplace(psrc->source_file, psrc);
  auto refiter = _moduleIndex.try_emplace(Reference{src.type, src.name}, 1, psrc);
  if (!refiter.second) {
    refiter.first->second.push_back(std::move(psrc));
  }
  return {};
}

void ModuleDatabase::erase(std::filesystem::path file) {
    std::unique_lock _(_mx);
    auto it1 = _fileIndex.find(file);
    if (it1 == _fileIndex.end()) return;

    PSource src = it1->second;
    _fileIndex.erase(it1);
    auto it2 = _moduleIndex.find(Reference{src->type, src->name});
    if (it2 == _moduleIndex.end()) return;

    auto iter = std::remove(it2->second.begin(), it2->second.end(), src);
    it2->second.erase(iter, it2->second.end());
    if (it2->second.empty()) {
        _moduleIndex.erase(it2);
    }
    
}

std::vector<ModuleDatabase::PSource> ModuleDatabase::get_all_imps(std::string module_name) const {
    std::shared_lock _(_mx);
    auto iter = _moduleIndex.find(Reference{ModuleType::implementation, std::move(module_name)});
    if (iter == _moduleIndex.end()) return {};
    else return iter->second;
}

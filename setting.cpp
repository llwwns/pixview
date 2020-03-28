#include "setting.h"
#include <toml.hpp>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

const char* DIR_NAME = "pixview";
const char* FILE_NAME = "config.toml";
const char* DEFAULT_LIBRARY = "default";

Setting Setting::_default() {
    std::vector<std::string> paths;
    for (auto p: QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)) {
        paths.push_back(p.toStdString());
    }
    auto lib = LibraryPath {
        .paths = paths,
        .name = DEFAULT_LIBRARY,
    };
    return Setting {
        .fullscreen = false,
        .interval = 4000,
        .autoRotate = false,
        .defaultLibrary = DEFAULT_LIBRARY,
        .libraries = std::vector<LibraryPath>{lib},
    };
};

Setting Setting::load() {
    Setting s = Setting::_default();
    auto cfgs = QStandardPaths::locateAll(QStandardPaths::ConfigLocation, QString("%1/%2").arg(DIR_NAME, FILE_NAME), QStandardPaths::LocateFile);
    for (auto c: cfgs) {
        toml::value t;
        try {
            t = toml::parse(c.toStdString());
        } catch(...) {
            continue;
        }
        if (t.contains("fullscreen")) {
            s.fullscreen = t.at("fullscreen").as_boolean();
        }
        if (t.contains("interval")) {
            auto interval = t.at("interval").as_integer();
            if (interval > 0) {
                s.interval = interval;
            }
        }
        if (t.contains("auto_rotate")) {
            s.autoRotate = t.at("auto_rotate").as_boolean();
        }
        if (t.contains("libraries")) {
            std::vector<LibraryPath> libs;
            for (auto l: t.at("libraries").as_array()) {
                std::vector<std::string> paths;
                for (auto p: l.at("paths").as_array()) {
                    paths.push_back(p.as_string());
                }
                libs.push_back({
                    .paths = std::move(paths),
                    .name = l.at("name").as_string(),
                });
            }
            if (libs.size() > 0) {
                s.libraries = std::move(libs);
            }
        }
        s.defaultLibrary = t.at("default_library").as_string();
        return s;
    }
    return s;
}

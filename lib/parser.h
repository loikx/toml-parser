#pragma once

#include <filesystem>
#include <fstream>
#include <istream>
#include "./utils/utils.h"


namespace omfl {
    class ParserOMFL {
        bool valid_ = true;
        sections::Section* globalSection_ = new sections::Section("global");

        void writeToFile(std::ofstream& file, const sections::Section& section) const;

    public:
        [[nodiscard]] bool valid() const;
        [[nodiscard]] omflelement::OMFLElement& Get(const std::string& key) const;

        void SetValid(bool valid);

        explicit ParserOMFL() = default;

        void AddSection(const std::string& name);

        void AddVariable(variables::Variable* varName, const std::string& sectionName);

        void extract(const std::string& path) const;
    };

//    ParserOMFL& parse(const std::filesystem::path& path);
    ParserOMFL& parse(const std::string& data);
}
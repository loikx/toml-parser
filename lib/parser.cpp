#include <fstream>
#include "parser.h"

bool omfl::ParserOMFL::valid() const {
    return globalSection_->valid() && valid_;
}

omflelement::OMFLElement& omfl::ParserOMFL::Get(const std::string &key) const {
    return globalSection_->Get(key);
}

void omfl::ParserOMFL::AddSection(const std::string& name) {
    globalSection_->AddSection(GetFullWay(name));
}

void omfl::ParserOMFL::AddVariable(variables::Variable* variable, const std::string& sectionName) {
    globalSection_->AddVariable(sectionName, variable);
}

//omfl::ParserOMFL& omfl::parse(const std::filesystem::path& path) {
//    std::string fileText;
//    std::string tmp;
//    std::ifstream file(path);
//    while (file >> tmp) {
//        fileText += tmp;
//    }
//
//    return omfl::parse(fileText);
//}

void omfl::ParserOMFL::SetValid(bool valid) {
    valid_ = valid;
}

omfl::ParserOMFL& omfl::parse(const std::string& data) {
    std::istringstream iss(data);
    std::string tmp;
    std::string lastSection = "global";

    auto* mainElement = new omfl::ParserOMFL();

    while (std::getline(iss, tmp)) {
        tmp = variables::TrimAll(tmp);
        variables::DeleteComment(tmp);
        std::string key = variables::TrimAll(variables::GetKeyName(tmp));
        std::string value = variables::TrimAll(variables::GetValue(tmp));
        variables::DeleteComment(value);
        if (tmp.empty()) {
            continue;
        }
        if (variables::IsValidSection(tmp)) {
            lastSection = tmp.substr(1, tmp.size() - 2);
            mainElement->AddSection(lastSection);
        } else {
            try {
                auto varType = variables::VariableType(value);
                variables::Variable* variable = variables::CreateElement(varType, value, key);
                bool validKey = variables::IsValidKey(key);
                variable->setValid(validKey && variable->valid());
                mainElement->AddVariable(variable, lastSection);
                mainElement->SetValid(mainElement->valid() && variable->valid());
            } catch (const omflexceptions::InvalidFormat& ex) {
                mainElement->SetValid(false);
                break;
            }
        }
    }

    return *mainElement;
}

void omfl::ParserOMFL::writeToFile(std::ofstream& file, const sections::Section& section) const {
    file << "<" << section.getName() << ">\n";
    for (const auto& item: section.variables_) {
        file << "<" << item.second->getName() << ">";
        if (item.second->IsInt()) {
            file << item.second->AsInt();
        } else if (item.second->IsFloat()) {
            file << item.second->AsFloat();
        } else if (item.second->IsBool()) {
            file << item.second->AsBool();
        } else if (item.second->IsString()) {
            file << item.second->AsString();
        }
        file << "</" << item.second->getName() << ">\n";
    }

    for (const auto& item: section.sections_) {
        writeToFile(file, *item.second);
    }

    file << "</" << section.getName() << ">\n";
}


void omfl::ParserOMFL::extract(const std::string& path) const {
    std::ofstream file(path);
    writeToFile(file, *globalSection_);
    file.close();
}
#include "utils.h"

using namespace variables;
using namespace omflelement;
using namespace sections;

std::vector<std::string> GetFullWay(const std::string& name) {
    std::istringstream iss(name);
    std::vector<std::string> full_way;
    std::string token;
    while (std::getline(iss, token, '.')) {
        if (!token.empty())
            full_way.push_back(token);
    }
    return full_way;
}

void OMFLElement::setValid(bool value) {
    valid_ = value;
}

std::string OMFLElement::getName() const {
    return elementName_;
}

bool OMFLElement::IsVariable() const {
    return elementType_ == elementType::VARIABLE;
}

bool OMFLElement::IsSection() const {
    return elementType_ == elementType::SECTION;
}

OMFLElement& OMFLElement::Get(const std::string& name) {
    if (elementType_ == elementType::SECTION) {
        auto* tmp = dynamic_cast<Section*>(this);
        return tmp->Get(name);
    }

    throw omflexceptions::NotThisTypeVariable("Variable is not section");
}

bool OMFLElement::IsInt() {
    if (IsVariable()) {
        auto copy = *this;
        auto* tmp = dynamic_cast<variables::Variable*>(&copy);
        return tmp->IsInt();
    }

    throw omflexceptions::NotVariableException((char*)"Section was used like int variable");
}

bool OMFLElement::IsFloat() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->IsFloat();
    }

    throw omflexceptions::NotVariableException((char*)"Section was used like float variable");
}

bool OMFLElement::IsString() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->IsString();
    }

    throw omflexceptions::NotVariableException((char*)"Section was used like string variable");
}

bool OMFLElement::IsBool() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->IsBool();
    }

    throw omflexceptions::NotVariableException((char*)"Section was used like bool variable");
}

bool OMFLElement::IsArray() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->IsArray();
    }

    throw omflexceptions::NotVariableException((char*)"Sections was used like array variable");
}

int OMFLElement::AsInt() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->AsInt();
    }

    throw omflexceptions::NotVariableException((char*)"Sections was used like int variable");
}

float OMFLElement::AsFloat() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->AsFloat();
    }

    throw omflexceptions::NotVariableException((char*)"Sections was used like float variable");
}

std::string OMFLElement::AsString() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->AsString();
    }

    throw omflexceptions::NotVariableException((char*)"Sections was used like string variable");
}

bool OMFLElement::AsBool() {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp->AsBool();
    }

    throw omflexceptions::NotVariableException((char*)"Sections was used like bool variable");
}

int OMFLElement::AsIntOrDefault(int defaultValue) {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        try {
            int objectValue = tmp->AsInt();
            return objectValue;
        } catch (const std::exception& ex) {
            return defaultValue;
        }
    }

    return defaultValue;
}

float OMFLElement::AsFloatOrDefault(float defaultValue) {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        try {
            float objectValue = tmp->AsFloat();
            return objectValue;
        } catch (const std::exception& ex) {
            return defaultValue;
        }
    }

    return defaultValue;
}

std::string OMFLElement::AsStringOrDefault(const std::string& defaultValue) {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        try {
            std::string objectValue = tmp->AsString();
            return objectValue;
        } catch (const std::exception& ex) {
            return defaultValue;
        }
    }

    return defaultValue;
}

bool Variable::IsInt() {
    return variableType_ == variables::type::INT;
}

bool Variable::IsFloat() {
    return variableType_ == type::FLOAT;
}

bool Variable::IsString() {
    return variableType_ == type::STRING;
}

bool Variable::IsBool() {
    return variableType_ == type::BOOL;
}

bool Variable::IsArray() {
    return variableType_ == type::ARRAY;
}

int Variable::AsInt() {
    if (IsInt()) {
        auto& tmp = dynamic_cast<Int&>(*this);
        return tmp.GetValue();
    }

    throw omflexceptions::NotThisTypeVariable((char*)"Not int variable was used like int");
}

float Variable::AsFloat() {
    if (IsFloat()) {
        auto& tmp = dynamic_cast<Float&>(*this);
        return tmp.GetValue();
    }

    throw omflexceptions::NotThisTypeVariable((char*)"Not float variable was used like float");
}

std::string Variable::AsString() {
    if (IsString()) {
        auto& tmp = dynamic_cast<String&>(*this);
        return tmp.GetValue();
    }

    throw omflexceptions::NotThisTypeVariable((char*)"Not string variable was used like string");
}

bool Variable::AsBool() {
    if (IsBool()) {
        auto& tmp = dynamic_cast<Bool&>(*this);
        return tmp.GetValue();
    }

    throw omflexceptions::NotThisTypeVariable((char*)"Not bool variable was used like bool");
}

int Int::GetValue() const {
    return value_;
}

float Float::GetValue() const {
    return value_;
}

std::string String::GetValue() const {
    return value_;
}

bool Bool::GetValue() const {
    return value_;
}

Variable& OMFLElement::operator[](int index) {
    if (IsVariable()) {
        auto* tmp = dynamic_cast<Variable*>(this);
        return tmp[index];
    }

    throw omflexceptions::NotVariableException((char*)"Section was used like array variable");
}

Variable& Variable::operator[](int index) {
    if (IsArray()) {
        auto* tmp = dynamic_cast<Array*>(this);
        return (*tmp)[index];
    }

    throw omflexceptions::NotThisTypeVariable((char*)"Not array type was used like array");
}

Variable& Array::operator[](int index) {
    if (index >= array_.size()) {
        auto* tmp = new Variable("Not exists", type::INT);
        return *tmp;
    }
    return *array_[index];
}

OMFLElement& Section::Get(const std::string& name) {
    auto wayToElement = GetFullWay(name);
    int index = 0;
    auto* tmp = dynamic_cast<Section*>(this);
    while (index < wayToElement.size()) {
        for (auto& section: tmp->sections_) {
            if (section.second->getName() == wayToElement[index]) {
                tmp = dynamic_cast<Section*>(section.second);
                ++index;
                break;
            }
        }

        for (auto& variable: tmp->variables_) {
            if (variable.second->getName() == wayToElement[index]) {
                auto* d = dynamic_cast<Variable*>(variable.second);
                return *d;
            }
        }
    }

    return *tmp;
}

void Section::AddSection(const std::vector<std::string>& way, int index) {
    if (index >= way.size()) {
        return;
    }

    if (sections_.find(way[index]) != sections_.end()) {
        sections_[way[index]]->AddSection(way, ++index);
        return;
    }

    auto* newSection = new Section(way[index]);
    sections_.insert(std::make_pair(newSection->getName(), newSection));
    sections_[way[index]]->AddSection(way, ++index);
}

void Section::AddVariable(const std::string& sectionName, variables::Variable* variable) {
    auto way = GetFullWay(sectionName);
    int index = 0;
    Section* tmp = this;
    if (tmp->getName() == sectionName) {
        if (tmp->variables_.find(variable->getName()) != tmp->variables_.end()) {
            setValid(false);
        }
        tmp->variables_.insert(std::make_pair(variable->getName(), variable));
        ++index;
        return;
    }
    while (index < way.size()) {
        for (const auto& item: tmp->sections_) {
            if (item.first == way[index]) {
                tmp = dynamic_cast<Section*>(item.second);
                ++index;
                break;
            }
        }
    }
    tmp->variables_.insert(std::make_pair(variable->getName(), variable));
}


std::vector<std::string> variables::Array::ArraySplit(const std::string& data) {
    std::vector<std::string> arrayValues;
    uint32_t squareBrackets = 0;
    uint32_t doubleQuotes = 0;
    uint32_t lastEndValue = 1;

    for (uint64_t i = 1; i < data.size() - 1; ++i) {
        if (data[i] == '[' && doubleQuotes % 2 == 0) {
            ++squareBrackets;
        } else if (data[i] == '\"' && squareBrackets == 0) {
            ++doubleQuotes;
        } else if (data[i] == ']' && doubleQuotes % 2 == 0) {
            --squareBrackets;
        } else if (data[i] == ',' && doubleQuotes % 2 == 0 && squareBrackets == 0) {
            std::string value = data.substr(lastEndValue, i - lastEndValue);
            arrayValues.push_back(value);
            lastEndValue = i + 1;
        }
    }

    std::string value = data.substr(lastEndValue, data.size() - 1 - lastEndValue);
    arrayValues.push_back(value);

    return arrayValues;
}

std::string variables::TrimAll(const std::string& s) {
    std::string copy = s;
    copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), copy.end());
    return copy;
}

std::string variables::GetKeyName(std::string& data) {
    const auto position = data.find('=');
    if (position >= data.size())
        return "";
    return TrimAll(data.substr(0, position));
}

bool variables::IsValidSection(const std::string& data) {
    const auto isSectionValid = std::regex("(\\[[^.][a-zA-Z0-9-_.]+[^.]\\])");
    return std::regex_match(data, isSectionValid);
}

std::string variables::GetValue(std::string& data) {
    const auto position = data.find('=');
    if (position >= data.size())
        return "";
    return TrimAll(data.substr(position + 1));
}

bool variables::IsValidKey(const std::string& data) {
    return std::regex_match(data, std::regex("([0-9a-zA-Z-_]+)"));
}

bool variables::IsValidInt(const std::string& data) {
    const auto isIntValid = std::regex("((-|\\+)?[0-9]+)");
    return std::regex_match(data, isIntValid);
}

bool variables::IsValidFloat(const std::string& data) {
    const auto isFloatValid = std::regex("((-|\\+)?[0-9]+\\.[0-9]+)");
    return std::regex_match(data, isFloatValid);
}

bool variables::IsValidString(const std::string& data) {
    bool valid = true;
    for (int i = 1; i < (int)data.size() - 1; ++i) {
        if (data[i] == '\"')
            valid = false;
    }
    const auto isStringValid = std::regex("(\"(.)*\")");
    return valid && std::regex_match(data, isStringValid);
}

bool variables::IsValidBool(const std::string& data) {
    const auto isBoolValid = std::regex("((true)|(false))");
    return std::regex_match(data, isBoolValid);
}

bool variables::IsValidArray(const std::string& data) {
    const auto isArrayValid = std::regex("(\\[.*\\])");
    return std::regex_match(data, isArrayValid);
}

variables::type variables::VariableType(const std::string& data) {
    if (IsValidString(data))
        return variables::type::STRING;
    else if (IsValidInt(data))
        return variables::type::INT;
    else if (IsValidFloat(data))
        return variables::type::FLOAT;
    else if (IsValidBool(data))
        return variables::type::BOOL;
    else if (IsValidArray(data))
        return variables::type::ARRAY;

    throw omflexceptions::InvalidFormat((char*)"Invalid format of variable");
}

void variables::DeleteComment(std::string& data) {
    const auto position = data.find('#');
    data = data.substr(0, position);
    TrimAll(data);
}

variables::Variable* variables::CreateElement(type itemType, const std::string& data, const std::string& name) {
    bool validValue = true;
    if (itemType == variables::type::INT) {
        validValue = variables::IsValidInt(data);
        auto* intVar = new variables::Int(name, data, validValue);
        return intVar;
    } else if (itemType == variables::type::FLOAT) {
        validValue = variables::IsValidFloat(data);
        auto* floatVar = new variables::Float(name, data, validValue);
        return floatVar;
    } else if (itemType == variables::type::BOOL) {
        validValue = variables::IsValidBool(data);
        auto* boolVar = new variables::Bool(name, data, validValue);
        return boolVar;
    } else if (itemType == variables::type::STRING) {
        validValue = variables::IsValidString(data);
        auto* stringVar = new variables::String(name, data.substr(1, data.size() - 2), validValue);
        return stringVar;
    } else if (itemType == variables::type::ARRAY) {
        validValue = variables::IsValidArray(data);
        auto* arrayVar = new variables::Array(name, data, validValue);
        return arrayVar;
    }
}

Array::Array(std::string name, const std::string& value, bool valid)
                    : Variable(std::move(name), type::ARRAY) {
    if (valid) {
        auto arrayValues = ArraySplit(value);
        for (const auto& item: arrayValues) {
            if (item.empty()) {
                continue;
            }
            try {
                std::string tmp = variables::TrimAll(item);
                auto itemType = variables::VariableType(tmp);
                auto* element = variables::CreateElement(itemType, tmp, "");
                array_.push_back(element);
            } catch (const omflexceptions::InvalidFormat& ex) {
                valid = false;
                array_.clear();
                break;
            }
        }
    }

    setValid(valid);
}

#ifndef LAB6_UTILS_H
#define LAB6_UTILS_H

#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <vector>

std::vector<std::string> GetFullWay(const std::string& name);

namespace omflexceptions {
    class NotVariableException : public std::exception {
        char* message_;
    public:

        explicit NotVariableException(char* text) : message_(text) {}

        char* what() {
            return message_;
        }
    };

    class NotThisTypeVariable : public std::exception {
        char* message_;

    public:
        explicit NotThisTypeVariable(char* text) : message_(text) {}

        char* what() {
            return message_;
        }
    };

    class InvalidFormat : public std::exception {
        char* message_;

    public:
        explicit InvalidFormat(char* text) : message_(text) {}

        char* what() {
            return message_;
        }
    };
}

namespace variables {
    class Variable;
}

namespace omflelement {
    enum elementType {
        VARIABLE,
        SECTION
    };

    class OMFLElement {
    protected:
        elementType elementType_;
        std::string elementName_;
        bool valid_ = true;

    public:

        explicit OMFLElement(std::string name, elementType type) :
                elementName_(std::move(name)), elementType_(type) {}

        explicit OMFLElement(elementType type) : elementType_(type) {}

        [[nodiscard]] std::string getName() const;

        void setValid(bool value);

        bool valid() {
            return valid_;
        }

        [[nodiscard]] bool IsVariable() const;

        [[nodiscard]] bool IsSection() const;

        [[nodiscard]] virtual OMFLElement& Get(const std::string& name);

        [[nodiscard]] virtual bool IsInt();

        [[nodiscard]] virtual bool IsFloat();

        [[nodiscard]] virtual bool IsString();

        [[nodiscard]] virtual bool IsBool();

        [[nodiscard]] virtual bool IsArray();

        [[nodiscard]] virtual int AsInt();

        [[nodiscard]] virtual float AsFloat();

        [[nodiscard]] virtual std::string AsString();

        [[nodiscard]] virtual bool AsBool();

        [[nodiscard]] virtual int AsIntOrDefault(int defaultValue);

        [[nodiscard]] virtual float AsFloatOrDefault(float value);

        [[nodiscard]] virtual std::string AsStringOrDefault(const std::string& value);

        [[nodiscard]] virtual variables::Variable& operator[](int index);
    };
}

namespace variables {
    enum type {
        INT,
        FLOAT,
        STRING,
        BOOL,
        ARRAY
    };

    std::string TrimAll(const std::string &s);

    std::string GetKeyName(std::string& data);

    bool IsValidSection(const std::string& data);

    std::string GetValue(std::string& data);

    bool IsValidKey(const std::string& data);

    bool IsValidInt(const std::string& data);

    bool IsValidFloat(const std::string& data);

    bool IsValidString(const std::string& data);

    bool IsValidBool(const std::string& data);

    bool IsValidArray(const std::string& data);

    void DeleteComment(std::string& data);

    Variable* CreateElement(type itemType, const std::string& data, const std::string& name);

    variables::type VariableType(const std::string& data);

    class Variable : public omflelement::OMFLElement {
        type variableType_;

    public:
        explicit Variable(std::string name, type variableType) :
                omflelement::OMFLElement(std::move(name), omflelement::elementType::VARIABLE) {
            variableType_ = variableType;
        }
        [[nodiscard]] bool IsInt() final;

        [[nodiscard]] bool IsFloat() final;

        [[nodiscard]] bool IsString() final;

        [[nodiscard]] bool IsBool() final;

        [[nodiscard]] bool IsArray() final;

        [[nodiscard]] int AsInt() final;

        [[nodiscard]] float AsFloat() final;

        [[nodiscard]] std::string AsString() final;

        [[nodiscard]] bool AsBool() final;

        [[nodiscard]] Variable& operator[](int index) override;
    };

    class Int : public Variable {
        int value_;

    public:
        explicit Int(std::string name, const std::string& value, bool valid) : Variable(std::move(name), type::INT) {
            if (valid) {
                value_ = std::stoi(value);
            }
            setValid(valid);
        }

        [[nodiscard]] int GetValue() const;

    };

    class Float : public Variable {
        float value_;

    public:
        explicit Float(std::string name, const std::string& value, bool valid) : Variable(std::move(name), type::FLOAT) {
            if (valid) {
                value_ = std::stof(value);
            }
            setValid(valid);
        }

        [[nodiscard]] float GetValue() const;

    };

    class String : public Variable {
        std::string value_;

    public:
        explicit String(std::string name, std::string value, bool valid) : Variable(std::move(name), type::STRING) {
            if (valid) {
                value_ = std::move(value);
            }

            setValid(valid);
        }

        [[nodiscard]] std::string GetValue() const;

    };

    class Bool : public Variable {
        bool value_;

    public:
        explicit Bool(std::string name, const std::string& value, bool valid) : Variable(std::move(name), type::BOOL) {
            if (valid) {
                value_ = (value == "true");
            }

            setValid(valid);
        }

        [[nodiscard]] bool GetValue() const;

    };

    class Array : public Variable {
        std::vector<Variable*> array_;

        std::vector<std::string> ArraySplit(const std::string& data);

    public:
        Variable& operator[](int index) final;


        explicit Array(std::string name, const std::string& value, bool valid);

    };
}

namespace sections {
    class Section : public omflelement::OMFLElement {
    public:
        std::map<std::string, variables::Variable*> variables_;
        std::map<std::string, Section*> sections_;

        explicit Section(std::string name) : OMFLElement(std::move(name), omflelement::SECTION) {}

        omflelement::OMFLElement& Get(const std::string& name) final;

        void AddSection(const std::vector<std::string>& way, int index=0);

        void AddVariable(const std::string& sectionName, variables::Variable* variable);
    };
}

#endif //LAB6_UTILS_H

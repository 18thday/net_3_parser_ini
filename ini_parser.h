#pragma once


#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>


namespace ini {

using section = std::unordered_map<std::string, std::string>;

class document {
public:
    explicit document(std::string filename);

    template <typename T>
    T get_value(std::string full_var_name) const;

    size_t get_section_count() const;

    const section& get_section(const std::string& name) const;

    void print_document(std::ostream& os) const;

private:
    std::unordered_map<std::string, section> sections_;
    section dummy_;

    section& add_section(std::string name);

    std::pair<std::string, std::string> parse_variable_name(const std::string& full_var_name) const;

    void load(std::istream& is);
};

template <typename T>
T document::get_value(std::string full_var_name) const {
    auto [sec, var] = parse_variable_name(full_var_name);
    // check for exception
    if (sections_.count(sec) == 0) {
        throw std::runtime_error("Access Error: there is no section with name " + sec);
    }
    if (sections_.at(sec).count(var) == 0) {
        std::cerr << std::endl << "In Section [" << sec << "] there is next variables:" << std::endl;
        for (const auto& [key, _] : sections_.at(sec)) {
            std::cerr << key << std::endl;
        }

        throw std::runtime_error("Unknown Variable Error: there is no variable with name " + var + " in section " + sec);
    }
    // get value
//        std::string value = sections_.at(sec).at(var);
//        if (typeid(T) == typeid(int)) {
//            T result = std::stoi(value);
//            return result;
//        } else if (typeid(T) == typeid(double) || typeid(T) == typeid(float)) {
//            T result = std::stod(value);
//            return result;
//        } else if (typeid(T) == typeid(std::string)) {
//            return value;
//        }
//        throw std::runtime_error("Type T in get_value<T> is not supported");

    std::istringstream value(sections_.at(sec).at(var));
    T result;
    value >> result;
    return result;
}

} // namespace ini

#include "ini_parser.h"


ini::document::document(std::string filename) {
    std::ifstream ifs;
    ifs.open(filename);
    if (!ifs.good()) {
        throw std::runtime_error("File Error: cannot open file with name " + filename);
    }
    load(ifs);
    ifs.close();
}

size_t ini::document::get_section_count() const {
    return sections_.size();
}

const ini::section& ini::document::get_section(const std::string& name) const {
    return (sections_.count(name)) ? sections_.at(name) : dummy_;
}

void ini::document::print_document(std::ostream& os) const {
    os << "; ini file content" << std::endl;
    for (const auto& [name, section] : sections_) {
        os << '[' << name << ']' << '\n';
        for (const auto& [key, value] : section) {
            os << key << "=" << value << '\n';
        }
    }
}

ini::section& ini::document::add_section(std::string name) {
    return sections_[name];
}

std::pair<std::string, std::string> ini::document::parse_variable_name(const std::string& full_var_name) const {
    std::string_view sec;
    std::string_view var_name;

    std::string_view sv = full_var_name;
    sv.remove_prefix(std::min(sv.find_first_not_of(' '), sv.size()));
    size_t pos = sv.find_last_of('.');
    if (pos == sv.npos) {
        throw std::runtime_error("Syntax Error: there is no delimiter \".\" in " + full_var_name);
    }

    sec = sv.substr(0, pos);
    sv.remove_prefix(pos + 1);
    sv.remove_prefix(std::min(sv.find_first_not_of(' '), sv.size()));

    var_name = sv.substr(0, std::min(sv.find_last_not_of(' ') + 1, sv.size()));

    return {std::string(sec), std::string(var_name)};
}

void ini::document::load(std::istream& is) {
    std::string line;
    section* sec;

    size_t cur_line = 0;
    while(std::getline(is, line)) {
        ++cur_line;
        std::string_view sv = line;
        sv.remove_prefix(std::min(sv.find_first_not_of(' '), sv.size()));

        // skip comment and empty line
        if (sv.empty() || sv[0] == ';') {
            continue;
        }

        // ignore comment
        size_t comment_pos = sv.find(';');
        if (comment_pos != sv.npos) {
            sv = sv.substr(0,comment_pos);
        }

        // get section name
        if (sv[0] == '['){
            size_t c_bracket = sv.find(']');
            if (c_bracket == sv.npos) {
                throw std::runtime_error("Syntax Error: ] bracket is not found in line: " +
                                         std::to_string(cur_line));
            }
            sec = &this->add_section(std::string(sv.substr(1, c_bracket-1)));
            // get key=value
        } else {
            size_t c_equal = sv.find('=');
            if (c_equal == 0) {
                throw std::runtime_error("Syntax Error: there is no variable name before symbol \"=\" in line: " +
                                         std::to_string(cur_line));
            }
            if (c_equal == sv.npos) {
                continue;
            }
            std::string_view key = sv.substr(0, c_equal);
            key = key.substr(0, std::min(key.find_last_not_of(' ') + 1, key.size()));

            sv.remove_prefix(c_equal + 1);

            sv.remove_prefix(std::min(sv.find_first_not_of(' '), sv.size()));
            std::string_view value = sv.substr(0, std::min(sv.find_last_not_of(' ') + 1, sv.size()));
//            (*sec).insert({std::string(key), std::string(value)}); // dont update value
            (*sec)[std::string(key)] = std::string(value);
        }
    }
}

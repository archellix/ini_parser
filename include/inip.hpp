/**
 * @file inip.hpp
 * @brief Ini parser main header file
 * @author Artem Budnikov
 * @date 1 December 2022
 * 
 * Parser of *.ini files. Based on spirit library.
 * @see https://github.com/archellix/ini_parser
 */
#include <boost/algorithm/string.hpp>

using namespace std;
/**
 * @brief An entry of .ini file.
 * 
 * A 'Entry' is used to represents one record of .ini file as key-value pair.
 */
typedef pair<string, string> Entry;
/**
 * @brief List of records of a section.
 * 
 * A 'Entries' is used to represents a list of records of one section.
 */
typedef list<Entry> Entries;
/**
 * @brief A section with entries.
 * 
 * A 'Scetion' is used to represents a section in .ini file as key-value pair.
 */
typedef pair<string, Entries> Section;
/**
 * @brief Data of .ini file.
 * 
 * A 'IniData' is used to represents all section which .ini file contains.
 */
typedef list<Section> IniData;
/**
 * @brief Functor to add section.
 * 
 * A functor that will be called each time the parser recognizes a new section.
 */
struct add_section 
{
    IniData &data_;
    
    add_section(IniData &data) : data_(data) {}
    
    void operator()(char const* p, char const* q) const 
    {
        string s(p,q);
        boost::algorithm::trim(s);
        data_.push_back(Section(s, Entries()));
    }
};
/**
 * @brief Functor to add key.
 * 
 * A functor that will be called each time the parser recognizes a key of record.
 */
struct add_key 
{
    IniData &data_;

    add_key(IniData &data) : data_(data) {}

    void operator()(char const* p, char const* q) const 
    {
        string s(p, q);
        boost::algorithm::trim(s);
        data_.back().second.push_back(Entry(s, string()));
    }
};
/**
 * @brief Fucntor to add value.
 * 
 * A functor that will be called each time the parser recognizes a value of record. 
 */
struct add_value 
{
    IniData &data_;

    add_value(IniData &data) : data_(data) {}

    void operator()(char const* p, char const* q) const 
    {
        data_.back().second.back().second.assign(p, q);
    }
};
/**
 * @brief Functor to recognize comments.
 * 
 * A simple predicate to determine if the current record is a comment or not.
 */
struct is_comment 
{
    bool operator()(string const &s) const {
        return s[0] == '\n' || s[0] == ';';
    }
};
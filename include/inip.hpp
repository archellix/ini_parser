/**
 * @file ini_p.hpp
 * @brief Ini parser main header file
 * @author Artem Budnikov
 * @date 1 December 2022
 * 
 * Parser of *.ini files. Based on spirit library.
 * @see https://github.com/archellix/ini_parser
 */
using namespace std;
/**
 * @brief Result of parsing .ini fiile. Represents one record as pair.
 * 
 * A 'Entry' is used to represend one record of .ini file as key-value pair.
 */
typedef pair<string, string> Entry;
/**
 * @brief Result of parsing .ini fiile. Represents one record as pair.
 * 
 * A 'Entry' is used to represend one record of .ini file as key-value pair.
 */
typedef list<Entry> Entries;
/**
 * @brief Result of parsing .ini fiile. Represents one record as pair.
 * 
 * A 'Entry' is used to represend one record of .ini file as key-value pair.
 */
typedef pair<string, Entries> Section;
/**
 * @brief Result of parsing .ini fiile. Represents one record as pair.
 * 
 * A 'Entry' is used to represend one record of .ini file as key-value pair.
 */
typedef list<Section> IniData;
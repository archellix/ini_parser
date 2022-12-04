#include <fstream>
#include <functional>
#include <numeric>
#include <list>
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/spirit.hpp>

#include "inip.hpp"

using namespace std;
using namespace boost::spirit;
/**
 * @brief Functor to find by key in pair.
 */
struct first_is 
{
    string const &s_;
    first_is(std::string const &s) : s_(s) {}

    template<class Pair>
    bool operator()(Pair const &p) const {
        return p.first == s_;
    }
};
/**
 * @brief 
 * 
 * @param ini 
 * @param s 
 * @param p 
 * @param res 
 * @return true 
 * @return false 
 */
bool find_value (IniData const &ini, string const &s, string const &p, string &res)
{
    IniData::const_iterator sit = find_if(ini.begin(), ini.end(), first_is(s));
    if (sit == ini.end()) 
    {
        return false;
    }

    Entries::const_iterator it = find_if(sit->second.begin(), sit->second.end(), first_is(p));
    if(it == sit->second.end()) 
    {
        return false;
    }

    res = it->second;
    return true;
}
/**
 * @brief Parser of ini files.
 * 
 * Specification:
 * 
 * inidata = spaces, {section} .
 * section = "[", ident, "]", stringSpaces, "\n", {entry} .
 * entry = ident, stringSpaces, "=", stringSpaces, value, "\n", spaces .
 * ident = identChar, {identChar} .
 * identChar = letter | digit | "_" | "." | "," | ":" | "(" | ")" | "{" | "}" | "-" | "#" | "@" | "&" | "*" | "|" .
 * value = {not "\n"} .
 * stringSpaces = {" " | "\t"} .
 * spaces = {" " | "\t" | "\n" | "\r"} .
 */
struct inidata_parser : public grammar<inidata_parser> 
{
    IniData &data_;
    
    inidata_parser(IniData &data) : data_(data) {}

    template <typename ScannerT> 
    struct definition 
    {
        rule<ScannerT> inidata, section, entry, ident, value, stringSpaces, spaces;
        rule<ScannerT> const &start() const { return inidata; }

        definition(inidata_parser const &self) 
        {
            inidata = *section;

            section = ch_p('[')
                    >> ident[add_section(self.data_)]
                    >> ch_p(']')
                    >> stringSpaces
                    >> ch_p('\n')
                    >> spaces
                    >> *(entry);
            
            entry = ident[add_key(self.data_)]
                    >> stringSpaces
                    >> ch_p('=')
                    >> stringSpaces
                    >> value[add_value(self.data_)]
                    >> spaces;

            ident = +(alnum_p | chset<>("-_.,:(){}#@&*|"));

            value = *(~ch_p('\n'));

            stringSpaces = *blank_p;

            spaces = *space_p;
        }
    };
};

int main(int argc, char** argv) 
{
    if (argc != 4) 
    {
        cout << "Usage: " << argv[0] << " <file.ini> <section> <parameter>" << endl;
        return 0;
    }

    ifstream in(argv[1]);
    if (!in) 
    {
        cout << "Can't open file \"" << argv[1] << "\"" << endl;
        return 1;
    }

    vector<string> lns;

    std::string s;
    while (!in.eof()) 
    {
        std::getline(in, s);
        boost::algorithm::trim(s);
        lns.push_back(s+='\n');
    }
    lns.erase(remove_if(lns.begin(), lns.end(), is_comment()), lns.end());
    string text = accumulate(lns.begin(), lns.end(), string());

    IniData data;
    inidata_parser parser(data);
    BOOST_SPIRIT_DEBUG_NODE(parser);

    parse_info<> info = parse(text.c_str(), parser, nothing_p);
    if (!info.hit) 
    {
        cout << "Parse error\n";
        return 1;
    }
    
    string res;
    if (find_value(data, argv[2], argv[3], res)) 
    {
        cout << res;
    } else 
    {
        cout << "Can't find requested parameter";
    }
    cout << endl;
}
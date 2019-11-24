
#include "json.hpp"
#include <any>
#include "JsonE.hpp"
#include <string>
#include <vector>

using namespace std;

void Json::object(string s, size_t i)
{
    size_t start = i;
    while (start < s.size())
    {
        if (s[start] != '\"')
            throw Except();
        string key = s.substr(start+1, s.find('\"', start+1)-start - 1);
        start = s.find('\"', start + 1)+1;
        if (s[start] != ':')
            throw Except();
        any val = get <0>(value(s, start+1));
        start += get <1>(value(s, start+1));
        if (s[start] == '}')
        { obj.insert({ key, val });
            break; }
        if (s[start] != ',')
            throw Except();
        ++start;
        obj.insert({ key, val });

    }
}

void Json::array(string s, size_t i)
{
    size_t start = i;
    size_t ch = 0;
    while (start < s.size())
    {
        any val = get <0> (value(s, start));
        start = start - 1 + get <1>(value(s, start));
        if (s[start] == ']')
        { arr.insert({ ch, val });
            break; }
        if (s[start] != ',')
            throw Except();
        arr.insert({ ch, val });
        ++start; ++ch;
    }
}

pair <any, int> Json:: value(string s, size_t i) {
    pair<any, int> p;

    size_t start = i;

    if (s.substr(start, 5) == "false") {
        bool value1 = false;
        any value = value1;
        return p = make_pair(value, 6);
    }

    if (s.substr(start, 4) == "true") {
        bool value1 = true;
        any value = value1;
        return p = make_pair(value, 5);
    }

    if (s.substr(start, 4) == "null") {
        auto value1 = nullptr;
        any value = value1;
        return p = make_pair(value, 5);
    }

    if (s[start] == '\"') {
        string value1 = s.substr(start + 1, s.find('\"', start + 1) - start - 1);
        any value = value1;
        return p = make_pair(value, s.find('\"', start + 1) + 2 - start);
    }

    if (s[start] == '[')
        return special2(s, start);

    if (s[start] == '{')
        return special1(s, start);

    else
        return numbers(s, start);
}

pair <any, size_t> Json::numbers(string s, size_t start)
{
    pair<any, int> p;
    size_t a; any value;
    size_t a1 = s.find(',', start);
    size_t a2 = s.find(']', start);
    size_t a3 = s.find('}', start);

    if (a1 < a2 && a1 < a3) {
        double value1 = stod(s.substr(start, s.find(',', start) - start));
        value = value1;
        a =s.find(',', start + 1) + 1 - start;
    }

    if (a2 < a1 && a2 < a3) {
        double value1 = stod(s.substr(start, s.find(']', start) - start));
        value = value1;
        a =s.find(']', start + 1) + 1 - start;
    }

    if (a3 < a2 && a3 < a1) {
        double value1 = stod(s.substr(start, s.find('}', start) - start));
        value = value1;
        a = s.find('}', start + 1) + 1 - start;
    }

    return p = make_pair(value, a);
}



pair <any, size_t> Json::special1(string s, size_t start) {
    pair<any, int> p;
    size_t finish = start+1;
    size_t a = 1;
    while (true) {
        if (s[finish] == '{')
            ++a;
        if (s[finish] == '}' && a != 0)
            --a;
        if (s[finish] == '}' && a == 0){
            Json te(s.substr(start, finish - start+1));
            any value = te;
            return p = make_pair(value, finish + 2 - start);
        }
        ++finish;
    }
}
pair <any, size_t> Json::special2(string s, size_t start) {
    pair<any, int> p;
    size_t finish = start+1;
    size_t b = 1;
    while (true) {
        {
            if (s[finish] == ']' && b != 0)
                --b;
            if (s[finish] == '[')
                ++b;
            if (s[finish] == ']' && b == 0) {
                Json te(s.substr(start, finish - start+1));
                any value = te;
                return p = make_pair(value, finish + 2 - start);
            }
            ++finish;
        }
    }
}
Json::Json(std::string s)
{
    for (size_t p = 0; p < s.size(); ++p) {
        if (s[p] == '\n' || s[p] == '\r' || s[p] == '\t' || s[p] == ' ')
            s.erase(p, 1); }

    if (s[0] == '{')
        object(s, 1);
    if (s[0] == '[')
        array(s, 1);
}

bool Json::is_array() const
{
    if (arr.size() == 0)
    {
        return false;
    }
    else { return true; }
}

bool Json::is_object() const
{
    if (obj.size() == 0)
    {
        return false;
    }
    else { return true; }

}

any Json::operator[](const std::string& key)
{
        if (obj.size() == 0 || arr.size() != 0)
        {
            throw Except();
        }

        else
        {
            any search = obj.find(key)->second;

            return search;
        }

}

std::any Json::operator[](int index)
{
        if (arr.size() == 0 || obj.size() != 0)
        {
            throw Except();
        }

        else
        {
            any search = arr.find(index)->second;

            return search;
        }
}


Json Json::parseFile(const std::string& path_to_file)
{
    ifstream file(path_to_file);

    string line = ""; string line2 = "";

    if (!file.is_open()) {
        throw Except();
    }
    else
    {
        while (!file.eof())
        {
            getline(file, line2);

            line = line + line2;
        }
    }
    file.close();

    Json obj3(line);

    return obj3;
}

Json Json::parse(const std::string& s)
{
    string line = s;
    Json obj3(line);
    return obj3;
}

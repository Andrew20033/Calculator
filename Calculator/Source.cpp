#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <functional>



typedef std::function<int(int, int)> func_t;
typedef std::pair<char, func_t> pair_t;



template<typename To, typename From>
To cast_to(const From& v) {
    To result;
    std::stringstream ss;
    ss << v;
    ss >> result;
    return result;
}



struct operation_t {
    enum { add = '+', sub = '-', mul = '*', div = '/' };
    operation_t(int a, int b) :v1(a), v2(b) {}
    int v1, v2;
};

operation_t separate(const std::string& exp, std::vector<pair_t>::const_iterator i) {
    std::string::const_iterator it = std::find(exp.begin(), exp.end(), i->first);
    return operation_t(
        cast_to<int>(std::string(exp.begin(), it)),
        cast_to<int>(std::string(it + 1, exp.end()))
    );
}



int calculate(const std::string& exp) {
    static std::vector<pair_t> pairs = {
       {operation_t::add, [](int a, int b) {return a + b; } },
       {operation_t::sub, [](int a, int b) {return a - b; } },
       {operation_t::mul, [](int a, int b) {return a * b; } },
       {operation_t::div, [](int a, int b) {return a / b; } }
    };
    static std::map<char, func_t> map(pairs.begin(), pairs.end());
    /** remove all spaces */
    std::string correctexp;
    std::remove_copy(exp.begin(), exp.end(), std::back_inserter(correctexp), ' ');
    /** find op`s symbol */
    std::vector<pair_t>::const_iterator it = std::find_if(pairs.begin(), pairs.end(),
        [&](const pair_t& p) {
            return std::find(correctexp.begin(), correctexp.end(), p.first) != correctexp.end();
        }
    );
    if (it == pairs.end()) { throw std::runtime_error("error in expression: \"" + correctexp + "\""); }
    /** separate expression into pieces */
    operation_t op = separate(correctexp, it);
    /** evaluate */
    return map[it->first](op.v1, op.v2);
}



int main() {
    std::cout << "1+2=" << calculate("1+2") << std::endl
        << "4-2=" << calculate("4-2") << std::endl
        << "2*2=" << calculate("2*2") << std::endl
        << "4/2=" << calculate("4/2") << std::endl;

    return 0;
}


#include <iostream>
#include <string>
#include "src/Array.h"

using std::cout;
using std::endl;
using std::string;

int main()
{
    Array<string> a({"ant", "bison", "camel", "duck", "elephant"});
    a.output(cout);
    a.slice(2).output(cout);
    a.slice(2, 4).output(cout);
    a.slice(1, 5).output(cout);
    a.slice().output(cout);

    Array<int> b({1, 2, 3, 4, 5});
    b.output(cout);
    cout << b.reduce<int>([](int res, const int el) { return res + el; }, 0)
         << endl;

    cout << a.reduce<string>(
                [](string res, const string& el) { return res + el + ' '; }, "")
         << endl
         << a.reduceRight<string>(
                [](string res, const string& el) { return res + el + ' '; }, "")
         << endl;

    Array<size_t> l;
    a.forEach([&](const string& el) { l.push({el.length()}); });
    a.output(cout);
    l.output(cout);

    l.forEach([](int el) { cout << el << " "; });
    cout << endl;

    return 0;
}
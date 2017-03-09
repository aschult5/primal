#include <stdexcept> //out_of_range
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>   //uint64_t
#include <boost/multiprecision/miller_rabin.hpp>

using namespace std;
using ull = uint64_t;

static bool isPrimal(ull n)
{
   using namespace boost::multiprecision;
   return miller_rabin_test(n, 25);
}

static bool isNumber(const string& s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(),
                         [](const char& c) { return !std::isdigit(c); }) == s.end();
}

void printUsage(ostream& out)
{
   out << "Pass a positive integer or a file containing positive integers seperated by whitespace" << endl;
}

bool getInput(string arg, vector<ull>& numbers)
{
   if (isNumber(arg))
   {
      // Treat as single integer
      try
      {
         numbers.push_back(stoull(arg));
      } catch (const out_of_range& oor) {
         cerr << "Could not convert " << arg << ": " << oor.what() << endl;
         return false;
      }

   } else {
      // Treat as file
      ifstream input(arg);
      if (!input.good())
      {
         cerr << "Could not open " << arg << endl;
         return false;
      }

      ull num;
      while (input >> num)
      {
         numbers.push_back(num);
      }
   }

   return true;
}

int main(int argc, char const *argv[])
{
   if (argc != 2)
   {
      printUsage(cerr);
      return 1;
   }

   string arg(argv[1]);
   if (arg.compare("-h")==0 || arg.compare("--help")==0)
   {
      printUsage(cout);
      return 0;
   }

   vector<ull> numbers;
   getInput(arg, numbers);

   for (auto const& num: numbers)
   {
      cout << num << " is ";
      if (isPrimal(num))
         cout << "PRIME" << endl;
      else
         cout << "not prime" << endl;
   }

   return 0;
}
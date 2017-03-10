#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>   //uint64_t
#include <string>

#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/program_options.hpp>

#include "utils.hpp"
//#include "server.hpp"

// TODO: move this out
static bool isPrimal(big n)
{
   using namespace boost::multiprecision;
   return miller_rabin_test(n, 25);
}


using namespace std;
namespace po = boost::program_options;

bool collectNumbers(po::variables_map vm, vector<big>& numbers)
{
   // Must be exception-safe
   // Bad user-input will throw

   if (vm.count("num"))
   {
      numbers.push_back(vm["num"].as<big>());
   }
   if (vm.count("file"))
   {
      string f;
      f = vm["file"].as<string>();

      ifstream input(f);
      if (!input.good())
      {
         cerr << "Could not open " << f << endl;
         return false;
      }

      big num;
      while (input >> num)
      {
         numbers.push_back(num);
      }
   }

   return true;
}

int main(int argc, char const *argv[])
{
   po::options_description desc("Options:");
   desc.add_options()
      ("help,h", "Print this message")
      ("file,f", po::value<string>(), "Client: File containing big integers to test")
      ("num,n", po::value<big>(), "Client: Big integer to test")
      //("port,p", po::value<uint16_t>(), "Client/Server: Server's port")
      //("ip,i", po::value<string>(), "Client: Specify the target server IP address")
      ;

   // Read command-line arguments
   po::variables_map vm;
   po::store(po::parse_command_line(argc, argv, desc), vm);
   po::notify(vm);

   if (vm.count("help"))
   {
      cout << desc << endl;
      return 0;
   }

   // Client
   if (vm.count("ip") || vm.count("num") || vm.count("file"))
   {
      vector<big> numbers;
      if (!collectNumbers(vm, numbers))
         return 1;
      //return startClient(ip, port, numbers);

      // TODO: move this out
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

   // Server
   //return startServer(port);

   return 0;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>   //uint16_t
#include <string>

#include <boost/program_options.hpp>

#include "server.hpp"
#include "client.hpp"


using namespace std;
namespace po = boost::program_options;

bool collectNumbers(po::variables_map vm, vector<uint64_t>& numbers)
{
   // Must be exception-safe
   // Bad user-input will throw

   if (vm.count("num"))
   {
      numbers.push_back(vm["num"].as<uint64_t>());
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

      uint64_t num;
      while (input >> num)
      {
         numbers.push_back(num);
      }
   }

   return true;
}

int main(int argc, char const *argv[])
{
   uint16_t port = 1734; // User Port for "Camber Corporation License Management"
   po::options_description desc("Clients must specify the server ip and numbers to test");
   desc.add_options()
      ("help,h", "Print this message")
      ("file,f", po::value<string>(), "Client: File containing uint64_t integers to test")
      ("num,n", po::value<uint64_t>(), "Client: Big integer to test")
      //("ip,i", po::value<string>(), "Client: Specify the target server IP address")
      //("port,p", po::value<uint16_t>(), "Client/Server: Server's port")
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


   if (vm.count("num") || vm.count("file") || vm.count("ip"))
   {
      // Client
      string ip = "127.0.0.1";
      if (vm.count("ip"))
      {
         ip = vm["ip"].as<string>();
      }
      vector<uint64_t> numbers;
      if (!collectNumbers(vm, numbers))
         return 1;
      return startClient(ip, port, numbers) ? 0 : 1;
   }
   else
   {
      // Server
      Primal::Server s(port);
      return s.listen() ? 0 : 1;
   }
}

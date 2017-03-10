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
   uint16_t port = 1734; // User Port for "Camber Corporation License Management"
   po::options_description desc("Clients must specify the server ip and numbers to test");
   desc.add_options()
      ("help,h", "Print this message")
      ("file,f", po::value<string>(), "Client: File containing big integers to test")
      ("num,n", po::value<big>(), "Client: Big integer to test")
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


   // Client
   if (vm.count("num") || vm.count("file") || vm.count("ip"))
   {
      string ip = "127.0.0.1";
      if (vm.count("ip"))
      {
         ip = vm["ip"].as<string>();
      }
      vector<big> numbers;
      if (!collectNumbers(vm, numbers))
         return 1;
      return startClient(ip, port, numbers) ? 0 : 1;
   }


   // Server
   return startServer(port) ? 0 : 1;
}

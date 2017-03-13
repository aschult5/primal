#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>   //uint16_t
#include <string>

#include <boost/program_options.hpp>

#include "primal.hpp" //big
#include "server.hpp"
#include "client.hpp"


using namespace std;
using namespace primal;
namespace po = boost::program_options;

static bool formRequest(po::variables_map vm, request& numbers)
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

   return !numbers.empty();
}

int main(int argc, char const *argv[])
{
   uint16_t port = 1734; // User Port for "Camber Corporation License Management"
   po::options_description desc("Clients must specify the server ip and numbers to test");
   desc.add_options()
      ("help,h", "Print this message")
      ("file,f", po::value<string>(), "Client: File containing integers to test")
      ("num,n", po::value<big>(), "Client: Integer to test")
      ("ip,i", po::value<string>(), "Client: Specify the target server IP address")
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
      string ip("127.0.0.1");
      if (vm.count("ip"))
      {
         ip = vm["ip"].as<string>();
      }

      // Form request
      request numbers;
      if (!formRequest(vm, numbers))
      {
         cerr << desc << endl;
         return 1;
      }
      response result(numbers.size());

      // Send request, retrieve result
      try
      {
         client c(ip, port);
         if (!c.sendRequest(numbers, result))
            return 3;
      }
      catch (std::exception& e)
      {
         cerr << e.what() << endl;
         return 2;
      }

      // Print result
      for (unsigned i=0; i<result.size(); ++i)
      {
         cout << numbers[i] << " is ";
         if (result[i] == 1)
            cout << "PRIME" << endl;
         else
            cout << "not prime" << endl;
      }
   }
   else
   {
      // Server
      try
      {
         server s(port);
      }
      catch (std::exception& e)
      {
         cerr << e.what() << endl;
         return 2;
      }
   }

   return 0;
}

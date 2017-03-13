#include <iostream>
#include <fstream>
#include <sstream>
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

static void printResult(const request& numbers, const response& res)
{
   // Print result
   cout << endl;
   for (unsigned i=0; i<res.size(); ++i)
   {
      cout << numbers[i] << " is ";
      switch(res[i])
      {
         case custom::primality::GUARANTEED:
            cout << "PRIME" << endl;
            break;
         case custom::primality::PROBABLY:
            cout << "probably PRIME" << endl;
            break;
         case custom::primality::NOT:
            cout << "not prime" << endl;
            break;
      }
   }
   cout << endl;
}

int main(int argc, char const *argv[])
{
   uint16_t port = 1734; // User Port for "Camber Corporation License Management"
   po::options_description desc("Clients must specify the server ip and numbers to test");
   desc.add_options()
      ("help,h", "Print this message")
      ("file,f", po::value<string>(), "Client: File containing integers to test")
      ("num,n", po::value<big>(), "Client: Integer to test")
      ("ip,i", po::value<string>(), "Client: Specify the target server IP address. Default: 127.0.0.1")
      ("pseudoprime", po::value<uint128_t>(), "Server: Specify 318665857834031151167461 for 128bit support")
      //("port,p", po::value<uint16_t>(), "Client/Server: Server's port")
      ;

   // Read command-line arguments
   po::variables_map vm;
   try
   {
      po::store(po::parse_command_line(argc, argv, desc), vm);
   }
   catch (std::exception& e)
   {
      cerr << e.what() << endl;
      return 1;
   }
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

      // Connect
      unique_ptr<client> c_p;
      try
      {
         c_p = make_unique<client>(ip, port);
      }
      catch (std::exception& e)
      {
         cerr << e.what() << endl;
         return 2;
      }

      // Form request
      request numbers;
      response results;
      if (!formRequest(vm, numbers))
      {
         cout << "Interactive mode" << endl;

         cout << "Type white-space-separated integers to test, followed by enter:" << endl;
         string line;
         while (getline(cin, line))
         {
            // Get input
            istringstream iss(line); 
            big num;
            while (iss >> num)
               numbers.push_back(num);
            results.resize(numbers.size());

            // Send request, retrieve results
            if (!c_p->sendRequest(numbers, results))
               return 3;

            printResult(numbers, results);
            numbers.clear();
         }
      }
      else
      {
         cout << "Batch mode" << endl;

         // Send request, retrieve results
         results.resize(numbers.size());
         if (!c_p->sendRequest(numbers, results))
            return 3;
         printResult(numbers, results);
      }
   }
   else
   {
      uint128_t pp=0;
      if (vm.count("pseudoprime"))
      {
         pp=vm["pseudoprime"].as<uint128_t>();
      }
      // Server
      try
      {
         server s(port,pp);
      }
      catch (std::exception& e)
      {
         cerr << e.what() << endl;
         return 2;
      }
   }

   return 0;
}

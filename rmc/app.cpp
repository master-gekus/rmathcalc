#include <string>
#include <iostream>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>

#include "gen-cpp/MathCalc.h"

#include "app.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace mathcalc;

MathCalcClientApp::MathCalcClientApp() :
  client_(nullptr)
{

}

MathCalcClientApp::~MathCalcClientApp()
{
  if (nullptr != client_)
    delete client_;
  client_ = nullptr;
}

int MathCalcClientApp::workloop()
{
  client_->ping();
  TaskList list;
  client_->taskList(list);
  return 0;
}

bool MathCalcClientApp::connect(const std::string& host, int port)
{
  boost::shared_ptr<TTransport> socket(new TSocket(host, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));

  try
    {
      cout << "Trying connect to " << host << ":" << port << "..." << endl;
      transport->open();
      cout << "Connection established. Type 'q' to exit." << endl;
    }
  catch(TException& tx)
    {
      cout << "THRIFT exceptio: " << tx.what() << endl;
      return 1;
    }

  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  client_ = new MathCalcClient(protocol);

  return true;
}

int MathCalcClientApp::exec(int argc, char *argv[])
{
  string host("localhost");
  int port = 9876;

  if (argc > 1)
    {
      host = argv[1];
    }

  if (argc > 2)
    {
      try
        {
          port = stoi(argv[2], nullptr, 0);
        }
      catch(...)
        {
          port = 0;
        }


      host = argv[1];
    }
  if ((1 > port) || (0xFFFF < port))
    {
      cout << "Invalid port \"" << argv[2]
           << "\". Port must be an integer value in range 1..65535." << endl;
      return 1;
    }

  if (!connect(host, port))
    {
      return 1;
    }

  return workloop();
}

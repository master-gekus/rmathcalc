#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "gen-cpp/MathCalc.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace mathcalc;

int main(int argc, char *argv[])
{
  boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9876));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  MathCalcClient client(protocol);

  try
    {
      cout << "Connecting..." << endl;
      transport->open();
      cout << "Connected." << endl;

      client.ping();
      cout << "ping()" << endl;

      return 0;
    }
  catch(TException& tx)
    {
      cout << "ERROR: " << tx.what() << endl;
      return 1;
    }
  catch(...)
    {
      cout << "Unknown exception." << endl;
      return 2;
    }
}

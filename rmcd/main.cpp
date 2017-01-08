#include <iostream>

#include <boost/make_shared.hpp>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>

#include "math_calc_handler.h"

using namespace std;

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace mathcalc;

class MathCalcCloneFactory : virtual public MathCalcIfFactory {
 public:
  ~MathCalcCloneFactory()
  {
  }

  MathCalcIf* getHandler(const TConnectionInfo& connInfo) override
  {
    boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
    cout << "Incoming connection\n";
    cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
    cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
    cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
    cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
    return new MathCalcHandler;
  }

  void releaseHandler(MathCalcIf* handler) override
  {
    delete handler;
  }
};

int main(int argc, char *argv[])
{
  TThreadedServer server(
    boost::make_shared<MathCalcProcessorFactory>(boost::make_shared<MathCalcCloneFactory>()),
    boost::make_shared<TServerSocket>(9876),
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>());

  cout << "Starting the RMathCalc server version " << GIT_DESCRIBE << "..." << endl;
  server.serve();
  cout << "Done." << endl;
  return 0;
}

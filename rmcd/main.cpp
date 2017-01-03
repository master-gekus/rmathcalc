#include <iostream>
#include <cassert>

#include <boost/make_shared.hpp>

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>

#include "gen-cpp/MathCalc.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace mathcalc;

class MathCalcHandler : public MathCalcIf
{
public:
  MathCalcHandler(const TConnectionInfo& connInfo) :
    iprot_(connInfo.input),
    oprot_(connInfo.output)
  {
  }

private:
  void ping() override
  {
    cout << "Sending message responce..." << endl;
    send_message("This is asyncronyous message!");
    cout << "Sending ping response..." << endl;
  }

  void message(string&) override
  {
    assert(false);
  }

private:
  void send_message(const string& msg)
  {
    int32_t seqid = 0;
    MathCalc_message_result result;
    result.success = msg;
    result.__isset.success = true;

    oprot_->writeMessageBegin("message", ::apache::thrift::protocol::T_ONEWAY, seqid);
    result.write(oprot_.get());
    oprot_->writeMessageEnd();
    oprot_->getTransport()->writeEnd();
    oprot_->getTransport()->flush();
  }

private:
  boost::shared_ptr<TProtocol> iprot_;
  boost::shared_ptr<TProtocol> oprot_;
};

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
    return new MathCalcHandler(connInfo);
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

  cout << "Starting the server..." << endl;
  server.serve();
  cout << "Done." << endl;
  return 0;
}

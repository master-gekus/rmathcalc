#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <thread>
#include <chrono>

#include <boost/algorithm/string.hpp>

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
  client_(nullptr),
  async_busy_(true),
  async_cmd_('?')
{
}

MathCalcClientApp::~MathCalcClientApp()
{
  if (nullptr != client_)
    delete client_;
  client_ = nullptr;
}

#define THRIFT_CALL(...) \
try \
  { \
    __VA_ARGS__; \
  } \
catch(TException& tx) \
  { \
    cout << "THRIFT exception: " << tx.what() << endl; \
    return false; \
  }

namespace
{
  bool _check_op_result(const OpResult& res, const char *messgae)
  {
    if (res.success)
      return true;

    cout << messgae;

    if (res.__isset.task)
      cout << " (task #" << res.task.id << ")";

    if (res.__isset.message)
      {
        cout << ": " << res.message;
      }
    else
      {
        cout << ". No error description received.";
      }

    cout << endl;

    return false;
  }
}

bool
MathCalcClientApp::process_factorization(const std::string& params)
{
  uint64_t value = 0;
  try
    {
      value = std::stoull(params, nullptr, 0);
    }
  catch(...)
    {
    }

  if (2ULL > value)
    {
      cout << "Invalid number. Number must be a positive integer in range 2.."
           << numeric_limits<uint64_t>::max() << endl;
      return true;
    }

  OpResult res;
  THRIFT_CALL(client_->createTask(res, TaskType::Factorization,
                                  std::vector<int64_t>{static_cast<int64_t>(value)}));

  if (!_check_op_result(res, "Error creating task"))
    return true;

  cout << "Task #" << res.task.id << " (factorization of " << value << ") created." << endl;

  return true;
}

bool
MathCalcClientApp::process_kill(const std::string& params)
{
  int32_t value = 0;
  try
    {
      value = std::stoi(params, nullptr, 0);
    }
  catch(...)
    {
    }

  if (1 > value)
    {
      cout << "Invalid task ID. Task ID must be a positive integer number."<< endl;
      return true;
    }

  OpResult res;
  THRIFT_CALL(client_->killTask(res, value));

  if (!_check_op_result(res, "Error kill task"))
    return true;

  cout << "Task #" << res.task.id << " killed." << endl;

  return true;
}

bool
MathCalcClientApp::process_take(const std::string& params)
{
  int32_t value = 0;
  try
    {
      value = std::stoi(params, nullptr, 0);
    }
  catch(...)
    {
    }

  if (1 > value)
    {
      cout << "Invalid task ID. Task ID must be a positive integer number."<< endl;
      return true;
    }

  OpResult res;
  THRIFT_CALL(client_->takeTask(res, value));

  if (!_check_op_result(res, "Error taking task"))
    return true;

  switch (res.task.type)
    {
    case TaskType::Factorization:
      {
        cout << "Factorization of " << static_cast<uint64_t>(res.task.params[0]) << ": ";
        for (size_t i = 0; i < res.result.size(); i++)
          {
            if (0 != i)
              cout << ", ";
            cout << static_cast<uint64_t>(res.result[i]);
          }
        cout << endl;
      }
      break;

    default:
      cout << "Unknown task type (" << static_cast<int>(res.task.type)
           << ". Result can not be shown." << endl;
      break;
    }

  return true;
}

bool
MathCalcClientApp::process_list()
{
  TaskList list;
  THRIFT_CALL(client_->taskList(list));

  if (list.empty())
    {
      cout << "No active tasks." << endl;
    }
  else
    {
      cout << list.size() << " active task(s):" << endl;
      cout << "    ID State     Task" << endl;
      cout << "------ --------- -------------------------------------------------" << endl;
      for (const Task& task : list)
        {
          cout << setw(6) << right << task.id << " ";

          string state("<invalid>");
          auto ts = _TaskState_VALUES_TO_NAMES.find(task.state);
          if (ts != _TaskState_VALUES_TO_NAMES.cend())
            state = ts->second;
          cout << setw(10) << left << state;

          string type("<unknown>");
          auto tt = _TaskType_VALUES_TO_NAMES.find(task.type);
          if (tt != _TaskType_VALUES_TO_NAMES.cend())
            type = tt->second;
          cout << type;

          cout << right;
          if (!task.params.empty())
              {
                cout << " (";
                for (size_t i = 0; i < task.params.size(); i++)
                  {
                    if (0 < i)
                      cout << ", ";
                    cout << static_cast<uint64_t>(task.params[i]);
                  }
                cout << ")";
              }

          cout << endl;
        }
    }

  return true;
}

bool
MathCalcClientApp::process_ping()
{
  THRIFT_CALL(client_->ping());
  cout << "Ping successfull." << endl;
  return true;
}

bool
MathCalcClientApp::process_help()
{
  cout
    << "Avaible commands:\n"
    << "  ?, h:\n"
    << "    Display this help.\n\n"
    << "  f: <number>:\n"
    << "    Create task for integer factorization of <number>.\n\n"
    << "  l:\n"
    << "    Display list of running and completed tasks.\n\n"
    << "  k <task-id>:\n"
    << "    Kill running or completed task. Results will be lost.\n\n"
    << "  t <task-id>:\n"
    << "    Take result of completed task.\n\n"
    << "  q:\n"
    << "    Quit application.\n\n"
    << "  p:\n"
    << "    Ping server.\n";

  return true;
}

bool
MathCalcClientApp::process_command(char cmd, const std::string& params)
{
  switch (cmd)
    {
    case 'Q':
      return false;

    case '?':
    case 'H':
      return process_help();

    case 'P':
      return process_ping();

    case 'L':
      return process_list();

    case 'F':
      return process_factorization(params);

    case 'K':
      return process_kill(params);

    case 'T':
      return process_take(params);

    default:
      cout << "Unknown command: '" << cmd << "'." << endl;
      break;
    }
  return true;
}

int
MathCalcClientApp::workloop()
{
  while (true)
    {
      cout << "rmc>";
      string s;
      getline(cin, s);
      boost::algorithm::trim(s);

      if (s.empty())
        {
          continue;
        }

      if ('#' == s[0])
        {
          s.erase(0,1);
          boost::algorithm::trim_left(s);
          if (s.empty())
            {
              continue;
            }
        }

      char cmd = toupper(s[0]);
      s.erase(0,1);
      boost::algorithm::trim_left(s);

      if (!process_command(cmd, s))
        break;
    }

  cout << "Good bye!" << endl;

  return 0;
}

bool
MathCalcClientApp::connect(const std::string& host, int port)
{
  boost::shared_ptr<TTransport> socket(new TSocket(host, port));
  boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));

  cout << "Trying connect to " << host << ":" << port << "..." << endl;
  THRIFT_CALL(transport->open());
  cout << "Connection established. Type 'q' to exit, '?' for help." << endl;

  boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  client_ = new MathCalcClient(protocol);

  return true;
}

int
MathCalcClientApp::exec(int argc, char *argv[])
{
  cout << "RMathCalc client version " << GIT_DESCRIBE << endl;

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
  host_ = host;
  port_ = port;

//  if (!connect(host, port))
//    {
//      return 1;
//    }

  async_workloop();

  return 0;
}

bool
MathCalcClientApp::wait_not_busy(bool set_busy)
{
  unique_lock<mutex> lock(async_mutext_);
  async_cond_.wait(lock, [this](){return !async_busy_;});
  async_busy_ = set_busy;
  return ('Q' != async_cmd_);
}

void
MathCalcClientApp::async_workloop()
{
  thread worker([this](){async_worker();});

  while (true)
    {
      if (!wait_not_busy(false))
        break;

      cout << "rmc>";
      string s;
      getline(cin, s);
      boost::algorithm::trim(s);

      if (s.empty())
        {
          continue;
        }

      if ('#' == s[0])
        {
          s.erase(0,1);
          boost::algorithm::trim_left(s);
          if (s.empty())
            {
              continue;
            }
        }

      async_cmd_ =toupper(s[0]);
      s.erase(0,1);
      async_params_ = boost::algorithm::trim_left_copy(s);

      if (!wait_not_busy(true))
        break;

      async_cond_.notify_all();
    }
  cout << "Good bye!" << endl;

  worker.join();
}

void
MathCalcClientApp::async_worker()
{
  async_cmd_ = 'Q';



  cout << "async_worker() started." << endl;
  this_thread::sleep_for(chrono::seconds(2));

  {
    lock_guard<mutex> lock(async_mutext_);
    async_busy_ = false;
  }
  async_cond_.notify_all();

  {
    unique_lock<mutex> lock(async_mutext_);
    //if (cv.wait_for(lk, idx*100ms, []{return i == 1;}))
    async_cond_.wait(lock, [this](){return async_busy_;});
    async_busy_ = true;
  }

  cout << "Command received." << endl;

  this_thread::sleep_for(chrono::seconds(2));

  cout << "async_worker() finished." << endl;
}

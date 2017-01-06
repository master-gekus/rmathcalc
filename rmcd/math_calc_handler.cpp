#include <iostream>
#include <map>

#include <boost/thread/thread.hpp>
#include <boost/chrono/chrono.hpp>

#include "math_calc_handler.h"

using namespace std;

using namespace mathcalc;

class MathCalcHandlerPrivate
{
private:
  MathCalcHandlerPrivate() :
    last_id_(0)
  {
  }

  ~MathCalcHandlerPrivate()
  {
  }

  MathCalcHandlerPrivate(const MathCalcHandlerPrivate&) = delete;
  MathCalcHandlerPrivate& operator =(const MathCalcHandlerPrivate&) = delete;

private:
  void _kill_or_take_task(OpResult& _return, const int32_t id, bool kill)
  {
    auto it = threads_.find(id);
    if (threads_.end() == it)
      {
        _return.__set_success(false);
        stringstream msg;
        msg << "Task #" << id << " not found.";
        _return.__set_message(msg.str());
        return;
      }

    Thread& th = threads_.find(id)->second;
    if (th.is_running_)
      {
        if (kill)
          {
            th.thread_.interrupt();
            th.thread_.join();
          }
        else
          {
            _return.__set_success(false);
            stringstream msg;
            msg << "Task #" << id << " is already running.";
            _return.__set_message(msg.str());
            return;
          }
      }

    _return.__set_success(true);
    _return.__set_task(th.task_);
    _return.task.__set_state(TaskState::Complete);

    if (!kill)
      {
        _return.__set_result(th.result);
        _return.__set_result2(th.result2);
      }

    threads_.erase(id);
  }

private:
  struct Thread
  {
    Task task_;
    boost::thread thread_;
    std::vector<int64_t>  result;
    std::vector<int64_t>  result2;
    bool is_running_ = true;
  };
  map<int32_t,Thread> threads_;
  int32_t last_id_;

  friend class MathCalcHandler;
  friend class std::default_delete<MathCalcHandlerPrivate>;
};

MathCalcHandler::MathCalcHandler() :
  d(new MathCalcHandlerPrivate())
{
}

MathCalcHandler::~MathCalcHandler()
{
}

void
MathCalcHandler::ping()
{
  cout << "ping() called." << endl;
}

void
MathCalcHandler::taskList(TaskList& _return)
{
  cout << "taskList() called." << endl;

  for (auto it = d->threads_.begin(); it != d->threads_.end(); ++it)
    {
      Task t(it->second.task_);
      t.state = it->second.is_running_ ? TaskState::Running : TaskState::Complete;
      _return.push_back(t);
    }
}

void
MathCalcHandler::createTask(OpResult& _return, const TaskType::type type,
                            const std::vector<int64_t> & params)
{
  cout << "createTask() called." << endl;
  _return.__set_success(false);

  MathCalcHandlerPrivate::Thread& th = d->threads_[++(d->last_id_)];
  th.task_.__set_id(d->last_id_);
  switch (type)
    {
    case TaskType::Factorization:
      if (1 != params.size())
        {
          _return.__set_message("Invalid count of parameters.");
          break;
        }
      if ((0 == params[0]) || (1 == params[0]))
        {
          _return.__set_message("Invalid parameter.");
          break;
        }
      _return.__set_success(true);
      break;

    default:
      _return.__set_message("Unknown task type.");
      break;
    }

  if (_return.success)
    {
      th.task_.__set_type(type);
      th.task_.__set_params(params);
      _return.__set_task(th.task_);

      th.thread_ = boost::thread([&th]()
        {
          boost::this_thread::sleep_for(boost::chrono::milliseconds(th.task_.params[0]));
          th.result.push_back(2017); // Fake, fake, but it is a prime!
          cout << "Thread finished!" << endl;
          th.is_running_ = false;
        });
    }
  else
    {
      d->threads_.erase(d->last_id_);
    }
}

void
MathCalcHandler::killTask(OpResult& _return, const int32_t id)
{
  cout << "killTask() called." << endl;

  d->_kill_or_take_task(_return, id, true);
}

void
MathCalcHandler::takeTask(OpResult& _return, const int32_t id)
{
  cout << "takeTask() called." << endl;

  d->_kill_or_take_task(_return, id, false);
}

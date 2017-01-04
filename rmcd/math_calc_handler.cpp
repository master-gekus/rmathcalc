#include <iostream>

#include "math_calc_handler.h"

using namespace std;

using namespace mathcalc;

MathCalcHandler::MathCalcHandler()
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
}

void
MathCalcHandler::createTask(OpResult& _return, const TaskType::type type,
                            const std::vector<int64_t> & params)
{
  cout << "createTask() called." << endl;
  _return.__set_success(false);
  _return.__set_message("Not implemented now.");
}

void
MathCalcHandler::killTask(OpResult& _return, const int32_t id)
{
  cout << "killTask() called." << endl;
  _return.__set_success(false);
  _return.__set_message("Not implemented now.");
}

void
MathCalcHandler::takeTask(OpResult& _return, const int32_t id)
{
  cout << "takeTask() called." << endl;
  _return.__set_success(false);
  _return.__set_message("Not implemented now.");
}

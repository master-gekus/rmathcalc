#ifndef MATH_CALC_HANDLER_H_INCLUDED
#define MATH_CALC_HANDLER_H_INCLUDED

#include "gen-cpp/MathCalc.h"

class MathCalcHandler : public mathcalc::MathCalcIf
{
public:
  MathCalcHandler();
  ~MathCalcHandler();

public:
  void ping() override;
  void taskList(mathcalc::TaskList& _return) override;
  void createTask(mathcalc::OpResult& _return, const mathcalc::TaskType::type type,
               const std::vector<int64_t> & params) override;
  void killTask(mathcalc::OpResult& _return, const int32_t id) override;
  void takeTask(mathcalc::OpResult& _return, const int32_t id) override;
};

#endif // MATH_CALC_HANDLER_H_INCLUDED

#ifndef _RMATHCALC_APP_INCLUDED_
#define _RMATHCALC_APP_INCLUDED_

#include <string>

namespace mathcalc
{
  class MathCalcClient;
}

class MathCalcClientApp
{
public:
  MathCalcClientApp();
  virtual ~MathCalcClientApp();

public:
  int exec(int argc, char *argv[]);

private:
  bool connect(const std::string& host, int port);
  int workloop();

private:
  mathcalc::MathCalcClient* client_;
};

#endif // ifndef _RMATHCALC_APP_INCLUDED_

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
  bool process_command(char cmd, const std::string& params);
  bool process_help();
  bool process_ping();
  bool process_list();
  bool process_factorization(const std::string& params);
  bool process_kill(const std::string& params);
  bool process_take(const std::string& params);

private:
  mathcalc::MathCalcClient* client_;
};

#endif // ifndef _RMATHCALC_APP_INCLUDED_

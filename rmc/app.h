#ifndef _RMATHCALC_APP_INCLUDED_
#define _RMATHCALC_APP_INCLUDED_

#include <string>
#include <mutex>
#include <condition_variable>

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
  std::string host_;
  int port_;

// Async work support
private:
  void async_workloop();
  void async_worker();
  bool wait_not_busy(bool set_busy);

private:
  bool async_busy_;
  std::mutex async_mutext_;
  std::condition_variable async_cond_;
  char async_cmd_;
  std::string async_params_;
};

#endif // ifndef _RMATHCALC_APP_INCLUDED_

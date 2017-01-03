namespace cpp mathcalc

enum TaskType
{
  Factorization
}

enum TaskState
{
  Running, Complete
}

struct Task
{
  1: i32 id;
  2: TaskType type;
  3: TaskState state;
  4: optional list<i64> params;
}

typedef list<Task> TaskList;

service MathCalc
{
  void ping();
  TaskList taskList();
}

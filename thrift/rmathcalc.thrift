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

struct OpResult
{
  1: bool success;
  2: optional Task task;
  3: optional string message;
  4: optional list<i64> result;
  5: optional list<i64> result2;
}

service MathCalc
{
  void ping();
  TaskList taskList();
  OpResult createTask(TaskType type, list<i64> params);
  OpResult killTask(i32 id);
  OpResult takeTask(i32 id);
}

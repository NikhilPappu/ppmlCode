#pragma once
#include <Sh4Types.h>
#include <cryptoTools/Network/Session.h>

#include <boost/variant.hpp>
#include <deque>
#include <function2/function2.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace aby4 {

class Sh4Runtime;
class Sh4TaskBase;

class Sh4Task {
 public:
  using RoundFunc = fu2::unique_function<void(CommPkg& comm, Sh4Task& self)>;
  using ContinuationFunc = fu2::unique_function<void(Sh4Task& self)>;
  enum Type { Evaluation, Closure };

  // returns the associated runtime.
  Sh4Runtime& getRuntime() const { return *mRuntime; }

  // schedules a task that can be executed in the following round.
  Sh4Task then(RoundFunc task);

  // schedule a task that can be executed in the same round as this task.
  Sh4Task then(ContinuationFunc task);

  // schedules a task that can be executed in the following round.
  Sh4Task then(RoundFunc task, std::string name);

  // schedule a task that can be executed in the same round as this task.
  Sh4Task then(ContinuationFunc task, std::string name);

  // Get a task that is fulfilled when all of this tasks dependencies
  // are fulfilled.
  Sh4Task getClosure();

  std::string& name();

  Sh4Task operator&&(const Sh4Task& o) const;

  Sh4Task operator&=(const Sh4Task& o);

  // blocks until this task is completed.
  void get();

  bool isCompleted();

  bool operator==(const Sh4Task& t) const {
    return mRuntime == t.mRuntime && mIdx == t.mIdx && mType == t.mType;
  }
  bool operator!=(const Sh4Task& t) const { return !(*this == t); }

  Sh4TaskBase* basePtr();
  Sh4Runtime* mRuntime = nullptr;
  i64 mIdx = -1;
  Type mType = Evaluation;
};

class Sh4TaskBase {
 public:
  Sh4TaskBase() = default;
  Sh4TaskBase(const Sh4TaskBase&) = delete;
  Sh4TaskBase(Sh4TaskBase&&) = default;
  ~Sh4TaskBase() = default;

  struct EmptyState {};
  struct And {};

  i64 mIdx = -1;

  std::vector<Sh4Task> mDepList;
  std::string mName;

  // The list of downstream tasks that depend on this task.
  // When this task is fulfillied, we will check if these
  // tasks are ready to be run.
  std::vector<i64> mChildren, mUpstreamClosures, mClosureChildren;

  i64 mClosureCount = -1, mDepCount = 0;

  void addChild(Sh4TaskBase& child, Sh4Task::Type type) {
    if (type == Sh4Task::Evaluation)
      mChildren.push_back(child.mIdx);
    else if (type == Sh4Task::Closure)
      mClosureChildren.push_back(child.mIdx);
    else
      throw RTE_LOC;
  }

  void addDep(Sh4Task d) {
    ++mDepCount;
    mDepList.emplace_back(d);
  }

  enum Status { Pending, Evaluated, Closed };
  Status mStatus = Pending;

  boost::variant<EmptyState, And, Sh4Task::RoundFunc, Sh4Task::ContinuationFunc>
      mFunc = EmptyState{};

  void depFulfilled(Sh4Task parent, Sh4Runtime& rt);

  // returns true if this task is initialized.
  bool isValid() const { return mIdx != -1; }

  // returns true if this task is ready to be performed
  bool isReady() const { return mDepCount == 0; }

  // returns true if this task has completed all of its work
  bool isEvaluated() { return mStatus == Evaluated || mStatus == Closed; }

  // returns true if this task can be executed in the same round as its
  // parent/dependent tasks.
  bool isContinuationTask() {
    return boost::get<Sh4Task::ContinuationFunc>(&mFunc);
  }

  bool isRemovable() {
    return isEvaluated() && (mUpstreamClosures.size() == 0);
  }

  bool isClosure() { return mClosureCount != -1; }
  bool isAnd() { return boost::get<And>(&mFunc); }
};

inline std::ostream& operator<<(std::ostream& o, const Sh4Task& d) {
  o << d.mIdx;
  if (d.mType == Sh4Task::Evaluation)
    o << ".E";
  else
    o << ".C";
  return o;
}

class TaskDag {
 public:
  std::deque<i64> mReadyDeque;
  std::unordered_map<i64, Sh4TaskBase> mTaskMap;

  i64 mPushIdx = 0;
  Sh4TaskBase& emplace() {
    auto p = mTaskMap.emplace(mPushIdx, Sh4TaskBase{});
    p.first->second.mIdx = mPushIdx++;
    return p.first->second;
  }
  void enqueueBack(i64 idx) {
    if (mTaskMap.find(idx) == mTaskMap.end()) throw RTE_LOC;
    mReadyDeque.push_back(idx);
  }

  Sh4TaskBase& front() {
    if (mReadyDeque.size() == 0) throw RTE_LOC;
    return mTaskMap.find(mReadyDeque.front())->second;
  }
  void popFront() {
    if (mReadyDeque.size() == 0) throw RTE_LOC;

    auto iter = mTaskMap.find(mReadyDeque.front());
    if (iter == mTaskMap.end()) throw RTE_LOC;

    mReadyDeque.pop_front();

    if (iter->second.isRemovable()) {
      mTaskMap.erase(iter);
    }
  }

  void remove(i64 idx) {
    auto iter = mTaskMap.find(idx);
    if (iter == mTaskMap.end()) throw RTE_LOC;

    mTaskMap.erase(iter);
  }

  Sh4TaskBase* tryGet(i64 idx) {
    if (idx >= mPushIdx)
      throw std::runtime_error(
          "requested task that has not been created. " LOCATION);

    auto iter = mTaskMap.find(idx);
    if (iter == mTaskMap.end()) return nullptr;
    return &iter->second;
  }

  Sh4TaskBase& get(i64 idx) {
    auto ptr = tryGet(idx);
    if (ptr) return *ptr;
    throw std::runtime_error("Task has been evaluated and removed. " LOCATION);
  }

  void reserve(u64 n) { mTaskMap.reserve(n); }

  u64 size() { return mTaskMap.size(); }
};

class Sh4Runtime {
 public:
  Sh4Runtime() = default;
  Sh4Runtime(const Sh4Runtime&) = default;
  Sh4Runtime(Sh4Runtime&&) = default;
  Sh4Runtime(u64 partyIdx, CommPkg& comm) { init(partyIdx, comm); }

  ~Sh4Runtime() {
    if (mTasks.size()) {
      std::cout << "~~~~~~~~~~~~~~~~ Runtime not empty!!! ~~~~~~~~~~~~~~~~"
                << std::endl;
    }
  }

  bool mPrint = false;
  u64 mPartyIdx = -1;
  CommPkg mComm;
  bool mIsActive = false;

  void init(u64 partyIdx, CommPkg& comm) {
    mPartyIdx = partyIdx;
    mComm = comm;

    mTasks.reserve(64);
    mNullTask.mRuntime = this;
    mNullTask.mIdx = -1;
  }

  const Sh4Task& noDependencies() const { return mNullTask; }
  operator Sh4Task() const { return noDependencies(); }

  Sh4Task addTask(span<Sh4Task> deps, Sh4Task::RoundFunc&& func,
                  std::string&& name);

  Sh4Task addTask(span<Sh4Task> deps, Sh4Task::ContinuationFunc&& func,
                  std::string&& name);

  Sh4Task addClosure(Sh4Task dep);

  Sh4Task addAnd(span<Sh4Task> deps, std::string&& name);

  void configureAnd(span<Sh4Task> deps, Sh4TaskBase& handle);
  Sh4Task configureTask(span<Sh4Task> deps, Sh4TaskBase& base);

  void removeClosure(Sh4TaskBase& closure);

  void runUntilTaskCompletes(Sh4Task task);
  void runNext();
  void runAll();
  void runOneRound();

  TaskDag mTasks;
  Sh4Task mNullTask;
};

} 

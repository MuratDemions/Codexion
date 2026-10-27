*This project has been created as part of the 42 curriculum by musipit.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads.
`number_of_coders` coders sit around a circular table, with one USB dongle
between each pair of neighbours (as many dongles as coders; a single coder gets
a single dongle). A coder must hold **both** adjacent dongles to compile. After
compiling it releases them, debugs, refactors and tries again. If a coder does
not start a compile within `time_to_burnout` ms of the start of its previous
compile (or of the simulation), it burns out and the simulation stops. The
simulation also stops once every coder has compiled `number_of_compiles_required`
times.

Goals: no deadlock, no starvation, a mandatory dongle cooldown, fair arbitration
(`fifo` or `edf`), burnout detected and logged within 10 ms, and clean,
non-interleaved logs.

## Instructions

```
make                # builds ./codexion (-Wall -Wextra -Werror -pthread)
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All times are in milliseconds. `scheduler` is exactly `fifo` or `edf`.
Examples:

```
./codexion 5 800 200 100 100 5 10 edf
./codexion 1 300 100 100 100 3 10 fifo     # lone coder: burns out at 300 ms
```

Invalid input (negative numbers, non-integers, overflow, wrong argument count,
unknown scheduler, `number_of_coders`/`time_to_burnout`/`time_to_compile`/
`number_of_compiles_required` equal to 0) is rejected with a message on stderr
and exit code 1. Rules: `make`, `make clean`, `make fclean`, `make re`.

Source layout: `src/` (`main`, `parse`, `init*`, `run`, `coder`, `monitor`,
`dongle*`, `heap*`, `log`, `time_utils`, `cleanup`) and `include/codexion.h`.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions).** A coder never holds one dongle
  while waiting for the other: it takes both dongles in one atomic step or none
  (no *hold and wait*). All multi-mutex acquisitions lock dongle mutexes in
  ascending index order, so mutex-level circular wait is impossible. Requests are
  enqueued in both dongles atomically and compared with one global total order
  (ticket / deadline, then coder id), so no cycle of "waits-for" coders can form.
- **Starvation prevention.** Each dongle owns a priority queue (binary heap).
  With `fifo` the key is an arrival ticket; with `edf` it is
  `last_compile_start + time_to_burnout`, ties broken by the lower coder id
  (deterministic). A waiting coder is only held back by a higher-priority
  neighbour that could actually run right now, so the most urgent request is
  always served first and dongles are not left idle needlessly.
- **Cooldown handling.** Releasing a dongle stores `free_at = now + dongle_cooldown`.
  A coder only claims a dongle pair when `now >= free_at` for both and otherwise
  sleeps with `pthread_cond_timedwait` until that exact time.
- **Precise burnout detection.** A dedicated monitor thread polls every 0.5 ms,
  compares `now - last_compile_start` with `time_to_burnout` and prints the
  burnout line (typically 1 ms after the real deadline, well within 10 ms).
  Sleeps used by coders are sliced (0.5 ms) and abort as soon as the stop flag is set.
- **Log serialization.** One `log` mutex protects `printf`; the stop flag is
  re-checked under that mutex so no line is printed after the burnout line.
- **Lone coder.** One dongle, so compiling is impossible: the coder takes it,
  waits and burns out at `time_to_burnout`.
- **Clean shutdown.** The stop flag wakes every sleeping coder; all threads are
  joined and every mutex, condition variable and heap is destroyed/freed.

## Thread synchronization mechanisms

- `pthread_mutex_t` per dongle: protects `in_use`, `free_at`, `ticket` and its
  priority queue. Without it two coders could take the same dongle (race on
  `in_use`).
- `pthread_mutex_t state`: protects the `stop` flag, each coder's
  `last_start` and `compiles` counter (read by the monitor, written by coders).
  It is a leaf lock (never held while taking another lock).
- `pthread_mutex_t log`: serializes output.
- `pthread_mutex_t wait_mutex` + `pthread_cond_t wake` per coder: a coder that
  cannot get both dongles sleeps on its own condition variable. Whoever changes
  dongle state (a coder releasing or claiming dongles) signals the neighbours
  that depend on it, and the monitor broadcasts to everybody on stop. Because
  the notifier takes `wait_mutex` and the waiter checks its predicate while
  holding it, no wake-up can be lost.
- Lock order: `wait_mutex` -> dongle mutexes (ascending index) -> `state`;
  `log` -> `state`. No cycles.

Coder/monitor communication is only through the `state` mutex (coder writes
`last_start`/`compiles`, monitor reads them, monitor writes `stop`, coders read it).

## Resources

- `man pthread_create`, `pthread_mutex_lock`, `pthread_cond_timedwait`, `gettimeofday`
- Dijkstra, *Hierarchical ordering of sequential processes* (dining philosophers)
- Coffman et al., *System Deadlocks* (1971)
- Liu & Layland, *Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment* (EDF)
- 42 Philosophers subject and common concurrency tutorials

**AI usage:** <describe honestly how you used AI, e.g. design discussion of the
atomic-acquisition strategy, generation of a first draft that you then reviewed,
tested (TSAN/ASAN) and can fully explain>.

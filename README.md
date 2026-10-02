*This project has been created as part of the 42 curriculum by Yande-ol.*

# Codexion

## Description

Codexion is a C concurrency project inspired by the Dining Philosophers problem. Several coder threads share a circular set of USB dongles. A coder must acquire both adjacent dongles before compiling, then releases them, debugs, refactors, and tries again.

The simulation must prevent deadlocks, avoid starvation, respect dongle cooldowns, detect burnout deadlines, serialize logs, and stop when a coder burns out or every coder reaches the required number of compilations.

## Instructions

### Compilation

```sh
make
```

The Makefile uses `cc` with `-Wall -Wextra -Werror -pthread`. Other available targets are:

```sh
make clean
make fclean
make re
```

### Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown fifo|edf
```

All numeric arguments are expressed in milliseconds except the number of coders and the compilation count. The scheduler must be exactly `fifo` or `edf`.

Example:

```sh
./codexion 4 1000 200 100 100 3 10 edf
```

## Technical choices

- Each coder is represented by one POSIX thread.
- There is one dongle per coder, connected circularly between neighboring coders.
- Each dongle owns a custom min-heap of waiting requests.
- FIFO prioritizes the earliest request time.
- EDF prioritizes the earliest deadline, calculated from the last compile start plus `time_to_burnout`.
- Equal priorities are resolved by the lowest coder ID.
- Coder and dongle memory is allocated dynamically and released during cleanup.

## Blocking cases handled

### Deadlock prevention

Coders acquire their two dongles in ascending dongle-ID order. This removes the circular-wait condition from the classic Coffman deadlock conditions.

### Starvation and fair arbitration

Each dongle has its own priority queue. Requests are granted according to FIFO or EDF instead of depending on thread scheduling order. The coder ID tie-breaker makes equal priorities deterministic.

### Dongle cooldown

After release, `last_released_time` records the release timestamp. A new owner waits until `dongle_cooldown` milliseconds have elapsed.

### Burnout detection

A separate monitor thread checks each coder's last compile start. If the deadline is reached, it prints the burnout message and stops the simulation.

### Completion condition

The monitor also checks whether every coder reached `number_of_compiles_required`. When that happens, it stops the simulation normally.

### Log serialization

`log_mutex` protects every status message so output from concurrent threads cannot interleave.

## Thread synchronization mechanisms

### `pthread_mutex_t`

Each dongle mutex protects `is_in_use`, `last_released_time`, and the dongle queue. Each coder has a mutex protecting `last_compile_start` and `compiles_count`. A global stop mutex protects `sim_stopped`, while a global log mutex protects output.

For example, the monitor locks a coder's mutex before reading its last compile timestamp, and the coder locks the same mutex before updating that timestamp. This prevents a data race.

### `pthread_cond_t`

Every dongle has a condition variable. A coder waits while the dongle is busy or while another request has higher priority. Releasing a dongle broadcasts to waiting coders so they can recheck the queue safely.

### Thread-safe communication

The monitor and coder threads communicate through the protected `sim_stopped` flag. Coders periodically check the flag during their waits, and the monitor sets it when burnout or completion occurs.

## Project structure

- `includes/`: shared types, constants, and public function prototypes.
- `src/parsing/`: command-line validation and conversion.
- `src/heap/`: custom FIFO/EDF min-heap implementation.
- `src/sync/`: resource initialization, cleanup, acquisition, release, and synchronization.
- `src/simulation/`: coder behavior and simulation startup.
- `src/monitor/`: burnout and completion monitoring.
- `src/utils/`: time, sleeping, logging, and simulation-state helpers.

## Resources

- POSIX Threads documentation: <https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html>
- POSIX mutex documentation: <https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html>
- POSIX condition variables: <https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html>
- The Linux man-pages documentation for `gettimeofday`, `usleep`, and pthread synchronization.
- The Dining Philosophers problem and Coffman deadlock conditions were used as conceptual references.

AI was used to help review the project structure, compare implementation behavior with the assignment subject, organize technical documentation, and identify test scenarios. All implementation decisions and generated suggestions were reviewed against the source code and tested locally.

*This project has been created as part of the 42 curriculum by asato.*

## Description
This is an implementation of the 42 School **Philosophers** project (Dining Philosophers problem). The program simulates philosophers who must alternate between thinking, eating, and sleeping while sharing forks (mutexes) and respecting timing constraints.

The goal is to practice concurrent programming with **POSIX threads** and **mutex-based synchronization**, ensuring correct behavior (no data races, correct stopping conditions, clean shutdown, and consistent logging).

## Instructions

### Prerequisites
- Unix-like OS (Linux/macOS)
- `cc` (clang/gcc) and `make`
- `pthread` support (provided by default on most Unix-like systems)

### Build
From the repository root:
```sh
make
```
This produces the executable:
- `./philo`

Other useful targets:
```sh
make clean
make fclean
make re
```

### Run
Usage (as implemented in this repository):
```sh
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```
All arguments must be **positive integers**.

Examples:
```sh
# Run without an "eat count" limit (simulation ends when a philosopher dies)
./philo 5 800 200 200

# Run until each philosopher has eaten at least N times
./philo 5 800 200 200 7
```

### Output
The program prints timestamped status lines describing philosopher actions, such as:
- `has taken a fork`
- `is eating`
- `is sleeping`
- `is thinking`
- `died`

## Resources

### Dining Philosophers / Concurrency references
- The Dining Philosophers problem (concept and common solution strategies)
- POSIX Threads (pthreads) overview and API reference
  - `pthread_create`, `pthread_join`
  - mutexes: `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`
- General concurrency guidance (deadlocks, starvation, lock ordering, logging from multiple threads)

### AI usage
AI was used to:
- Draft and edit this `README.md` (structure, English wording, and a concise explanation of the project goal).
- Infer the build/run interface by reading the repository’s `Makefile` and `includes/philos.h` usage string.

AI was not used to:
- Implement the project’s synchronization logic, thread routines, or core C source code (unless explicitly stated elsewhere by the author).

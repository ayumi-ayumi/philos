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
All arguments must be **positive integers**. <br>
[number_of_times_each_philosopher_must_eat] is optional argument.

Examples:
```sh
# Run without an "eat count" limit (simulation ends when a philosopher dies)
./philo 5 800 200 200

# Run until each philosopher has eaten at least N times
./philo 5 800 200 200 7
```

### Output
The program prints timestamped status lines describing philosopher actions, such as:
- `timestamp_in_ms X has taken a fork`
- `timestamp_in_ms X is eating`
- `timestamp_in_ms X is sleeping`
- `timestamp_in_ms X is thinking`
- `timestamp_in_ms X died`

### Leak check and test cases
If you want to check memory leaks, use:

```sh
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./philo
```

Test cases for the philosophers program:

```sh
./philo 5 800 200 200        # No one should die
./philo 5 600 150 150        # No one should die
./philo 4 410 200 200        # No one should die
./philo 100 800 200 200      # No one should die
./philo 105 800 200 200      # No one should die
./philo 200 800 200 200      # No one should die

./philo 1 800 200 200        # One philo should die
./philo 4 310 200 100        # One philo should die
./philo 4 200 205 200        # One philo should die

./philo 5 600 200 abc        # It should error and not run (no crashing)
./philo 600 200 200          # It should error and not run (no crashing)
./philo -5 600 200 200       # It should error and not run (no crashing)
./philo 4 -5 200 200         # It should error and not run (no crashing)
./philo 4 600 -5 200         # It should error and not run (no crashing)
./philo 4 600 200 -5         # It should error and not run (no crashing)
./philo 4 600 200 200 -5     # It should error and not run (no crashing)
```

## Resources
- [philosophers-visualizer](https://github.com/nafuka11/philosophers-visualizer)
- [philosopers playlist on YouTube](https://youtube.com/playlist?list=PLGU1kcPKHMKi41Py2kqxdvqYE3M9VhCHe&si=fPHN33C_dbVwci8P)
- POSIX threads and mutex man pages:
  - [`pthread_create(3)`](https://man7.org/linux/man-pages/man3/pthread_create.3.html)
  - [`pthread_join(3)`](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
  - [`pthread_mutex_init(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_init.3.html)
  - [`pthread_mutex_lock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
  - [`pthread_mutex_unlock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)

### Dining Philosophers / Concurrency references
- The Dining Philosophers problem (concept and common solution strategies)
- POSIX Threads (pthreads) overview and API reference
  - `pthread_create`, `pthread_join`
  - mutexes: `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`
- General concurrency guidance (deadlocks, starvation, lock ordering, logging from multiple threads)

### AI usage
AI was used to:
- Draft this `README.md` (structure and English wording).
- Suggest clearer naming for functions and variables based on common best practices.

# Dining Philosophers

Two C implementations of the dining philosophers concurrency problem, developed for the 42 curriculum. Philosophers share limited forks while alternating between eating, sleeping, and thinking.

## Implementations

| Directory | Approach | Executable |
| --- | --- | --- |
| [philo](philo) | POSIX threads and mutexes | `philo` |
| [philo_bonus](philo_bonus) | Processes and semaphores | `philo_bonus` |

Both versions include timing, status output, argument parsing, and monitoring code.

## Build

Requires GCC, Make, and POSIX thread/process/semaphore APIs. From the repository root:

```sh
make -C philo
make -B -C philo_bonus
```

The bonus folder includes tracked build artifacts, so `-B` forces a rebuild for the current machine. Its Makefile also compiles the shared `philo/time_func.c` source. Each directory provides `clean`, `fclean`, and `re` targets.

## Usage

Both programs take exactly four numeric arguments:

```text
number_of_philosophers time_to_die time_to_eat time_to_sleep
```

Times are in milliseconds. For example:

```sh
./philo/philo 5 800 200 200
./philo_bonus/philo_bonus 5 800 200 200
```

Run one simulation at a time. Output describes philosopher activity with timestamps and identifiers. These versions do not accept the optional meal-count argument found in some versions of the project subject. Timing and scheduling depend on the host operating system.

## Concepts practiced

Shared resources, mutual exclusion, process coordination, semaphores, race conditions, deadlock risks, timing, and resource cleanup.

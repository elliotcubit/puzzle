# Puzzle

Countdown numbers game. It looks like this when run:

```
$ ./main
Get as close as possible to 295 using the numbers 50 50 8 3 8 3 3 3 (Using only + - * / and parentheses). You have 45 seconds.
>>> 50 * (3+3) - 8 + 3
 = 295! Great work - it took 19.16 seconds!

$ ./main
Get as close as possible to 538 using the numbers 50 25 5 1 8 1 12 (Using only + - * / and parentheses). You have 45 seconds.
>>> 50 + 25
 = 75 (distance: 463) in 1.85 seconds
>>> 50 * 8
 = 400 (distance: 138) in 5.04 seconds
>>> 50 * 12
 = 600 (distance: 62) in 6.93 seconds
>>> 50 * 12 + (25 / 5)
 = 605 (distance: 67) in 11.10 seconds
>>> 50 *
Times up!
Best distance: 62 (in 6.93 seconds). Not good.
```

Every puzzle is always exactly solvable.

# Usage

```
$ make
$ ./main
$ ./main -t [timeout in seconds (default: 45). Zero means no timeout.]
```

# License
GPLv3

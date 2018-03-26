psstop(1) -- memory snapshot of the current processes based on /proc
====================================================================


## SYNOPSIS

`/usr/bin/psstop`


## DESCRIPTION


The Linux kernel exposes memory usage information about running
processes in the `procfs` filesystem. This information can be retrieved
by reading several files that can simply be read as ordinary text files
and offer various different views of the usage of memory by processes.

Because memory consumption between applications and processes is often
shared between threads, there are different ways to describe how much
memory a specific thread or process is actually using. Depending on
what kind of memory consumption you want to know about, you may have
to determine the right memory consumption metric.

The PSS (Proportional set size) usage of a process is a method to
describe memory consumption by a process that accounts for it's
proportion of shared memory consumption between itself and other
processes that also cover shared memory resources. These shared
resources could be any type of shared memory that is shared between
processes, like buffers or caches, or library code.


## SYNTAX

`psstop` is meant to be directly executed from the command line:

```
    psstop

    [...]

    0 Kb: (sd-pam) (24051)
    1122 Kb: tmux: server (31523)
    4 Kb: bash (31524)
    4 Kb: bash (31525)
    4 Kb: vim (31684)
    Total is 14958Kb (174 processes)
```


## OPTIONS

The following options are understood:

 * `-c`:

   Display the basename of the executable rather than the name
   of the process or thread.

 * `-p processname`:

   Display the PSS usage of processes that match the provided process
   name.

 * `-h`:

   Display the program's available options.


## EXIT STATUS

On success, 0 is returned, a non-zero failure code otherwise.


## FILES

`/proc/*/smaps`


## SEE ALSO

 * ps(1)

 * free(1)

 * `https://en.wikipedia.org/wiki/Proportional_set_size`

 * `procfs(5)`


## BUGS

Please report bugs to: `dev@lists.clearlinux.org`


## COPYRIGHT

 * This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3 or later of the License

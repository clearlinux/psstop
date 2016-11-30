
# PSSTOP

The main purpose of a computer system is to execute programs. These programs,
together with the data they access, must be at least partially in main memory
during execution. As a developer and user are necessary to know the amount of
memory consumed by every process in execution. PSSTOP helps to get this
information in an accurate way.

PSSTOP gets the memory of each process from:

    /proc/PID/smaps

The /proc/PID/smaps is an extension based on maps, showing the memory
consumption for each of the process's mappings. For each of mappings there
is a series of lines such as the following:

    Size:               1084 kB
    Rss:                 892 kB
    Pss:                 374 kB
    Shared_Clean:        892 kB
    Shared_Dirty:          0 kB
    Private_Clean:         0 kB
    Private_Dirty:         0 kB
    Referenced:          892 kB
    Anonymous:             0 kB
    AnonHugePages:         0 kB
    ShmemPmdMapped:        0 kB
    Shared_Hugetlb:        0 kB
    Private_Hugetlb:       0 kB
    Swap:                  0 kB
    SwapPss:               0 kB
    KernelPageSize:        4 kB
    MMUPageSize:           4 kB
    Locked:                0 kB

PPSTOP tracks the amount of memory from the process' proportional share of this
mapping (Pss) and the proportional swap share of the mapping (SwapPss).

Others tools like ps doesn't give an acurate detail of which processes are
consuming the memory of our system . Depending on how you look at it, ps is not
reporting the real memory usage of processes. What it is really doing is
showing how much real memory each process would take up if it were the only
process running.

## Installation

    autoreconf --install

    make

    make install

## Usage

    psstop

    [...]

    0 Kb: (sd-pam) (24051)
    1122 Kb: tmux: server (31523)
    4 Kb: bash (31524)
    4 Kb: bash (31525)
    4 Kb: vim (31684)
    Total is 14958Kb (174 processes)

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## Authors

    Arjan van de Ven <arjan@linux.intel.com>

## License

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 or later of the License


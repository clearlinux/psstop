psstop(1) -- memory snapshot of the current processes based on /proc
====================================================================

## SYNOPSIS

`psstop`

## DESCRIPTION


PPSTOP tracks the amount of memory from the process' proportional share of this
mapping (Pss) and the proportional swap share of the mapping (SwapPss). The
momory information came from smaps (/proc/PID/smaps). The /proc/PID/smaps is an
extension based on maps, showing the memory consumption for each of the
process's mappings.

## SYNTAX

`psstop` is meant to be directly executed from the
command line as :

    psstop

    [...]

    0 Kb: (sd-pam) (24051)
    1122 Kb: tmux: server (31523)
    4 Kb: bash (31524)
    4 Kb: bash (31525)
    4 Kb: vim (31684)
    Total is 14958Kb (174 processes)

## EXIT STATUS

On success, 0 is returned, a non-zero failure code otherwise.

## COPYRIGHT

 * This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 or later of the License

## SEE ALSO

 * ps(1)

 * free(1)



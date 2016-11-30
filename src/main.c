/*
 *    Clear Linux Project for Intel Architecture memory tracking 
 *
 *      Copyright (C) 2014-2016 Intel Corporation
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, version 3 or later of the License.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Authors:
 *         Arjan van de Ven <arjan@linux.intel.com>
 *
 */

#define _GNU_SOURCE
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <dirent.h>
#include <inttypes.h>

char *searchkey;
uint64_t total_PSS;
int total_proc;

struct process {
        int pid;
        char *name;
        uint64_t PSS_kb;
};

static void do_one_process(int pid)
{
        FILE *file;
        char buffer[4096];
        char filename[4096];
        struct process process;
        if (!pid)
                return;

        memset(&process, 0, sizeof(struct process));

        sprintf(filename, "/proc/%i/comm", pid);
        file = fopen(filename, "r");
        if (file) {
                char *c;
                buffer[0] = 0;
                if (fgets(buffer, 4096, file) != NULL) {
                        c = strchr(buffer, '\n');
                        if (c)
                                *c = 0;
                        process.name = strdup(buffer);
                }
                fclose(file);
        }
        if (process.name && searchkey && strstr(process.name, searchkey) == NULL)
                return;

        sprintf(filename, "/proc/%i/smaps", pid);
        file = fopen(filename, "r");
        if (file) {
                while (!feof(file)) {
                        buffer[0] = 0;
                        if (fgets(buffer, 4096, file) != NULL) {
                                if (strlen(buffer) == 0)
                                        break;
                                if (strstr(buffer, "Pss:") == NULL)
                                        continue;
                                process.PSS_kb += strtoull(buffer + 4, NULL, 10);
                        }
                }
                fclose(file);
        }

        printf("%" PRIu64 " Kb: %s (%i) \n", process.PSS_kb, process.name, pid);
        total_PSS += process.PSS_kb;
        total_proc++;
        free(process.name);
}

int main(int argc, char **argv)
{
        DIR *dir;
        struct dirent *entry;

        if (argc > 1) {
                searchkey = strdup(argv[1]);
        }

        dir = opendir("/proc");
        while (dir) {
                int pid;
                entry = readdir(dir);
                if (!entry)
                        break;
                if (entry->d_name[0] == '.')
                        continue;
                pid = strtoull(entry->d_name, NULL, 10);
                do_one_process(pid);
        }

        printf("Total is %" PRIu64 "Kb (%i processes)\n", total_PSS, total_proc);
        closedir(dir);
        free(searchkey);
        return EXIT_SUCCESS;
}

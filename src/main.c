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
 *         Victor Rodriguez <victor.rodriguez.bahena@intel.com>
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
#include <limits.h>

// TASK_COMM_LEN on Linux
#define MAX_COMM_LEN 16

char *searchkey;
uint64_t total_PSS;
int total_proc;
int use_cmdline = 0;


struct process {
        int pid;
        char *name;
        uint64_t PSS_kb;
};

struct process *allprocs=NULL;
int numproc=0;

int cmpfunc (const void * a, const void * b) {
    struct process *s_a = (struct process *)a;
    struct process *s_b = (struct process *)b;
    if ((s_a->PSS_kb < INT_MIN && s_a->PSS_kb > INT_MAX) ||
            (s_b->PSS_kb < INT_MIN && s_b->PSS_kb > INT_MAX) ){
        printf("Error sizes greater than 2^41 i.e. 2TB\n");
        exit(1);
    }
    return ( s_a->PSS_kb - s_b->PSS_kb );
}

void addnode(struct process *p)
{
    numproc++;
    allprocs = realloc(allprocs,numproc*sizeof(struct process));
    if (allprocs==NULL) {
        printf("Error reallocating memory!");
        free (allprocs);
        exit (1);
    }
    allprocs[numproc-1]=*p;
}

void print_list(){
    struct process *p = allprocs;
    for (int i=0 ; i<numproc; i++){
        printf("%-20s %-5i : %-5" PRIu64 "  Kb\n", p[i].name ,p[i].pid, p[i].PSS_kb);
    }
}

struct process do_one_process(int pid)
{
        FILE *file;
        char buffer[4096];
        char filename[4096];
        struct process process;

        memset(&process, 0, sizeof(struct process));

        if (use_cmdline) {
                sprintf(filename, "/proc/%i/cmdline", pid);
                file = fopen(filename, "r");
                if (file) {
                        char *c, *name;
                        buffer[0] = 0;
                        if (fgets(buffer, 4096, file) != NULL) {
                                name = buffer;
                                if ((c = strchr(buffer, '\n')))
                                        *c = 0;
                                if ((c = strchr(buffer, ' ')))
                                        *c = 0;
                                if ((c = strrchr(buffer, '/')))
                                        name = c + 1;
                                if (strlen(name) > MAX_COMM_LEN)
                                        name[MAX_COMM_LEN] = 0;
                                process.name = strdup(name);
                        }
                        fclose(file);
                }
        }

        if (!process.name) {
                sprintf(filename, "/proc/%i/comm", pid);
                file = fopen(filename, "r");
                if (file) {
                        char *c;
                        buffer[0] = 0;
                        if (fgets(buffer, 4096, file) != NULL) {
                                if ((c = strchr(buffer, '\n')))
                                        *c = 0;
                                process.name = strdup(buffer);
                        }
                        fclose(file);
                }
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
        process.pid = pid;
        return process;
}

void print_help(){
    printf("\nHelp : \n");
    printf("    -h : Print this help\n");
    printf("    -p : Process name to measure memory usage\n");
    printf("    -c : Use cmdline for process name\n");
    printf(" \n");
}

int main(int argc, char **argv)
{

    DIR *dir;
    struct dirent *entry;
    struct process process;

    int c = 0;

    while ((c = getopt (argc, argv, "chp:")) != -1){
        switch (c) {
            case 'c':
                use_cmdline = 1;
                break;
            case 'p':
                searchkey = strdup(optarg);
                break;
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            default:
                print_help();
                abort ();
        }
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
                if (pid){
                    process = do_one_process(pid);
                    if (process.pid == pid){
                        addnode(&process);
                        total_PSS += process.PSS_kb;
                        total_proc++;
                    }
                }
        }

        printf("\n%-20s %-5s : %-5s \n\n","Process Name", "PID", "Size in KB");
        qsort(allprocs,total_proc,sizeof(process), cmpfunc);
        print_list();
        printf("\nTotal is %" PRIu64 "Kb (%i processes)\n", total_PSS, total_proc);
        closedir(dir);
        free(searchkey);
        return EXIT_SUCCESS;
}

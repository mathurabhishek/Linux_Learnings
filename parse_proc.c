/*

If we cat /proc/29519/stat, we get this info:

29519 (vim) S 5997 29519 5997 34835 29519 24576 1275 0 47 0 5 0 0 0 20 0 2 0 49083340 188043264 3718 18446744073709551615 4194304 6665820 140737488349264 140737488347024 140737280970147 0 0 12288 1837256447 18446744073709551615 0 0 17 3 0 0 21 0 0 8764120 8861948 8925184 140737488349925 140737488349929 140737488349929 140737488351211 0

What do all those numbers represent? The answer is in man proc, in the section called /proc/[pid]/stat. From this we see the first four things are:

pid %d

(1) The process ID.

comm %s

(2) The filename of the executable, in parentheses. This is visible whether or not the executable is swapped out.

state %c

(3) One character from the string "RSDZTW" where R is running, S is sleeping in an interruptible wait, D is waiting in uninterruptible disk sleep, Z is zombie, T is traced or stopped (on a signal), and W is paging.

ppid %d

(4) The PID of the parent.

With this knowledge we can parse it out with fscanf(f, "%d %s %c %d", ...):

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(int argc, char **argv) {
    int pid;
    sscanf(argv[1], "%d", &pid);
    printf("pid = %d\n", pid);

    char filename[1000];
    sprintf(filename, "/proc/%d/stat", pid);
    FILE *f = fopen(filename, "r");

    int unused;
    char comm[1000];
    char state;
    int ppid;
    fscanf(f, "%d %s %c %d", &unused, comm, &state, &ppid);
    printf("comm = %s\n", comm);
    printf("state = %c\n", state);
    printf("parent pid = %d\n", ppid);
    fclose(f);
}
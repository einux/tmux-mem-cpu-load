#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // sleep
#include <string.h>

char *cpu_string()
{
    char *str = NULL;
    str = (char *)malloc(sizeof("CPU"));
    strcpy(str, "CPU");
    return str;
}

unsigned int get_mem_stat(char *line, size_t len)
{
    char *str = NULL, *p1 = line, *p2 = line + len; /* p2 now points to '\n' */
    while ((*p1) != ' ')
        p1++;
    while ((*p1) == ' ')
        p1++;

    while ((*p2) != ' ')
        p2--;

    str = (char *)malloc((size_t)(p2 - p1 + 1));
    strncpy(str, p1, p2 - p1);
    str[p2-p1] = '\0';
    return (unsigned int)atoi(str);
}

char *mem_string()
{
    unsigned int total_mem, unused_mem, used_mem;
    int i;
    char *line = NULL;
    char *ret = NULL;
    size_t len = 0;
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    getline(&line, &len, fp);

    used_mem = total_mem = get_mem_stat(line, len);

    for (i = 0 ; i < 3; i++) {
        getline(&line, &len, fp);
        unused_mem = get_mem_stat(line, len);
        used_mem -= unused_mem;
    }
    ret = (char *)malloc((size_t)20);
    sprintf(ret, "%d/%dMB", used_mem / 1024, total_mem / 1024);
    
    fclose(fp);
    return ret;
}

char * load_string()
{
    char *line = NULL;
    size_t len = 0;
    FILE *fp = fopen("/proc/loadavg", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    
    getline(&line, &len, fp);
    line[14] = '\0';
    fclose(fp);
    return line;
}

int main(int argc, char** argv)
{
    unsigned int cpu_usage_delay = 900000;
    unsigned int graph_lines = 10;
    char *p_cpu_string;
    char *p_mem_string;
    char *p_load_string;
    
    if (argc > 1)
        cpu_usage_delay = (unsigned int)atoi(argv[1]) * 1000000 - 100000;
    if (argc > 2)
        graph_lines = (unsigned int)atoi(argv[2]);
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [tmux_status_interval(seconds)] [graph lines]\n", argv[0]);
        return 1;
    }
    /* printf("%s %s %s", mem_string(), cpu_string(cpu_usage_delay, graph_lines), load_string()); */
    p_cpu_string = cpu_string();
    p_mem_string = mem_string();
    p_load_string = load_string();
    printf("%s|%s|%s", p_cpu_string, p_mem_string, p_load_string);

    free(p_cpu_string);
    free(p_mem_string);
    free(p_load_string);
    return 0;
}


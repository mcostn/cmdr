#ifndef CMDR_H
#define CMDR_H
struct cmdr_state
{
    int argc;
    char **argv;
};

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
extern int cmdr_init(struct cmdr_state *state, int argc, char **argv);
extern int cmdr_end(struct cmdr_state *state);
extern int cmdr_find(struct cmdr_state *state, const char *str, int *out_idx);
extern int cmdr_command(struct cmdr_state *state, const char *cmd);
extern int cmdr_flag(struct cmdr_state *state, const char *flag);
#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef CMDR_IMPLEMENTATION
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int cmdr_init(struct cmdr_state *state, int argc, char **argv)
{
    if (state == NULL || argc <= 0 || argv == NULL)
        return 1;

    state->argc = argc;
    state->argv = argv;
    return 0;
}

int cmdr_end(struct cmdr_state *state)
{
    if (state == NULL)
        return 1;

    state->argc = 0;
    state->argv = NULL;
    return 0;
}

int cmdr_find(struct cmdr_state *state, const char *str, int *out_idx)
{
    if (state == NULL || str == NULL)
        return 0;

    for (int i = 0; i < state->argc; i++) {
        if (strcmp(state->argv[i], str) == 0) {
            if (out_idx != NULL)
                *out_idx = i;
            return 1;
        }
    }

    return 0;
}

int cmdr_command(struct cmdr_state *state, const char *cmd)
{
    if (state == NULL || cmd == NULL)
        return 0;
    return cmdr_find(state, cmd, NULL);
}

int cmdr_flag(struct cmdr_state *state, const char *flag)
{
    if (state == NULL || flag == NULL)
        return 0;
    return cmdr_find(state, flag, NULL);
}
#endif // CMDR_IMPLEMENTATION
#endif // CMDR_H

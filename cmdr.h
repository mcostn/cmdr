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
extern int cmdr_confirm(struct cmdr_state *state, const char *msg);
extern int cmdr_confirm_default(struct cmdr_state *state, const char *msg, int def);
#ifdef __cplusplus
}
#endif // __cplusplus

#ifdef CMDR_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

#define CMDR_ASSERT_MSG(cond, msg, ...) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "cmdr: " msg "\n", __VA_ARGS__); \
            exit(EXIT_FAILURE); \
        } \
    } while(0);

#define CMDR_ASSERT(cond) \
    CMDR_ASSERT_MSG(cond, "assert failed: %s", #cond)

static int cmdr_internal_valid_command(const char *command);
static int cmdr_internal_valid_flag(const char *flag);

int cmdr_init(struct cmdr_state *state, int argc, char **argv)
{
    CMDR_ASSERT(state != NULL);
    CMDR_ASSERT(argc > 0);
    CMDR_ASSERT(argv != NULL);

    state->argc = argc;
    state->argv = argv;
    return 0;
}

int cmdr_end(struct cmdr_state *state)
{
    CMDR_ASSERT(state != NULL);

    state->argc = 0;
    state->argv = NULL;
    return 0;
}

int cmdr_find(struct cmdr_state *state, const char *str, int *out_idx)
{
    CMDR_ASSERT(state != NULL);
    CMDR_ASSERT(str != NULL);

    for (int i = 1; i < state->argc; i++) {
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
    CMDR_ASSERT(state != NULL);
    CMDR_ASSERT(cmd != NULL);
    CMDR_ASSERT_MSG(cmdr_internal_valid_command(cmd),
                    "invalid command name: %s",
                    cmd);
    return cmdr_find(state, cmd, NULL);
}

int cmdr_flag(struct cmdr_state *state, const char *flag)
{
    CMDR_ASSERT(state != NULL);
    CMDR_ASSERT(flag != NULL);
    CMDR_ASSERT_MSG(cmdr_internal_valid_flag(flag),
                    "invalid flag name: %s",
                    flag);
    return cmdr_find(state, flag, NULL);
}

int cmdr_confirm(struct cmdr_state *state, const char *msg)
{
    CMDR_ASSERT(state != NULL);
    CMDR_ASSERT(msg != NULL);

    return cmdr_config_default(state, msg, 1);
}

int cmdr_confirm_default(struct cmdr_state *state, const char *msg, int def)
{
    CMDR_ASSERT(state != NULL);
    CMDR_ASSERT(msg != NULL);

    char yes_ch = 'Y';
    char no_ch = 'n';
    if (def == 0) {
        yes_ch = tolower(yes_ch);
        no_ch = toupper(no_ch);
    }

    printf("%s [%c/%c] ", msg, yes_ch, no_ch);
    fflush(stdout);

    char buf[32];
    if (fgets(buf, sizeof(buf), stdin) == NULL)
        return !!def;

    buf[strcspn(buf, "\n")] = '\0';

    // Trim leading whitespace
    while (isspace(*buf))
        buf++;

    // Trim trailing whitespace
    char *end = buf + strlen(buf);
    while (end > buf && isspace((unsigned char)end[- 1]))
        end--;
    *end = '\0';

    for (char *ch = buf; *ch; ++ch)
        *ch = (char)tolower(*ch);

    if (*buf == '\0')
        return !!def;

    if (strcmp(buf, "y") == 0 || strcmp(buf , "yes") == 0)
        return 1;
    else if (strcmp(buf, "n") == 0 || strcmp(buf , "no") == 0)
        return 0;

    return 0;
}

static int cmdr_internal_valid_command(const char *command)
{
    CMDR_ASSERT(command != NULL);

    if (command[0] == '\0' || command[0] == '-')
        return 0;

    const char *ch = command;
    while (*ch != '\0') {
        if (isspace(*ch) || *ch == '=')
            return 0;

        ch++;
    }

    return 1;
}

static int cmdr_internal_valid_flag(const char *flag)
{
    CMDR_ASSERT(flag != NULL);

    if (flag[0] != '-')
        return 0;
    if (flag[1] == '\0' ||
        (flag[1] == '-' && flag[2] == '\0'))
        return 0;
    if (flag[1] == '-' && flag[2] == '-')
        return 0;


    const char *ch = flag;
    while (*ch != '\0') {
        if (isspace(*ch) || *ch == '=')
            return 0;

        ch++;
    }

    return 1;
}
#endif // CMDR_IMPLEMENTATION
#endif // CMDR_H

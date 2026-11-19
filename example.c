#define CMDR_IMPLEMENTATION
#include "cmdr.h"

#include <stdio.h>

static void usage()
{
    printf("usage: example <command> [flags]\n\n"
           "commands:\n"
           "  hello       print a hello message\n"
           "  goodbye     print a goodbye message\n"
           "  ask         prompt a yes/no question (default: yes)\n"
           "  ask-no      prompt a yes/no question (default: no)\n\n"
           "flags:\n"
           "  -a          example short flag\n"
           "  --long-name example long flag\n"
           "  --argv-pos  print its argv position\n"
           "  --str       string value\n"
           "  --long      long value\n"
           "  --int       int value\n"
           "  -h, --help  show this message\n\n");
}

int main(int argc, char **argv)
{
    struct cmdr_state cmdr;
    cmdr_init(&cmdr, argc, argv);

    if (argc == 1 || cmdr_flag(&cmdr, "-h") || cmdr_flag(&cmdr, "--help")) {
        usage();
        cmdr_end(&cmdr);
        return 0;
    }

    if (cmdr_command(&cmdr, "hello")) {
        printf("command: hello\n");
    } else if (cmdr_command(&cmdr, "goodbye")) {
        printf("command: goodbye\n");
    } else if (cmdr_command(&cmdr, "ask")) {
        if (cmdr_confirm(&cmdr, "Continue?"))
            printf("answered: yes\n");
        else
            printf("answered: no\n");
    } else if (cmdr_command(&cmdr, "ask-no")) {
        if (cmdr_confirm_default(&cmdr, "Continue?", 0))
            printf("answered: yes\n");
        else
            printf("answered: no\n");
    }


    if (cmdr_flag(&cmdr, "-a"))
        printf("flag: -a\n");
    if (cmdr_flag(&cmdr, "--long-name"))
        printf("flag: --long-name\n");

    const char *str;
    if (cmdr_flag_str(&cmdr, "--str", &str))
        printf("flag: --str, value is %s\n", str);

    long _long;
    if (cmdr_flag_long(&cmdr, "--long", &_long))
        printf("flag: --long, value is %ld\n", _long);

    int integer;
    if (cmdr_flag_int(&cmdr, "--int", &integer))
        printf("flag: --int, value is %d\n", integer);

    int idx;
    if (cmdr_find(&cmdr, "--argv-pos", &idx))
        printf("--argv-pos is at argv[%d]\n", idx);

    cmdr_end(&cmdr);
    return 0;
}

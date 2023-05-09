// ctx.c
/*
    console program to
    save multiple items from clipboard
    set to clipboard from multiple saved items
    Makes heavy use of the myc.h library:
        readfile
        open_for_write, open_for_append
        strcon
        clr_fg color codes sturcture
        contains
        cbcopy
        cbpaste
        list_def
        list_split
        list_display
        see: https://github.com/MLeidel/myc
*/
#include <myc.h>
#define BUFSIZE 4096

char strbuf[BUFSIZE] = {'\0'};
char path[1024] = {'\0'};
char delbuf[BUFSIZE] = {'\0'};
FILE * ctxfile;
char flag = 'X';  // must be 'l', 'd', or 's'
char rsep[2] = {30, '\0'};  // record separator
char *msg = {"Usage: \n\
 ctx many-args will-save as one-string | \n\
 ctx s  paste to ctx.txt save file | \n\
 ctx d  delete all clips in ctx.txt save file | \n\
 ctx l  list saved clips to select for copy  \n\
 "};


void add_record(char *rec) {
    /* appends new record to TOP of file */
    char xbuf[BUFSIZE] = {'\0'};
    readfile (xbuf, "ctx.txt");
    ctxfile = open_for_write("ctx.txt");
    fprintf(ctxfile, "%s%s%s", rec, rsep, xbuf);
    fclose(ctxfile);
}

void main (int argc, char *argv[]) {

    // Change to data directory: /home/USER/.config/ctx/
    // data files are in this location
    sprintf(path, "%s/.config/ctx", getenv("HOME"));
    chdir(path);

    if (argc > 2) {
        strcon(strbuf, argv, " ", argc, 1);
        add_record(strbuf);
        printf("%s%s\n", clr_fg.cyan, "Saved from command-line:");
        printf("%s%s\n", clr_fg.green, strbuf);
        exit(EXIT_SUCCESS);
    }

    if (argc < 2) {
        printf("%s%s\n", clr_fg.cyan, msg);
        exit(EXIT_SUCCESS);
    }

//  now there should be just 1 argument
//  and it should be a flag: s, l, or d

//  or just a one word string
    if (strlen(argv[1]) > 1) {
        add_record(argv[1]);
        printf("%s%s\n", clr_fg.cyan, "Saved from command line arg 1:");
        printf("%s%s\n", clr_fg.green, argv[1]);
        exit(EXIT_SUCCESS);
    }

    // its a flag
    flag = argv[1][0];

    if (flag == 's') {
        cbpaste(strbuf);
        if (strlen(strbuf) > 1) {
            add_record(strbuf);
            printf("%s%s\n", clr_fg.cyan, "Saved from clipboard:");
            printf("%s%s\n", clr_fg.green, strbuf);
            exit(EXIT_SUCCESS);
        }
    }
    else if (flag == 'l') {
        int inx = 0;
        int rows = 0;
        readfile (strbuf, "ctx.txt");
        strbuf[strlen(strbuf)-2] = '\0';  // remove trailing delimiter

        rows = contains(strbuf, rsep) + 1;  // correct nbr columns
        list a = list_def(rows, BUFSIZE);
        int n = list_split(a, strbuf, rsep);
        printf("%s", clr_fg.yellow);
        list_display(a);
        printf("%sEnter number to copy /or/ Ctrl-c: ", clr_fg.dark_yellow);
        do {
            scanf("%d", &inx);
        } while (inx >= rows);
        cbcopy(a.item[inx]);
        printf("%s%s\n", clr_fg.green, a.item[inx]);
        printf("%s%s\n", clr_fg.cyan, "--->COPIED to system clipboard.");
    }
    else if (flag == 'd') {
        int inx = 0;
        int rows = 0;
        int counter = 0;
        readfile (strbuf, "ctx.txt");
        strbuf[strlen(strbuf)-2] = '\0';  // remove trailing delimiter

        rows = contains(strbuf, rsep) + 1;  // correct nbr columns
        list a = list_def(rows, BUFSIZE);
        int n = list_split(a, strbuf, rsep);
        printf("%s", clr_fg.yellow);
        list_display(a);
        printf("%sEnter number to delete /or/ 99 (all) /or/ Ctrl-c: ", clr_fg.dark_yellow);
        do {
            scanf("%d", &inx);
        } while (inx >= rows && inx != 99);
        if (inx <= n) {
            // delete specific record (inx)
            strcpy(delbuf, "");
            list_remove(a, inx);
            list_string(a, delbuf, rsep, false); //create the new ctx.txt delimited strings
            ctxfile = open_for_write("ctx.txt");
            fprintf(ctxfile,"%s", delbuf);
            fclose(ctxfile);
            printf("%s--->Deleted record index %d\n", clr_fg.cyan, inx);
        } else {
            printf("%s%s", clr_fg.dark_yellow, "Delete All Saved Clips? y|n ");
            scanf("%s", strbuf);
            if (equals(strbuf, "y")) {
                ctxfile = open_for_write("ctx.txt");
                fprintf(ctxfile, "");
                fclose(ctxfile);
                printf("%s--->Deleted all ctx clips\n", clr_fg.cyan);
            }
        }
    }
    else {
        // 1 argv exists but it is not a valid flag
        printf("%sInvalid flag arg [l | d | s]\n", clr_fg.red);
    }
}

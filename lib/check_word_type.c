#include "../globals.h"
#include "../config.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "../lib/registers.h"

int is_reserved_name(char *word)
{
    int i;
    /* check if the given word is not reserved register name */
    for (i = 0; i < 8; i++)
    {
        if (!strcmp(word, registers_list[i]))
            return TRUE;
    }

    /* check if the given word is not a reserved operation name */
    for (i = 0; i < 16; i++)
    {
        if (!strcmp(word, operations_list[i].name))
            return TRUE;
    }

    return FALSE;
}

int is_legal_label(char *token)
{
    int i;
    /* check if starts with an alphabet letter (upercase or lowercase) */
    if (!isalpha(token[0]))
    {
        printf("ERROR: illegal label name - must start with an alphabetic character.\n");
        return FALSE;
    }

    /* check if contains more than 31 characters */
    if (strlen(token) > MAX_LABEL_LENGTH + 1) /*+1 for the ':'*/
    {
        printf("ERROR: illegal label name - contains more than 31 characters\n");
        return FALSE;
    }

    /* check if contains only alphabet letters and digits */
    for (i = 1; i < strlen(token); i++)
    {

        if (!(isalpha(token[i]) || isdigit(token[i])))
        {
            printf("ERROR: illegal label name - name shold consist letters and digits only\n");
            return FALSE;
        }
    }

    /* check if label name is a reserved name */
    if (is_reserved_name(token))
    {
        printf("ERROR: illegal label name - %s is a reserved word\n", token);
        return FALSE;
    }

    return TRUE;
}

enum word_type check_word_type(char *token)
{
    int i = 0;
    char label_name[MAX_LABEL_LENGTH] = {'\0'};

    if (strcmp(token, ".data") == 0) /*the token is .data*/
        return DATA;

    if (strcmp(token, ".string") == 0) /*the token is .striny*/
        return STRING;

    if (strcmp(token, ".entry") == 0) /* if the word type is .entry*/
        return ENT;

    if (strcmp(token, ".extern") == 0) /* if the word type is .extern*/
        return EXT;

    while (i < 16)
    {
        if (strcmp(token, operations_list[i].name) == 0)
            /* checking what is the  name of the instruction */
            return INST;

        else
            i++;
    }

    strncpy(label_name, token, strlen(token) - 1);
    label_name[strlen(token)] = NULL_TERMINATOR;
    if (token[strlen(token) - 1] == ':' && is_legal_label(label_name))
        return LABEL; /*the token is label*/

    return ERROR_WORD;
}
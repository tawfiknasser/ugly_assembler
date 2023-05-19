#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char **build_words_array(char *line)
{
    char **words = NULL;
    int max_words = 0;
    int i = 0;
    char *word_start = NULL;
    int word_length = 0;
    int in_parentheses = 0;

    while (*line != '\0')
    {
        if (*line == '(')
        {
            in_parentheses = 1;
        }
        else if (*line == ')')
        {
            in_parentheses = 0;
        }

        if (!in_parentheses && (isspace(*line) || *line == ','))
        {
            if (word_start != NULL)
            {
                if (i >= max_words)
                {
                    max_words += 10;
                    words = realloc(words, max_words * sizeof(char *));
                }
                words[i] = malloc(word_length + 1);
                strncpy(words[i], word_start, word_length);
                words[i][word_length] = '\0';
                i++;
                word_start = NULL;
                word_length = 0;
            }

            if (*line == ',')
            {
                if (i >= max_words)
                {
                    max_words += 10;
                    words = realloc(words, max_words * sizeof(char *));
                }
                words[i] = malloc(2);
                strncpy(words[i], ",", 1);
                words[i][1] = '\0';
                i++;
            }
        }
        else
        {
            if (word_start == NULL)
            {
                word_start = line;
            }
            word_length++;
        }

        line++;
    }

    if (word_start != NULL)
    {
        if (i >= max_words)
        {
            max_words += 10;
            words = realloc(words, max_words * sizeof(char *));
        }
        words[i] = malloc(word_length + 1);
        strncpy(words[i], word_start, word_length);
        words[i][word_length] = '\0';
        i++;
    }

    if (i >= max_words)
    {
        max_words += 1;
        words = realloc(words, max_words * sizeof(char *));
    }
    words[i] = NULL;

    return words;
}
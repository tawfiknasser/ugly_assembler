
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node
{
    char *key;

    unsigned int node_type : 4;
    /* 
     0 DATA       0
     1 CODE       0
     2 extern     0
     3 ENTRY      0
     1001 */
    struct node *next;
    union value {
        char *string_value;
        int int_value;
    } value;
} node;

enum symbol_types
{
    DATA_SYMBOL
};

node *create_node();
node *add_node_to_head(node *head_node);
node *add_node_to_tail(node *tail_node);
node *find_tail(node *root_node);
node *find_node_by_key(node *root_node, char *key);
node *find_node_by_int_value(node *root_node, char *value);
void clean_linked_list(node *head_node);

#endif
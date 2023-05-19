#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
* create node
* @return head node
*/
node *create_node()
{
    node *single_node = (node *)malloc(sizeof(node));
    if (single_node == NULL)
    {
        printf("Failed to allocate new node");
        exit(1);
    }
    single_node->next = NULL;
    single_node->node_type = 0;
    return single_node;
}

/**
* @return new head node
*/
node *add_node_to_head(node *head_node)
{
    node *new_head_node = create_node();
    new_head_node->next = head_node;

    if (head_node == NULL)
    {
        return new_head_node;
    }

    return new_head_node;
}

/**
* @return new tail node pointer
*/
node *add_node_to_tail(node *tail_node)
{
    node *new_tail_node = create_node();

    if (tail_node == NULL || tail_node->next)
    {
        printf("Error: Something is wrong with the tail node provided.");
        exit(1);
    }

    tail_node->next = new_tail_node;
    new_tail_node->next = NULL;
    return new_tail_node;
}

/**
* @return tail node pointer
*/
node *find_tail(node *root_node)
{
    node *tail_node;

    tail_node = root_node;
    while (tail_node->next)
    {
        tail_node = tail_node->next;
    }
    return tail_node;
}

/**
 * @return target node or NULL in case not found
 */
node *find_node_by_key(node *root_node, char *key)
{

    /*problema*/
    node *target_node;

    if (root_node == NULL)
    {
        return NULL;
    }
    target_node = root_node;

    do
    {
        if (strcmp(key, target_node->key) == 0)
            return target_node;

    } while ((target_node = target_node->next));

    return NULL;
}

/**
 * @return target node or NULL in case not found
 */
node *find_node_by_int_value(node *root_node, char *value)
{
    node *target_node;

    if (root_node == NULL)
        return NULL;

    target_node = root_node;

    do
    {
        if (strcmp(value, target_node->value.string_value) == 0)
            return target_node;

        target_node = target_node->next;
    } while (target_node->next);

    return NULL;
}

/**
loop over all the nodes and free them
*/
void clean_linked_list(node *head_node)
{
    node *node_walker = head_node;
    while (node_walker)
    {
        free(node_walker->key);
        free(node_walker);
        node_walker = node_walker->next;
    }
}

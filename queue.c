#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *newHead =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (!newHead)
        return NULL;
    INIT_LIST_HEAD(newHead);
    return newHead;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;
    element_t *cur = NULL, *next;
    list_for_each_entry_safe (cur, next, l, list) {
        free(cur->value);
        free(cur);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *node = (element_t *) malloc(sizeof(element_t));
    if (!node)
        return false;
    node->value = strdup(s);
    if (!node->value) {
        free(node);
        return false;
    }
    list_add(&node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *node = (element_t *) malloc(sizeof(element_t));
    if (!node)
        return false;
    node->value = strdup(s);
    if (!node->value) {
        free(node);
        return false;
    }
    list_add_tail(&node->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head))
        return NULL;

    struct list_head *cur = head->next;
    element_t *ele = container_of(cur, element_t, list);
    head->next = cur->next;
    cur->next->prev = head;
    cur->next = NULL;
    cur->prev = NULL;
    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (list_empty(head))
        return NULL;

    struct list_head *cur = head->prev;
    element_t *ele = container_of(cur, element_t, list);
    head->prev = cur->prev;
    cur->prev->next = head;
    cur->prev = cur->next = NULL;
    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return -1;
    int count = 0;
    struct list_head *node;
    list_for_each (node, head->next)
        count++;
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *curHead = head->next;
    struct list_head *curTail = head->prev;
    struct list_head *list_reverse(struct list_head *);
    curTail->next = NULL;
    list_reverse(curHead);

    head->next = curTail;
    head->prev = curHead;
    curTail->prev = head;
    curHead->next = head;
}

struct list_head *list_reverse(struct list_head *cur)
{
    if (!cur || !cur->next)
        return cur;
    struct list_head *next = list_reverse(cur->next);
    cur->prev = next;
    next->next = cur;
    return cur;
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

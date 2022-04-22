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
    if (!head)
        return false;

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
    if (!head)
        return false;
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
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;

    struct list_head *cur = head->next;
    element_t *ele = container_of(cur, element_t, list);
    head->next = cur->next;
    cur->next->prev = head;
    cur->next = NULL;
    cur->prev = NULL;

    if (!sp)
        return ele;

    /* Double check sp is bufsize*/
    // int l = strlen(sp);
    // bufsize = (bufsize < l)?bufsize : l;

    int len = strlen(ele->value);
    if (bufsize > len)
        bufsize = len;
    else
        bufsize = bufsize - 1;
    memset(sp, 0, bufsize + 1);
    strncpy(sp, ele->value, bufsize);

    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;

    struct list_head *cur = head->prev;
    element_t *ele = container_of(cur, element_t, list);
    head->prev = cur->prev;
    cur->prev->next = head;
    cur->prev = cur->next = NULL;

    if (!sp)
        return ele;

    memset(sp, 0, bufsize);

    int len = strlen(ele->value);
    if (bufsize > len)
        bufsize = len;
    strncpy(sp, ele->value, bufsize);

    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
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
    if (!head)
        return false;
    if (list_empty(head))
        return false;

    struct list_head *turtle = head->next;
    struct list_head *rabbit = head->next;


    head->prev->next = NULL;

    while (rabbit && rabbit->next) {
        turtle = turtle->next;
        rabbit = rabbit->next->next;
    }

    list_del(turtle);
    element_t *tmp = container_of(turtle, element_t, list);
    free(tmp->value);
    free(tmp);
    head->prev->next = head;

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;
    q_sort(head);

    struct list_head *newHead = head->next;
    struct list_head **prev = &newHead;
    struct list_head *cur = newHead;
    head->prev->next = NULL;

    while (cur && cur->next) {
        element_t *a, *b;
        a = container_of(cur, element_t, list);
        b = container_of(cur->next, element_t, list);

        /* Determine is it duplicate */
        if (strcmp(a->value, b->value) == 0) {
            struct list_head *tmp = cur;
            element_t *c = a;
            element_t *d = b;

            /* Locate duplicate elements */
            while (strcmp(c->value, d->value) == 0) {
                tmp = tmp->next;
                if (!tmp->next)
                    break;
                d = container_of(tmp->next, element_t, list);
            }
            /* Update current point */
            cur = tmp->next;

            /* Temporarily keep next list node */
            (*prev) = tmp->next;

            /* Keep the next point to be end of free duplicate */
            element_t *now = container_of(tmp->next, element_t, list);
            while (c != now) {
                struct list_head *achieve = c->list.next;
                free(c->value);
                free(c);
                c = container_of(achieve, element_t, list);
            }

        } else {
            cur = cur->next;
            prev = &(*prev)->next;
        }
    }

    if (newHead) {
        head->next = newHead;
        newHead->prev = head;

        struct list_head *tmp = head->next;
        while (tmp->next) {
            tmp = tmp->next;
        }
        head->prev = tmp;
        tmp->next = head;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    void swap(struct list_head **);
    if (!head)
        return;
    if (list_empty(head))
        return;

    head->prev->next = NULL;
    swap(&(head->next));

    struct list_head *last = NULL;
    last = head->next;
    while (last->next) {
        last->next->prev = last;
        last = last->next;
    }
    last->next = head;
    head->prev = last;
}

void swap(struct list_head **head)
{
    if (!*head || !(*head)->next)
        return;
    struct list_head *next = (*head)->next;

    /* A -> B -> C*/
    (*head)->next = next->next;
    /*
     * A -> C
     * B -> C
     */
    next->next = (*head);
    /* B -> A -> C */

    (*head) = next;
    swap(&(*head)->next->next);
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;
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
void q_sort(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *sort(struct list_head *);
    if (list_empty(head))
        return;
    head->prev->next = NULL;
    head->next = sort(head->next);


    struct list_head *tmp = head->next;
    while (tmp->next) {
        tmp->next->prev = tmp;
        tmp = tmp->next;
    }
    head->prev = tmp;
    tmp->next = head;
}

#include <stdint.h>
struct list_head *sort(struct list_head *head)
{
    if (!head || !head->next)
        return head;

    struct list_head *tur = head;
    struct list_head *rab = tur->next;

    while (rab && rab->next) {
        tur = tur->next;
        rab = rab->next->next;
    }

    rab = tur->next;
    tur->next = NULL;
    tur = head;

    tur = sort(tur);
    rab = sort(rab);

    struct list_head **cur, **node = NULL;
    for (cur = &head; rab && tur;) {
        element_t *r_ele = container_of(rab, element_t, list);
        element_t *t_ele = container_of(tur, element_t, list);
        node = (strcmp(r_ele->value, t_ele->value) < 0) ? &rab : &tur;
        *cur = *node;
        cur = &(*cur)->next;
        (*node) = (*node)->next;
    }

    *cur = (struct list_head *) ((uintptr_t) rab | (uintptr_t) tur);
    return head;
}

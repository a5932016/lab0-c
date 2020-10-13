#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    int len = sizeof(char) * (strlen(s) + 1);
    char *tmp = malloc(len);
    if (!tmp) {
        free(newh);
        return false;
    }

    memcpy(tmp, s, len);
    newh->value = tmp;
    newh->next = q->head;
    q->head = newh;

    if (!q->tail) {
        q->tail = q->head;
    }

    q->size += 1;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    int len = sizeof(char) * (strlen(s) + 1);
    char *tmp = malloc(len);
    if (!tmp) {
        free(newt);
        return false;
    }
    memcpy(tmp, s, len);
    newt->value = tmp;
    newt->next = NULL;

    if (!q->head) {
        q->head = newt;
    }
    if (q->tail) {
        q->tail->next = newt;
    }

    q->tail = newt;
    q->size += 1;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    q->size -= 1;
    if (!q->size) {
        q->tail = NULL;
    }

    if (sp) {
        size_t len =
            bufsize > strlen(tmp->value) ? strlen(tmp->value) : bufsize - 1;
        memset(sp, '\0', len + 1);
        strncpy(sp, tmp->value, len);
    }

    free(tmp->value);
    free(tmp);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1)
        return;

    list_ele_t *tmp;
    list_ele_t *prev = NULL;
    list_ele_t *curr = q->head;
    while (curr) {
        tmp = curr->next;
        curr->next = prev;
        prev = curr;
        curr = tmp;
    }
    q->head->next = NULL;

    tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
}

/*
 * Merge Sort
 * sort the linked list
 * head is list_ele_t
 */
void mergeSort(list_ele_t **head)
{
    if (!*head || !(*head)->next)
        return;
    list_ele_t *l1 = (*head)->next;
    list_ele_t *l2 = *head;

    while (l1 && l1->next) {
        l2 = l2->next;
        l1 = l1->next->next;
    }
    l1 = l2->next;
    l2->next = NULL;
    l2 = *head;

    mergeSort(&l2);
    mergeSort(&l1);

    *head = NULL;
    list_ele_t **tmp = head;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            l1 = l1->next;
        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }

    *tmp = l1 ? l1 : l2;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || q->size <= 1)
        return;
    mergeSort(&q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}

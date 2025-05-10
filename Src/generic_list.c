/*****************************************************************************
 *                                                                            *
 *  -------------------------------- list.c --------------------------------  *
 *                                                                            *
 *****************************************************************************/


#include <string.h>
#include <assert.h>


#include "constante.h"
#include "generic_list.h"

#ifdef DEBUG
#pragma message "activate debug mode generic_list"
#endif

/*****************************************************************************
 *                                                                            *
 *  ------------------------------- list_init ------------------------------  *
 *                                                                            *
 *****************************************************************************/

void generic_list_init(generic_list_t *list,
		       int (*compare) (const void *key1, const void *key2),
		       void* (*build)(const void *data),
		       void (*destroy)(void *data))
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  /* Initialize the list. */

  list->size = 0;
  list->compare = compare;
  list->build = build;
  list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- list_size  -------------------------------  *
 *                                                                            *
 *****************************************************************************/


int generic_list_size (generic_list_t *list)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (list != NULL);
  return list->size;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- list_destroy -----------------------------  *
 *                                                                            *
 *****************************************************************************/

void generic_list_destroy(generic_list_t *list)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  void *data;

  /*  Remove each element. */
  while (generic_list_size(list) > 0) {

    if (generic_list_rem_next(list, NULL, (void **)&data) == 0 && list->destroy !=
	NULL) {

      /* Call a user-defined function to free dynamically allocated data. */
      list->destroy(data);
    }
  }

  /*  No operations are allowed now, but clear the structure as a precaution. */
  memset(list, 0, sizeof(generic_list_t));
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- list_ins_next ----------------------------  *
 *                                                                            *
 *****************************************************************************/

int generic_list_ins_next(generic_list_t *list,
			  generic_list_elmt_t *element,
			  const void *data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif
  assert (list != NULL);

  generic_list_elmt_t *new_element;

  /*  Allocate storage for the element. */
  new_element = malloc(sizeof(generic_list_elmt_t));
  if (new_element == NULL) {
    return -1;
  }

  /* Insert the element into the list. */
  /* HACK for the generic_graph which uses build function for different purposes (list, queue, stack and set) */
  if (list->build != NULL) {
    new_element->data = list->build(data);
  }
  else {
    new_element->data = (void*) data;
  }

  if (element == NULL) {
    /* Handle insertion at the head of the list. */
    if (generic_list_size(list) == 0) {
      list->tail = new_element;
    }

    new_element->next = list->head;
    list->head = new_element;
  }
  else {
    /* Handle insertion somewhere other than at the head. */
    if (element->next == NULL) {
      list->tail = new_element;
    }
    new_element->next = element->next;
    element->next = new_element;
  }

  /* Adjust the size of the list to account for the inserted element. */
  list->size++;

  return 0;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- list_rem_next ----------------------------  *
 *                                                                            *
 *****************************************************************************/

int generic_list_rem_next(generic_list_t *list,
			  generic_list_elmt_t *element,
			  void **data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *old_element = NULL;

  /*  Do not allow removal from an empty list. */
  if (generic_list_size(list) == 0) {
    return -1;
  }

  /* Remove the element from the list. */
  if (element == NULL) {
    /* Handle removal from the head of the list. */

    *data = list->head->data;
    old_element = list->head;
    list->head = list->head->next;

    if (generic_list_size(list) == 1) {
      list->tail = NULL;
    }

  }

  else {

    /* Handle removal from somewhere other than the head. */
    if (element->next == NULL) {
      return -1;
    }

    *data = element->next->data;
    old_element = element->next;
    element->next = element->next->next;

    if (element->next == NULL) {
      list->tail = element;
    }
  }

  /* Free the storage allocated by the abstract data type. */
  if (list->destroy != NULL) {
    list->destroy(old_element);
  }

  /* Adjust the size of the list to account for the removed element. */
  list->size--;

  return 0;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- generic_list_head ------------------------  *
 *                                                                            *
 *****************************************************************************/

generic_list_elmt_t* generic_list_head (generic_list_t *list)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (list != NULL);
  return list->head;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- generic_list_tail ------------------------  *
 *                                                                            *
 *****************************************************************************/

generic_list_elmt_t* generic_list_tail (generic_list_t *list)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (list != NULL);
  return list->tail;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- generic_list_is_head ---------------------  *
 *                                                                            *
 *****************************************************************************/

bool generic_list_is_head (generic_list_t *list, generic_list_elmt_t *element)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (list != NULL);
  if (list->head==element) {
    return true;
  }
  else {
    return false;
  }
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- generic_list_is_tail ---------------------  *
 *                                                                            *
 *****************************************************************************/

bool generic_list_is_tail (generic_list_elmt_t *element)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (element != NULL);
  if( element->next==NULL) {
    return true;
  }
  else {
    return false;
  }
}


/*****************************************************************************
 *                                                                            *
 *  ----------------------------- generic_list_data ----------------------  *
 *                                                                            *
 *****************************************************************************/

void* generic_list_data(generic_list_elmt_t* element)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif
  assert (element != NULL);
  return element->data;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- generic_list_next ----------------------  *
 *                                                                            *
 *****************************************************************************/

generic_list_elmt_t* generic_list_next(generic_list_elmt_t* element)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (element != NULL);
  return element->next;
}

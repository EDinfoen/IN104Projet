/*! \file generic_list.h
 * \brief Liste chainée contenant des valeurs de type <b> void* </b>
 *
 */

/*****************************************************************************
 *                                                                            *
 *  -------------------------------- list.h --------------------------------  *
 *                                                                            *
 *****************************************************************************/

#ifndef GENERIC_LIST_H
#define GENERIC_LIST_H

#include "constante.h"

/*****************************************************************************
 *                                                                            *
 *  Define a structure for linked list elements.                              *
 *                                                                            *
 *****************************************************************************/

/*! \brief Type de données pour représenter un maillon d'une liste
    chainée contenant des <b> void* </b>
 *
 * Une liste chainée est composée de plusieurs maillons. Chacun
 * contient une données et un pointeur vers le prochain maillon
 */
typedef struct generic_list_elmt_ {
  void *data; /**< donnée stockée dans le maillon de la chaine */
  struct generic_list_elmt_ *next;  /**< pointeur vers le prochain
					maillon (peut être NULL) */
} generic_list_elmt_t;

/*****************************************************************************
 *                                                                            *
 *  Define a structure for linked lists.                                      *
 *                                                                            *
 *****************************************************************************/

/*! \brief Type de données pour représenter une liste chainée
    contenant des <b> void* </b>
 *
 * Une liste chainée permet un accès direct à la taille (nombre
 * d'éléments de la chaine), vers le premier élément et vers le
 * dernier élément.
 */
typedef struct generic_list_ {
  int size;  /**< nombre d'éléments stockés dans la liste */

  int (*compare)(const void *key1, const void *key2); /**< fonction de
							 comparaison
							 entre deux
							 éléments de
							 la liste */
  void* (*build)(const void *data); /**< fonction de construction d'un
							 élément de la
							 liste */
  void (*destroy)(void *data); /**< fonction de destruction d'un
							 élément de la
							 liste */

  generic_list_elmt_t *head; /**< pointeur vers le premier élément de
					 la liste */
  generic_list_elmt_t *tail; /**< pointeur vers le dernier
					 élément de la liste */

} generic_list_t;

/*****************************************************************************
 *                                                                            *
 *  --------------------------- Public Interface ---------------------------  *
 *                                                                            *
 *****************************************************************************/

/*! \brief Initialisation d'une nouvelle liste chainée
 *
 * Met à zéro la taille et à NULL les pointeurs vers le premier et le
 * dernier élément de la liste. Cette fonction nécessite une fonction
 * de comparaison des éléments de la file, une fonction de
 * construction d'un élément de la file et une fonction de destruction
 * des éléments de la file.
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 *
 * @param compare fonction permettant la comparaison de deux éléments
 * de la liste
 *
 * @param build fonction permettant la construction d'une valeur de la
 * liste
 *
 * @param destroy fonction permettant la destruction d'une valeur de
 * la liste
 */
void generic_list_init(generic_list_t *list,
		       int (*compare) (const void *key1, const void *key2),
		       void* (*build)(const void *data),
		       void (*destroy)(void *data));

/*! \brief Destruction d'une liste chainée
 *
 * Parcours la liste pour détruire chaque maillon
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 */
void generic_list_destroy(generic_list_t *list);

/*! \brief Insertion dans une liste chainée
 *
 * Etant donné un pointeur vers une liste chainée et un pointeur vers
 * un élément de cette liste, ajout d'un nouvel élément après
 * l'élément passé en argument. Si element est le pointeur NULL alors
 * l'insertion se fait en tête de liste.
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 *
 * @param element un pointeur vers la structure de donnée maillon
 * generic_list_elmt_t
 *
 * @param data une valeur de type <b> void* </b> à ajouter dans la
 * liste dans un nouveau maillon
 *
 * @return un entier indiquant si tout c'est bien passé (0 si ok, -1
 * en cas de problème d'allocation mémoire)
 */
int generic_list_ins_next(generic_list_t *list,
			  generic_list_elmt_t *element,
			  const void *data);

/*! \brief Suppression dans une liste chainée
 *
 * Etant donné un pointeur vers une liste chainée et un pointeur vers
 * un élément de cette liste, suppression de l'élément après l'élément
 * passé en argument. Si element est le pointeur NULL alors
 * la suppression se fait en tête de liste.
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 *
 * @param element un pointeur vers la structure de donnée maillon
 * generic_list_elmt_t
 *
 * @param data un pointeur de type <b> void* </b> pour récupérer la
 * donnée associée à l'élément supprimé
 *
 * @return un entier indiquant si tout c'est bien passé (0 si ok, -1
 * en cas de problème mémoire)
 */
int generic_list_rem_next(generic_list_t *list,
			  generic_list_elmt_t *element,
			  void **data);

/*! \brief Accesseur sur la taille de la liste
 *
 * Etant donné un pointeur vers une liste chainée, retourne la valeur
 * du champ size
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 *
 * @return un entier indiquant la taille de la liste
 */
int generic_list_size (generic_list_t *list);

/*! \brief Accesseur sur le premier élément de la liste
 *
 * Etant donné un pointeur vers une liste chainée, retourne la valeur
 * du pointeur sur l'élément en tête
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 *
 * @return l'adresse du premier élément ou NULL si la chaine est vide
 */
generic_list_elmt_t* generic_list_head (generic_list_t *list);

/*! \brief Accesseur sur le dernier élément de la liste
 *
 * Etant donné un pointeur vers une liste chainée, retourne la valeur
 * du pointeur sur l'élément en queue
 *
 * @param list est un pointeur vers la structure de données liste
 * chainée generic_list_t
 *
 * @return l'adresse du dernier élément ou NULL si la chaine est vide
 */
generic_list_elmt_t* generic_list_tail (generic_list_t *list);

/*! \brief Prédicat pour tester si un élément est le premier maillon
    d'une liste chaînée
 *
 * Etant donné un pointeur vers une liste chainée et un pointeur vers
 * un élément, indique si l'élément est l'élément de tête de la liste
 *
 * @param list est un pointeur vers une structure de données liste
 * chainée generic_list_t
 *
 * @param element est un pointeur vers une structure de données
 * generic_list_elmt_t
 *
 * @return un booléen indiquant vrai si element est le premier élement
 * de list ou faux sinon
 */
bool generic_list_is_head (generic_list_t* list,
			   generic_list_elmt_t *element);

/*! \brief Prédicat pour tester si un élément est le dernier maillon
    d'une liste chaînée
 *
 * Etant donné un pointeur vers une liste chainée et un pointeur vers
 * un élément, indique si l'élément est l'élément de queue de la liste
 *
 * @param list est un pointeur vers une structure de données liste
 * chainée generic_list_t
 *
 * @param element est un pointeur vers une structure de données
 * generic_list_elmt_t
 *
 * @return un booléen indiquant vrai si element est le dernier élement
 * de list ou faux sinon
 */
bool generic_list_is_tail (generic_list_elmt_t *element);

/*! \brief Accesseur sur la donnée contenue dans un élément d'une
    liste chaînée
 *
 * Etant donné un pointeur vers un élément d'une liste chaînée,
 * retourne la valeur de la donnée stockée dans cet élément
 *
 * @param element est un pointeur vers la structure de données
 * generic_list_elmt_t
 *
 * @return la valeur de la donnée du type <b> void* </b> contenu dans
 * l'élément
 */
void* generic_list_data (generic_list_elmt_t *element);

/*! \brief Accesseur sur le successeur d'un élément
 *
 * Etant donné un pointeur vers un élément d'une liste chaînée,
 * retourne la valeur du pointeur vers le prochain élément de la
 * liste.
 *
 * @param element est un pointeur vers la structure de données
 * generic_list_elmt_t
 *
 * @return la valeur du pointeur du prochain élément ou NULL s'il n'y
 * a pas de successeur
 */
generic_list_elmt_t* generic_list_next (generic_list_elmt_t *element);

#endif

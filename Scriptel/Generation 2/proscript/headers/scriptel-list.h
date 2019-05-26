#ifndef _SCRIPTEL_LIST_H_
#define _SCRIPTEL_LIST_H_

#include <stdlib.h>

/**
* This structure defines a doubly linked list wrapper.
*/
typedef struct scriptel_list_item {
	/**
	* Pointer to list item.
	*/
	void* ptr;
	/**
	* Pointer to the next item in the list.
	*/
	struct scriptel_list_item* next;
	/**
	* Pointer to the previous item in the list.
	*/
	struct scriptel_list_item* prev;
} scriptel_list_item;

/**
* This structure defines a linked list.
*/
typedef struct scriptel_list {
	/**
	* First item in the list.
	*/
	scriptel_list_item* first;
	/**
	* Last item in the list.
	*/
	scriptel_list_item* last;
} scriptel_list;

/**
 * This function allocates a new list.
 * @return Newly allocated list.
 */
scriptel_list* scriptel_create_list(void);
/**
 * This function frees a previously allocated list.
 * @param list List to free.
 */
void scriptel_list_free(scriptel_list* list);
/**
 * This function adds a new list item to a list and sets
 * the pointer to the passed in value.
 * @param list List to add to.
 * @param ptr Pointer to wrap.
 * @return List item that was created and added.
 */
scriptel_list_item* scriptel_list_add(scriptel_list* list, void* ptr);
/**
 * This function removes an list item from a list. This function
 * is tolerant of list or item being null, so you can do:
 * scriptel_list_remove(list, scriptel_list_find(list, ptr));
 * safely.
 * @param list List to remove from.
 * @param item Item to remove.
 * @return Returns 1 if something was removed, 0 otherwise.
 */
unsigned char scriptel_list_remove(scriptel_list* list, scriptel_list_item* item);
/**
 * This function counts a list.
 * @param list List to count.
 * @return List length.
 */ 
unsigned int scriptel_list_count(scriptel_list* list);
/**
 * This function looks for a specific pointer in a list and will return the
 * list item associated with the first instance found, or null if nothing is found.
 * @param list List to search.
 * @param ptr Pointer to look for.
 * @return List item if found, NULL otherwise.
 */
scriptel_list_item* scriptel_list_find(scriptel_list* list, void* ptr);

/**
 * This function searches a list for a pointer and will remove the first
 * one it finds if it finds any at all.
 * @param list List to search.
 * @param ptr Pointer to search for.
 * @return Returns 1 if something was removed, 0 otherwise.
 */
unsigned char scriptel_list_remove_ptr(scriptel_list* list, void* ptr);
#endif
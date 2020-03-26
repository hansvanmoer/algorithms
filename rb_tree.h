/*
 * This file is part of Algorithms.
 *
 * Algorithms is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Algorithms is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Algorithms.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdbool.h>

/**
 * A simple implementation of a red black tree
 */


/**
 * A node in the red black tree
 */
struct rb_node;

struct rb_tree;

/**
 * A function pointer type for the comparison function used
 * in the red black tree.
 * Signature: int fn(struct rb_tree *, void * first, void * second)
 * Returns an int smaller than 1 if first < second
 * Returns 0 if first == second
 * Returns an int greater than 1 if first > second
 */
typedef int (*rb_cmp_f)(const struct rb_tree *, void *, void *);

/**
 * A function pointer type for a function to apply to values
 * in the red black tree.
 * Signature: void f(struct rb_tree *, void * key_or_val)
 */
typedef void (*rb_apply_f)(struct rb_tree *, void *);

/**
 * A red black tree
 */
struct rb_tree{

  /**
   * The root of the tree
   */
  struct rb_node * root;

  /**
   * The sentinel node
   */
  struct rb_node * nil;
  
  /**
   * The comparison function
   */
  rb_cmp_f cmp_value;

  /**
   * The free function for values
   */
  rb_apply_f free_value;

  /**
   * Extra state for the tree
   */
  void * state;
  
};

/**
 * Initializes a red black tree.
 * @param a pointer to the tree
 * @param cmp_value a pojnter to a comparison function
 * @param free_value a pointer to a function to free the values or NULL if the values should not be freed
 * @param free_state a pointer to state data used in the free function
 */
void rb_tree_init(struct rb_tree * tree, rb_cmp_f cmp_value, rb_apply_f free_value, void * state);

/**
 * Finds the node associated to the specified value in the tree
 * @param tree the tree
 * @param the value to find
 * @return the associated node or NULL if no such node exists
 */
struct rb_node * rb_tree_find(const struct rb_tree * tree, void * value);

/**
 * Returns the first in the tree, or NULL if the tree is empty
 * @param tree the tree
 * @return a pointer to the node
 */
struct rb_node * rb_tree_get_begin(const struct rb_tree * tree);

/**
 * Returns the last in the tree, or NULL if the tree is empty
 * @param tree the tree
 * @return a pointer to the node
 */
struct rb_node * rb_tree_get_end(const struct rb_tree * tree);

/**
 * Returns the next node in the tree, or NULL if the tree is empty
 * @param tree the tree
 * @param node the current node
 * @return a pointer to the node
 */
struct rb_node * rb_tree_get_next(const struct rb_tree * tree, struct rb_node * node);


/**
 * Returns the previous node in the tree, or NULL if the tree is empty
 * @param tree the tree
 * @param node the current node
 * @return a pointer to the node
 */
struct rb_node * rb_tree_get_previous(const struct rb_tree * tree, struct rb_node * node);

/**
 * Returns the value associated to the node
 * @param tree the tree
 * @param node the node
 * @return the value
 */
void * rb_tree_get_value(const struct rb_tree * tree, struct rb_node * node);

/**
 * Checks whether the tree is empty
 * @param tree the tree
 * @return true if the tree is empty, false otherwise
 */
bool rb_tree_is_empty(const struct rb_tree * tree);

/**
 * Applies the function to all values in the red black tree in order
 * @param tree the tree
 * @param apply a function to apply to all values
 */
void rb_tree_apply(struct rb_tree * tree, rb_apply_f apply);

/**
 * Inserts a value in the red black tree
 * @param tree the tree
 * @param value the value to insert
 * @return true if the value replaces an existing value, false otherwise
 */
bool rb_tree_insert(struct rb_tree * tree, void * value);

/**
 * Deletes a node from the tree
 * @param tree the tree
 * @param node the node to delete
 */
void rb_tree_delete(struct rb_tree * tree, struct rb_node * node);

/**
 * Deletes a value from the red black tree
 * @param tree the tree
 * @param value the value to delete
 * @return true if the value was found and deleted, false otherwise
 */
bool rb_tree_find_and_delete(struct rb_tree * tree, void * value);

/**
 * Frees all data associated to the red black tree
 * Does not free the tree struct itself
 * @pram tree a pointer to a tree
 */
void rb_tree_free(struct rb_tree * tree);

#endif

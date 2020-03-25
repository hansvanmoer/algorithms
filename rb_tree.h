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

/**
 * A function pointer type for the comparison function used
 * in the red black tree.
 * Signature: int f(void * first, void * second)
 * Returns an int smaller than 1 if first < second
 * Returns 0 if first == second
 * Returns an int greater than 1 if first > second
 */
typedef int (*rb_cmp_f)(void *, void *);

/**
 * A function pointer type for a function to apply to values
 * in the red black tree.
 * Signature: void f(void * key_or_val)
 */
typedef void (*rb_apply_f)(void *);

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
};

/**
 * Initializes a red black tree.
 * @param a pointer to the tree
 * @param cmp_value a pojnter to a comparison function
 * @param free_value a pointer to a function to free the values or NULL if the values should not be freed
 */
void rb_tree_init(struct rb_tree * tree, rb_cmp_f cmp_value, rb_apply_f free_value);


/**
 * Checks whether the tree is empty
 * @param tree the tree
 * @return true if the tree is empty, false otherwise
 */
bool rb_tree_is_empty(const struct rb_tree * tree);

/**
 * Applies the functin to all values in the red black tree in order
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
 * Deletes a value from the red black tree
 * @param tree the tree
 * @param value the value to delete
 * @return true if the value was found and deleted, false otherwise
 */
bool rb_tree_delete(struct rb_tree * tree, void * value);

/**
 * Frees all data associated to the red black tree
 * Does not free the tree struct itself
 * @pram tree a pointer to a tree
 */
void rb_tree_free(struct rb_tree * tree);

#endif

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

#include "memory.h"
#include "rb_tree.h"

#include <assert.h>
#include <stdbool.h>

/**
 * A node in the red black node
 */
struct rb_node{

  /**
   * A pointer to the parent
   */
  struct rb_node * parent;

  /**
   * A pointer to the left child
   */
  struct rb_node * left;

  /**
   * A pointer to the right child
   */
  struct rb_node * right;

  /**
   * Whether the node is red
   */
  bool red;
  
  /**
   * A pointer the value
   */
  void * value;
};

/**
 * The sentinel node
 */
static struct rb_node nil = { NULL, NULL, NULL, NULL, false};

#define NIL &nil

/**
 * The default free function (does nothing)
 */
static void default_free_value(void * value){};
  
void rb_tree_init(struct rb_tree * tree, rb_cmp_f cmp_value, rb_apply_f free_value){
  assert(tree != NULL);
  assert(cmp_value != NULL);

  tree->root = NIL;
  tree->cmp_value = cmp_value;
  if(free_value == NULL){
    tree->free_value = default_free_value;
  }else{
    tree->free_value = free_value;
  }
}

void rb_tree_free(struct rb_tree * tree){
  assert(tree != NULL);

  
}

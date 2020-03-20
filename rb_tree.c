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
#include <stdlib.h>

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

/**
 * Returns the minimum node in this subtree
 * @param the root of the subtree or NIL
 * @return the minimum node or NIL if node is NIL
 */
static struct rb_node * get_min(struct rb_node * node){
  if(node == NIL){
    return NIL;
  }else{
    while(node->left != NIL){
      node = node->left;
    }
    return node;
  }
}

/**
 * Gets the in-order successor of the supplied node
 * @param node a node that is not NIL
 * @return the successor
 */
static struct rb_node * get_next(struct rb_node * node){
  assert(node != NIL);
  
  if(node->right == NIL){
    while(node == node->parent->right){
      node = node->parent;
    }
    return node->parent;
  }else{
    return get_min(node->right);
  }
}

/**
 * Creates a node containing the supplied values and sensible defaults
 * @param the value of the new node
 * @return a new node
 */
static struct rb_node * create_node(void * value){
  struct rb_node * node = malloc_checked(sizeof(struct rb_node));
  node->value = value;
  node->red = true;
  node->left = NIL;
  node->right = NIL;
  return node;
}

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

bool rb_tree_is_empty(const struct rb_tree * tree){
  assert(tree !=NULL);
  return tree->root != NIL;
}

bool rb_tree_insert(struct rb_tree * tree, void * value){
  assert(tree != NULL);

  if(tree->root == NIL){
    struct rb_node * node = create_node(value);
    node->parent = NIL;
    tree->root = node;
    return false;
  }else{
    struct rb_node * pos = tree->root;
    while(true){
      int cmp = (*tree->cmp_value)(value, pos->value);
      if(cmp < 0){
	if(pos->left == NIL){
	  struct rb_node * node = create_node(value);
	  node->parent = pos;
	  pos->left = node;
	  return false;
	}else{
	  pos = pos->left;
	}
      }else if(cmp > 0){
	if(pos->right == NIL){
	  struct rb_node * node = create_node(value);
	  node->parent = pos;
	  pos->right = node;
	  return false;
	}else{
	  pos = pos->right;
	}
      }else{
	(*tree->free_value)(pos->value);
	pos->value = value;
	return true;
      }
    }
  }
}

void rb_tree_apply(struct rb_tree * tree, rb_apply_f apply){
  assert(tree != NULL);
  assert(apply != NULL);

  struct rb_node * node = get_min(tree->root);
  while(node != NIL){
    (*apply)(node->value);
    node = get_next(node);
  }
}

void rb_tree_free(struct rb_tree * tree){
  assert(tree != NULL);

  struct rb_node * pos = tree->root;
  struct rb_node * next;
  
  while(pos != NIL){
    if(pos->left != NIL){
      next = pos->left;
      pos->left = NIL;
    }else if(pos->right != NIL){
      next = pos->right;
      pos->right = NIL;
    }else{
      next = pos->parent;
      (*tree->free_value)(pos->value);
      free(pos);
    }
    pos = next;
  }
}

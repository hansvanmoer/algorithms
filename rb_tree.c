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
 * The default free function (does nothing)
 */
static void default_free_value(void * value){};


/**
 * Assertion functions for testing purposes
 */
static int assert_node(struct rb_tree * tree, struct rb_node * node){
  if(node == tree->nil){
    return 1;
  }else{
    if(node->parent == tree->nil){
      assert(!node->red);
    }else if(node->red){
      assert(!node->left->red);
      assert(!node->right->red);
    }
    int left = assert_node(tree, node->left);
    int right = assert_node(tree, node->right);
    assert(left == right);
    return left + (node->red ? 0 : 1);
  }
}

static void assert_tree(struct rb_tree * tree){
  assert(!tree->root->red);
  assert_node(tree, tree->root);
}

/**
 * Returns the minimum node in this subtree
 * @param the root of the subtree or NIL
 * @return the minimum node or NIL if node is NIL
 */
static struct rb_node * get_min(struct rb_tree * tree, struct rb_node * node){
  assert(tree != NULL);
  assert(node != NULL);
 
  if(node == tree->nil){
    return tree->nil;
  }else{
    while(node->left != tree->nil){
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
static struct rb_node * get_next(struct rb_tree * tree, struct rb_node * node){
  assert(tree != NULL);
  assert(node != NULL);
  assert(node != tree->nil);
  
  if(node->right == tree->nil){
    while(node == node->parent->right){
      node = node->parent;
    }
    return node->parent;
  }else{
    return get_min(tree, node->right);
  }
}

/**
 * Creates a node containing the supplied values and sensible defaults
 * @param value the value of the new node
 * @return a new node
 */
static struct rb_node * create_node(struct rb_tree * tree, void * value){
  assert(tree != NULL);

  struct rb_node * node = malloc_checked(sizeof(struct rb_node));
  node->value = value;
  node->red = true;
  node->left = tree->nil;
  node->right = tree->nil;
  return node;
}


void rb_tree_init(struct rb_tree * tree, rb_cmp_f cmp_value, rb_apply_f free_value){
  assert(tree != NULL);
  assert(cmp_value != NULL);

  struct rb_node * nil = malloc_checked(sizeof(struct rb_node));
  nil->value = NULL;
  nil->red = false;
  nil->left = NULL;
  nil->right = NULL;
  
  tree->root = nil;
  tree->nil = nil;
  tree->cmp_value = cmp_value;
  if(free_value == NULL){
    tree->free_value = default_free_value;
  }else{
    tree->free_value = free_value;
  }
}

bool rb_tree_is_empty(const struct rb_tree * tree){
  assert(tree !=NULL);
  
  return tree->root != tree->nil;
}

/**
 * Performs a left rotation on a pivot, rotating it into parent position
 * @param pivot the node to rotate into the parent position
 */
static void rotate_left(struct rb_tree * tree, struct rb_node * pivot){
  assert(tree != NULL);
  assert(pivot != NULL);
  assert(pivot != tree->nil);
  assert(pivot->parent != tree->nil);
  assert(pivot->parent->right == pivot);
  
  struct rb_node * child = pivot->left;
  struct rb_node * parent = pivot->parent;

  pivot->parent = parent->parent;
  if(parent->parent == tree->nil){
    tree->root = pivot;
  }else{
    if(parent == parent->parent->left){
      parent->parent->left = pivot;
    }else{
      parent->parent->right = pivot;
    }
  }
  
  pivot->left = parent;
  parent->parent = pivot;

  parent->right = child;
  if(child != tree->nil){
    child->parent = parent;
  }
}

/**
 * Performs a left rotation on a pivot, rotating it into parent position
 * @param pivot the node to rotate into the parent position
 */
static void rotate_right(struct rb_tree * tree, struct rb_node * pivot){
  assert(tree != NULL);
  assert(pivot != NULL);
  assert(pivot != tree->nil);
  assert(pivot->parent != tree->nil);;
  assert(pivot->parent->left == pivot);
  
  struct rb_node * child = pivot->right;
  struct rb_node * parent = pivot->parent;

  pivot->parent = parent->parent;
  if(parent->parent == tree->nil){
    tree->root = pivot;
  }else{
    if(parent == parent->parent->left){
      parent->parent->left = pivot;
    }else{
      parent->parent->right = pivot;
    }
  }
  
  pivot->right = parent;
  parent->parent = pivot;

  parent->left = child;
  if(child != tree->nil){
    child->parent = parent;
  }
}

/**
 * Fixes the tree after an insert
 */
static void fix_after_insert(struct rb_tree * tree, struct rb_node * node){
  assert(tree != NULL);
  assert(node != NULL);
  assert(node != tree->nil);
  
  while(node->parent->red){
    if(node->parent == node->parent->parent->left){
      struct rb_node * uncle = node->parent->parent->right;
      if(uncle->red){
	node->parent->red = false;
	uncle->red = false;
	node->parent->parent->red = true;
	node = node->parent->parent;
      }else{
	if(node == node->parent->right){
	  rotate_left(tree, node);
	  node = node->left;
	}
	rotate_right(tree, node->parent);
	node->parent->red = false;
	node->parent->right->red = true;
      }
    }else{
      struct rb_node * uncle = node->parent->parent->left;
      if(uncle->red){
	node->parent->red = false;
	uncle->red = false;
	node->parent->parent->red = true;
	node = node->parent->parent;
      }else{
	if(node == node->parent->left){
	  rotate_right(tree, node);
	  node = node->right;
	}
	rotate_left(tree, node->parent);
	node->parent->red = false;
	node->parent->left->red = true;
      }
    }
  }
  tree->root->red = false;
}

bool rb_tree_insert(struct rb_tree * tree, void * value){
  assert(tree != NULL);

  if(tree->root == tree->nil){
    struct rb_node * node = create_node(tree, value);
    node->parent = tree->nil;
    node->red = false;
    tree->root = node;
    return false;
  }else{
    struct rb_node * pos = tree->root;
    while(true){
      int cmp = (*tree->cmp_value)(value, pos->value);
      if(cmp < 0){
	if(pos->left == tree->nil){
	  struct rb_node * node = create_node(tree, value);
	  node->parent = pos;
	  pos->left = node;
	  fix_after_insert(tree, node);
	  return false;
	}else{
	  pos = pos->left;
	}
      }else if(cmp > 0){
	if(pos->right == tree->nil){
	  struct rb_node * node = create_node(tree, value);
	  node->parent = pos;
	  pos->right = node;
	  fix_after_insert(tree, node);
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

/**
 * Finds a value in the tree
 * @param tree the tree
 * @param value the value to look for
 * @return the node containing the supplied value
 */
static struct rb_node * find(struct rb_tree * tree, void * value){
  assert(tree != NULL);
  
  struct rb_node * node = tree->root;
  while(node != tree->nil){
    int cmp = (*tree->cmp_value)(value, node->value);
    if(cmp < 0){
      node = node->left;
    }else if(cmp > 0){
      node = node->right;
    }else{
      break;
    }
  }
  return node;
}

static void replace_node(struct rb_tree * tree, struct rb_node * node, struct rb_node * repl){
  assert(tree != NULL);
  assert(node != NULL);
  assert(node != tree->nil);
  assert((node->left == tree->nil && repl == node->right) || (node->right == tree->nil && repl == node->left));

  if(node->parent == tree->nil){
    tree->root = repl;
  }else if(node == node->parent->left){
    node->parent->left = repl;
  }else{
    node->parent->right = repl;
  }
  repl->parent = node->parent;
}

static void fix_after_delete(struct rb_tree * tree, struct rb_node * node){
  assert(tree != NULL);
  assert(node != NULL);

  while(node->parent != tree->nil && !node->red){
    if(node == node->parent->left){
      struct rb_node * sibling = node->parent->right;
      if(sibling->red){
	sibling->red = false;
	node->parent->red = true;
	rotate_left(tree, sibling);
	sibling = node->parent->right;
      }
      if(!sibling->left->red && !sibling->right->red){
	sibling->red = true;
	node = node->parent;
      }else{
	if(!sibling->right->red){
	  sibling->left->red = false;
	  sibling->red = true;
	  rotate_right(tree, sibling->left);
	  sibling = node->parent->right;
	}
	sibling->red = node->parent->red;
	node->parent->red = false;
	sibling->right->red = false;
	rotate_left(tree, node->parent->right);
	node = tree->root;
      }
      
    }else{
      struct rb_node * sibling = node->parent->left;
      if(sibling->red){
	sibling->red = false;
	node->parent->red = true;
	rotate_right(tree, sibling);
	sibling = node->parent->left;
      }
      if(!sibling->right->red && !sibling->left->red){
	sibling->red = true;
	node = node->parent;
      }else{
	if(!sibling->left->red){
	  sibling->right->red = false;
	  sibling->red = true;
	  rotate_left(tree, sibling->right);
	  sibling = node->parent->left;
	}
	sibling->red = node->parent->red;
	node->parent->red = false;
	sibling->left->red = false;
	rotate_right(tree, node->parent->left);
	node = tree->root;
      }
    }
  }
  node->red = false;
}

static void delete_node(struct rb_tree * tree, struct rb_node * node){
  assert(tree != NULL);
  assert(node != NULL);
  assert(node != tree->nil);
  
  if(node->left == tree->nil){
    replace_node(tree, node, node->right);
    if(!node->red){
      fix_after_delete(tree, node->right);
    }
    (*tree->free_value)(node->value);
    free(node);
  }else if(node->right == tree->nil){
    replace_node(tree, node, node->left);
    if(!node->red){
      fix_after_delete(tree, node->left);
    }
    (*tree->free_value)(node->value);
    free(node);
  }else{
    struct rb_node * repl = get_min(tree, node->right);
    void * value = node->value;
    node->value = repl->value;
    repl->value = value;
    delete_node(tree, repl);
  }
}

bool rb_tree_delete(struct rb_tree * tree, void * value){
  assert(tree != NULL);

  struct rb_node * node = find(tree, value);
  if(node == tree->nil){
    return false;
  }else{
    delete_node(tree, node);
    return true;
  }
}

void rb_tree_apply(struct rb_tree * tree, rb_apply_f apply){
  assert(tree != NULL);
  assert(apply != NULL);

  struct rb_node * node = get_min(tree, tree->root);
  while(node != tree->nil){
    (*apply)(node->value);
    node = get_next(tree, node);
  }
}

void rb_tree_free(struct rb_tree * tree){
  assert(tree != NULL);

  struct rb_node * pos = tree->root;
  struct rb_node * next;
  
  while(pos != tree->nil){
    if(pos->left != tree->nil){
      next = pos->left;
      pos->left = tree->nil;
    }else if(pos->right != tree->nil){
      next = pos->right;
      pos->right = tree->nil;
    }else{
      next = pos->parent;
      (*tree->free_value)(pos->value);
      free(pos);
    }
    pos = next;
  }

  free(tree->nil);
}


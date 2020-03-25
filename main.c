/**
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

#include "rb_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare(void * first, void * second){
  return strcmp((const char *)first, (const char *)second); 
}

static void print(void * value){
  puts((const char *)value);
}

/**
 * The main application entry point
 * Tests the relevant algorithms for correctness
 * @param arg_count the number of command line arguments
 * @param args the command line arguments
 * @return the exit code
 */
int main(int arg_count, const char ** args){

  const char * values[] = {"alpha", "x-ray", "coca", "book", "terra", "none", "factor", "not", "original", "zulu"};

  struct rb_tree tree;

  rb_tree_init(&tree, &compare, NULL);

  for(int i = 0; i < 10; ++i){
    rb_tree_insert(&tree, (void *)values[i]);
    puts("values in tree:");
    rb_tree_apply(&tree, &print);
    puts("");
  }


  for(int i = 5; i < 15; ++i){
    printf("deleting value '%s'\n", values[i % 10]);
    rb_tree_find_and_delete(&tree, (void *)values[i % 10]);
    rb_tree_apply(&tree, &print);
  }
  
  rb_tree_free(&tree);

  return 0;
}

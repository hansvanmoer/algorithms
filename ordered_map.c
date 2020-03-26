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
#include "ordered_map.h"

#include <assert.h>
#include <stdlib.h>

static void default_free_key(struct ordered_map * map, void * key){}

static void default_free_value(struct ordered_map * map, void * value){} 

static int cmp_entry(const struct rb_tree * tree, void * first, void * second){
  struct ordered_map * map = (struct ordered_map *)tree->state;
  struct ordered_map_entry * first_entry = (struct ordered_map_entry *)first;
  struct ordered_map_entry * second_entry = (struct ordered_map_entry *)second;
  return (*map->cmp)(map, first_entry->key, second_entry->key);
} 

static void free_entry(struct rb_tree * tree, void * value){
  struct ordered_map * map = (struct ordered_map *)tree->state;
  struct ordered_map_entry * entry = (struct ordered_map_entry *)value;
  (*map->free_key)(map, entry->key);
  (*map->free_value)(map, entry->value);
  free(entry);
}

void ordered_map_init(struct ordered_map * map, ordered_map_cmp_f cmp, ordered_map_free_f free_key, ordered_map_free_f free_value, void * state){
  assert(map != NULL);
  assert(cmp != NULL);

  rb_tree_init(&map->tree, &cmp_entry, &free_entry, map);
  map->cmp = cmp;

  if(free_key == NULL){
    map->free_key = default_free_key;
  }else{
    map->free_key = free_key;
  }

  if(free_value == NULL){
    map->free_value = default_free_value;
  }else{
    map->free_value = free_value;
  }
  map->state = state;
}

bool ordered_map_insert(struct ordered_map * map, void * key, void * value){
  assert(map != NULL);
  
  struct ordered_map_entry * entry = (struct ordered_map_entry *)malloc_checked(sizeof(struct ordered_map_entry));
  entry->key = key;
  entry->value = value;

  return rb_tree_insert(&map->tree, entry);
}

bool ordered_map_delete(struct ordered_map * map, void * key){
  assert(map != NULL);

  struct ordered_map_entry seek = {key, NULL};
  return rb_tree_find_and_delete(&map->tree, &seek);
}

struct ordered_map_entry * ordered_map_find(const struct ordered_map * map, void * key){
  assert(map != NULL);

  struct ordered_map_entry seek = {key, NULL};
  struct rb_node * found = rb_tree_find(&map->tree, &seek);
  if(found == NULL){
    return NULL;
  }else{
    return (struct ordered_map_entry *)rb_tree_get_value(&map->tree, found);
  }
}

void * ordered_map_get(const struct ordered_map * map, void * key){
  assert(map != NULL);

  struct ordered_map_entry * found = ordered_map_find(map, key);
  if(found == NULL){
    return NULL;
  }else{
    return found->value;
  }
}

bool ordered_map_is_empty(const struct ordered_map * map){
  assert(map != NULL);
  
  return rb_tree_is_empty(&map->tree);
}

void ordered_map_free(struct ordered_map * map){
  assert(map != NULL);
  rb_tree_free(&map->tree);
};

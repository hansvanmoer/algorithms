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

#ifndef ORDERED_MAP_H
#define ORDERED_MAP_H

#include "rb_tree.h"

struct ordered_map;

typedef int (*ordered_map_cmp_f)(const struct ordered_map *, void *, void *);

typedef void (*ordered_map_free_f)(struct ordered_map *, void *);

struct ordered_map_entry{
  void * key;
  void * value;
};

typedef void (*ordered_map_apply_f)(struct ordered_map *, struct ordered_map_entry *);

/**
 * An ordered map
 */
struct ordered_map{
  struct rb_tree tree;
  ordered_map_cmp_f cmp;
  ordered_map_free_f free_key;
  ordered_map_free_f free_value;
  void * state;
};

void ordered_map_init(struct ordered_map * map, ordered_map_cmp_f cmp, ordered_map_free_f free_key, ordered_map_free_f free_value, void * state);

bool ordered_map_insert(struct ordered_map * map, void * key, void * value);

bool ordered_map_delete(struct ordered_map * map, void * key);

struct ordered_map_entry * ordered_map_find(const struct ordered_map * map, void * key);

void * ordered_map_get(const struct ordered_map * map, void * key);

bool ordered_map_is_empty(const struct ordered_map * map);

void ordered_map_free(struct ordered_map * map);

#endif

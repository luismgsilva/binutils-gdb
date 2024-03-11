/* hash.h -- header file for gas hash table routines
   Copyright (C) 1987-2023 Free Software Foundation, Inc.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#ifndef HASH_H
#define HASH_H

struct string_tuple
{
  const char *key;
  const void *value;
};

typedef struct string_tuple string_tuple_t;

/* Hash function for a string_tuple.  */

extern hashval_t hash_string_tuple (const void *);

/* Equality function for a string_tuple.  */

extern int eq_string_tuple (const void *, const void *);

/* Insert ELEMENT into HTAB.  If REPLACE is non-zero existing elements
   are overwritten.  If ELEMENT already exists, a pointer to the slot
   is returned.  Otherwise NULL is returned.  */

extern void **htab_insert (htab_t, void * /* element */, int /* replace */);

/* Print statistics about a hash table.  */

extern void htab_print_statistics (FILE *f, const char *name, htab_t table);

/* Inline string hash table functions.  */

/* "htab_t table": The hash table to which this tuple will belong.
 * "const char *key": The key associated with the tuple.
 * "const void *value": The value associated with the tuple. */
static inline string_tuple_t *
string_tuple_alloc (htab_t table, const char *key, const void *value)
{
  string_tuple_t *tuple = table->alloc_f (1, sizeof (*tuple));
  tuple->key = key;
  tuple->value = value;
  return tuple;
}

static inline void *
str_hash_find (htab_t table, const char *key)
{
  string_tuple_t needle = { key, NULL };
  string_tuple_t *tuple = htab_find (table, &needle);
  return tuple != NULL ? (void *) tuple->value : NULL;
}

static inline void *
str_hash_find_n (htab_t table, const char *key, size_t n)
{
  char *tmp = XNEWVEC (char, n + 1);
  memcpy (tmp, key, n);
  tmp[n] = '\0';
  string_tuple_t needle = { tmp, NULL };
  string_tuple_t *tuple = htab_find (table, &needle);
  free (tmp);
  return tuple != NULL ? (void *) tuple->value : NULL;
}

static inline void
str_hash_delete (htab_t table, const char *key)
{
  string_tuple_t needle = { key, NULL };
  htab_remove_elt (table, &needle);
}


/*
 * "htab_t table" - This is a hash table where the key-value pair
 * will be inserted.
 * "const char *key" - The key to be inserted into the hash table.
 * "const void *value" - The value associated with the key.
 * "int replace" - A flag indicating whether to replace teh value
 * if the key already exists in the hash table. */
static inline void **
str_hash_insert (htab_t table, const char *key, const void *value, int replace)
{
  /* A new "string_tuple_t" object is allocated to create a tuple
   * containing the key and the value. */
  string_tuple_t *elt = string_tuple_alloc (table, key, value);

  /* The "htab_insert" function is called to insert the newly
   * created tuple into the hash table.
   * It returns a pointer to the slot where the key-value
   * pair is inserted. If the key already exists in the
   * hash table and "replace" is set to false, it returns
   * a pointer to the existing slot without replacing the value. */
  void **slot = htab_insert (table, elt, replace);

  /* If a slot is returned (ei, insertion is successful,
   * and its not replacing an existing value), and "replace"
   * is false, and "table->free_f" is not NULL (indicating
   * there is a free function specified for the hash table),
   * then the memory allocated for the string tuple "elt" is
   * freed. This conditional check ensures that memory is
   * freed only if necessary and avoids calling the free function
   * if its not provided. */
  if (slot && !replace && table->free_f)
    table->free_f (elt);
  return slot;
}

static inline htab_t
str_htab_create (void)
{
  return htab_create_alloc (16, hash_string_tuple, eq_string_tuple,
			    NULL, notes_calloc, NULL);
}

#endif /* HASH_H */

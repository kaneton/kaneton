/*
 * tests the SET_TYPE_ARRAY
 *
 *
 */
void		set_test_array(int test_set)
{
  t_uint32	nalloc = alloc_nalloc();
  t_uint32	nfree = alloc_nfree();
  t_setid	id;
  t_setid	cloned;
  t_id		data;
  t_id		objs[64];
  t_id*		pdata;
  t_iterator	it;
  t_state	state;
  int		i;

  cons_msg('#', "testing SET_TYPE_ARRAY\n");

  if (test_set == -1 || test_set == 0)
    {
      cons_msg('#', "testing with no options\n");

      if (set_reserve(array, 0, 4, sizeof(t_id), &id) !=
	  ERROR_NONE)
	cons_msg('!', "error: set_reserve()\n");

      set_show(id);

      objs[0] = 80LL;
      objs[1] = 98654LL;
      objs[2] = 42LL;
      objs[3] = 122LL;
      objs[4] = 45LL;
      objs[5] = 64LL;
      if (set_add(id, &objs[0]) != ERROR_NONE ||
	  set_add(id, &objs[1]) != ERROR_NONE ||
	  set_add(id, &objs[2]) != ERROR_NONE ||
	  set_add(id, &objs[3]) != ERROR_NONE)
	cons_msg('!', "error set_add()\n");

      many_remove(id, 1, 42LL);

      if (set_add(id, &objs[4]) != ERROR_NONE ||
	  set_add(id, &objs[5]) != ERROR_NONE)
	cons_msg('!', "error set_add()\n");

      set_show(id);

      set_release(id);
    }

  if (test_set == -1 || test_set == 1)
    {
      cons_msg('#', "testing with SET_OPT_ALLOC\n");

      if (set_reserve(array, SET_OPT_ALLOC, 4, sizeof(t_id), &id) !=
	  ERROR_NONE)
	cons_msg('!', "error: set_reserve()\n");

      many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

      many_remove(id, 1, 42LL);

      many_add(id, 2, 45LL, 64LL);

      set_show(id);

      set_release(id);
    }

  if (test_set == -1 || test_set == 2 || test_set == 3 || test_set == 4 ||
      test_set == 5 || test_set == 6)
    {
      cons_msg('#', "testing with SET_OPT_ORGANISE | SET_OPT_ALLOC\n");

      if (set_reserve(array, SET_OPT_ORGANISE | SET_OPT_ALLOC, 1,
		      sizeof(t_id), &id) != ERROR_NONE)
	cons_msg('!', "error: set_reserve()\n");

      many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

      many_remove(id, 1, 42LL);

      many_add(id, 2, 45LL, 64LL);

      set_show(id);

      if (test_set == 2)
	set_release(id);
    }

  if (test_set == -1 || test_set == 3)
    {
      cons_msg('#', "testing iterators\n");

      printf("--> ");
      set_foreach(SET_OPT_FORWARD, id, &it, state)
	{
	  if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	    cons_msg('!', "error set_object\n");
	  printf("%qd ", *pdata);
	}
      printf("\n");

      printf("<-- ");
      set_foreach(SET_OPT_BACKWARD, id, &it, state)
	{
	  if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	    cons_msg('!', "error set_object\n");
	  printf("%qd ", *pdata);
	}
      printf("\n");

      if (test_set == 3)
	set_release(id);
    }

  if (test_set == -1 || test_set == 4)
    {
      cons_msg('#', "testing insert_*\n");

      objs[0] = 4LL;
      if (set_insert_head(id, &objs[0]) != ERROR_NONE)
	cons_msg('!', "error in insert_head\n");

      objs[1] = 123456LL;
      if (set_insert_tail(id, &objs[1]) != ERROR_NONE)
	cons_msg('!', "error in insert_tail\n");

      set_head_array(id, &it);
      set_next_array(id, it, &it);
      objs[2] = 456LL;
      if (set_insert_after(id, it, &objs[2]) != ERROR_NONE)
	cons_msg('!', "error in insert_after\n");

      objs[3] = 454LL;
      if (set_insert_before(id, it, &objs[3]) != ERROR_NONE)
	cons_msg('!', "error in insert_before\n");

      set_show(id);

      if (test_set == 4)
	set_release(id);
    }

  if (test_set == -1 || test_set == 5)
    {
      cons_msg('#', "testing locate\n");

      if (set_locate(id, 42LL, &it) == ERROR_NONE)
	cons_msg('!', "error with locate: found a non-existent element !\n");

      if (set_locate(id, 122LL, &it) != ERROR_NONE)
	cons_msg('!', "error with locate\n");
      else
	{
	  if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	    cons_msg('!', "error set_object\n");
	  printf("%qd found\n", *pdata, it.u.array.i);
	}

      if (set_locate(id, 98654LL, &it) != ERROR_NONE)
	cons_msg('!', "error with locate\n");
      else
	{
	  if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	    cons_msg('!', "error set_object\n");
	  printf("%qd found\n", *pdata, it.u.array.i);
	}

      if (test_set == 5)
	set_release(id);
    }

  if (test_set == -1 || test_set == 6)
    {
      cons_msg('#', "testing flush\n");

      set_flush(id);

      set_show(id);

      set_release(id);
    }

  if (test_set == -1 || test_set == 7 || test_set == 8)
    {
      cons_msg('#', "testing with SET_OPT_SORT\n");

      if (set_reserve(array, SET_OPT_SORT | SET_OPT_ALLOC, 1,
		      sizeof(t_id), &id) != ERROR_NONE)
	cons_msg('!', "error: set_reserve()\n");

      many_add(id, 4, 80LL, 98654LL, 42LL, 122LL);

      many_remove(id, 1, 42LL);

      many_add(id, 2, 45LL, 64LL);

      set_show(id);

      if (test_set == 7)
	set_release(id);
    }

  if (test_set == -1 || test_set == 8)
    {
      cons_msg('#', "testing clone function\n");

      if (set_clone(id, &cloned) != ERROR_NONE)
	cons_msg('!', "error in set_clone\n");

      set_release(id);

      many_add(cloned, 1, 89LL);

      set_show(cloned);

      set_release(cloned);
    }

  if (test_set == -1 || test_set == 9)
    {
      cons_msg('#', "testing with a longer list & sorting\n");

      if (set_reserve(array, SET_OPT_SORT, 4,
		      sizeof(t_id), &id) != ERROR_NONE)
	cons_msg('!', "error set_reserve()\n");

      for (i = 0; i < 64; ++i)
	{
	  objs[i] = (i * 234) % 6578;
	  if (set_add(id, &objs[i]) != ERROR_NONE)
	    cons_msg('!', "error set_add()\n");
	}

      printf("--> ");
      set_foreach(SET_OPT_FORWARD, id, &it, state)
	{
	  if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
	    cons_msg('!', "error set_object\n");
	  printf("%qd ", *pdata);
	}
      printf("\n");

      for (i = 0; i < 64; ++i)
	{
	  if (set_locate(id, objs[i], &it) != ERROR_NONE)
	    cons_msg('!', "error with locate %qd\n", objs[i]);
	  else
	    {
	      if (set_object(id, it, (void**)&pdata) != ERROR_NONE)
		cons_msg('!', "error set_object\n");
	      if (*pdata != objs[i])
		cons_msg('!', "error in returned object %qd\n", objs[i]);
	    }
	}

      data = objs[5] - 1;
      if (set_locate(id, data, &it) == ERROR_NONE)
	cons_msg('!', "found a non-existent value !\n");

      set_release(id);
    }

  if (test_set == -1 || test_set == 10)
    {
      cons_msg('#', "testing with empty list\n");

      if (set_reserve(array, 0, 4, sizeof(t_id), &id) != ERROR_NONE)
	cons_msg('!', "error set_reserve()\n");

      if (set_locate(id, 42LL, &it) == ERROR_NONE)
	cons_msg('!', "error set_locate()\n");

      if (set_remove(id, 42LL) == ERROR_NONE)
	cons_msg('!', "error set_remove()\n");

      set_foreach(SET_OPT_FORWARD, id, &it, state)
	{
	  cons_msg('!', "error !\n");
	}

      objs[0] = 42LL;
      if (set_insert_tail(id, &objs[0]) != ERROR_NONE)
	cons_msg('!', "error set_insert_tail()\n");

      set_show(id);

      set_release(id);
    }

  if ((nalloc - nfree) != (alloc_nalloc() - alloc_nfree()))
    cons_msg('!', "error: memory leaks detected: %u/%u -> %u/%u\n",
	     nalloc, nfree, alloc_nalloc(), alloc_nfree());
  else
    cons_msg('#', "no memory leak detected\n");
}

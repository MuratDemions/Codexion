/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:35 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:35 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int cap)
{
	h->data = malloc(sizeof(t_req) * cap);
	h->size = 0;
	h->cap = cap;
	return (h->data != NULL);
}

int	heap_push(t_heap *h, t_req r)
{
	if (h->size >= h->cap)
		return (0);
	h->data[h->size] = r;
	sift_up(h, h->size);
	h->size++;
	return (1);
}

void	heap_remove(t_heap *h, int id)
{
	int	i;

	i = 0;
	while (i < h->size && h->data[i].coder_id != id)
		i++;
	if (i == h->size)
		return ;
	h->size--;
	h->data[i] = h->data[h->size];
	if (i < h->size)
	{
		sift_up(h, i);
		sift_down(h, i);
	}
}

int	heap_find(t_heap *h, int id, t_req *out)
{
	int	i;

	i = 0;
	while (i < h->size)
	{
		if (h->data[i].coder_id == id)
		{
			*out = h->data[i];
			return (1);
		}
		i++;
	}
	return (0);
}

void	heap_free(t_heap *h)
{
	free(h->data);
	h->data = NULL;
	h->size = 0;
}

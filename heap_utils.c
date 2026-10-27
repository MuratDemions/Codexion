/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:41 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:41 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	req_less(t_req *a, t_req *b)
{
	if (a->key != b->key)
		return (a->key < b->key);
	return (a->coder_id < b->coder_id);
}

void	swap_req(t_req *a, t_req *b)
{
	t_req	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sift_up(t_heap *h, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!req_less(&h->data[i], &h->data[parent]))
			break ;
		swap_req(&h->data[i], &h->data[parent]);
		i = parent;
	}
}

void	sift_down(t_heap *h, int i)
{
	int	small;
	int	l;
	int	r;

	while (1)
	{
		small = i;
		l = 2 * i + 1;
		r = 2 * i + 2;
		if (l < h->size && req_less(&h->data[l], &h->data[small]))
			small = l;
		if (r < h->size && req_less(&h->data[r], &h->data[small]))
			small = r;
		if (small == i)
			return ;
		swap_req(&h->data[i], &h->data[small]);
		i = small;
	}
}

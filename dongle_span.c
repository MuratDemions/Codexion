/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_span.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:56:09 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:56:09 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	insert_sorted(int *idx, int *cnt, int v)
{
	int	i;

	i = 0;
	while (i < *cnt)
	{
		if (idx[i] == v)
			return ;
		i++;
	}
	i = *cnt;
	while (i > 0 && idx[i - 1] > v)
	{
		idx[i] = idx[i - 1];
		i--;
	}
	idx[i] = v;
	(*cnt)++;
}

int	span_indices(t_sim *sim, int id, int *idx)
{
	int	n;
	int	i;
	int	cnt;

	n = sim->p.n;
	i = id - 1;
	cnt = 0;
	insert_sorted(idx, &cnt, (i + n - 1) % n);
	insert_sorted(idx, &cnt, i);
	insert_sorted(idx, &cnt, (i + 1) % n);
	insert_sorted(idx, &cnt, (i + 2) % n);
	return (cnt);
}

void	lock_span(t_sim *sim, int *idx, int cnt)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		pthread_mutex_lock(&sim->dongles[idx[i]].mutex);
		i++;
	}
}

void	unlock_span(t_sim *sim, int *idx, int cnt)
{
	while (cnt > 0)
	{
		cnt--;
		pthread_mutex_unlock(&sim->dongles[idx[cnt]].mutex);
	}
}

void	notify_span(t_coder *c)
{
	int	idx[4];
	int	cnt;
	int	i;
	int	k;

	cnt = span_indices(c->sim, c->id, idx);
	i = 0;
	while (i < cnt)
	{
		k = 0;
		while (k < 2)
		{
			if (c->sim->dongles[idx[i]].users[k])
				wake_coder(c->sim->dongles[idx[i]].users[k]);
			k++;
		}
		i++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:56:03 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:56:03 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	dongle_ok(t_coder *c, t_dongle *d)
{
	return (!d->in_use && !is_blocked(c, d));
}

static int	claim_pair(t_coder *c)
{
	t_dongle	*a;
	t_dongle	*b;

	a = c->first;
	b = c->second;
	c->wake_at = 0;
	if (!dongle_ok(c, a) || !dongle_ok(c, b))
		return (0);
	c->wake_at = a->free_at;
	if (b->free_at > c->wake_at)
		c->wake_at = b->free_at;
	if (now_ms() < c->wake_at)
		return (0);
	a->in_use = 1;
	b->in_use = 1;
	heap_remove(&a->queue, c->id);
	heap_remove(&b->queue, c->id);
	return (1);
}

static int	try_take(t_coder *c)
{
	int	idx[4];
	int	cnt;
	int	ok;

	cnt = span_indices(c->sim, c->id, idx);
	lock_span(c->sim, idx, cnt);
	ok = claim_pair(c);
	unlock_span(c->sim, idx, cnt);
	return (ok);
}

static void	wait_turn(t_coder *c)
{
	struct timespec	ts;

	if (c->wake_at > 0)
	{
		ms_to_timespec(c->wake_at, &ts);
		pthread_cond_timedwait(&c->wake, &c->wait_mutex, &ts);
	}
	else
		pthread_cond_wait(&c->wake, &c->wait_mutex);
}

int	acquire_pair(t_coder *c)
{
	int	ok;

	enqueue_pair(c);
	ok = 0;
	pthread_mutex_lock(&c->wait_mutex);
	while (!sim_stopped(c->sim))
	{
		ok = try_take(c);
		if (ok)
			break ;
		wait_turn(c);
	}
	pthread_mutex_unlock(&c->wait_mutex);
	if (!ok)
		return (0);
	notify_span(c);
	log_state(c, "has taken a dongle");
	log_state(c, "has taken a dongle");
	return (1);
}

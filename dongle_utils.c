/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:56:06 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:56:06 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	make_key(t_coder *c, t_dongle *d)
{
	if (c->sim->p.sched == EDF)
		return (c->last_start + c->sim->p.burnout);
	d->ticket++;
	return (d->ticket);
}

void	enqueue_pair(t_coder *c)
{
	t_req	r;

	r.coder_id = c->id;
	pthread_mutex_lock(&c->first->mutex);
	pthread_mutex_lock(&c->second->mutex);
	r.key = make_key(c, c->first);
	heap_push(&c->first->queue, r);
	r.key = make_key(c, c->second);
	heap_push(&c->second->queue, r);
	pthread_mutex_unlock(&c->second->mutex);
	pthread_mutex_unlock(&c->first->mutex);
}

static void	release_one(t_coder *c, t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->in_use = 0;
	d->free_at = now_ms() + c->sim->p.cooldown;
	pthread_mutex_unlock(&d->mutex);
}

void	wake_coder(t_coder *c)
{
	pthread_mutex_lock(&c->wait_mutex);
	pthread_cond_signal(&c->wake);
	pthread_mutex_unlock(&c->wait_mutex);
}

void	release_pair(t_coder *c)
{
	release_one(c, c->first);
	release_one(c, c->second);
	notify_span(c);
}

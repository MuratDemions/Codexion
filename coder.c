/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:56:16 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:56:16 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	take_dongles(t_coder *c)
{
	if (c->second == NULL)
	{
		log_state(c, "has taken a dongle");
		while (!sim_stopped(c->sim))
			usleep(500);
		return (0);
	}
	return (acquire_pair(c));
}

static void	do_compile(t_coder *c)
{
	pthread_mutex_lock(&c->sim->state);
	c->last_start = now_ms();
	pthread_mutex_unlock(&c->sim->state);
	log_state(c, "is compiling");
	sleep_ms(c->sim, c->sim->p.compile);
	release_pair(c);
	pthread_mutex_lock(&c->sim->state);
	c->compiles++;
	pthread_mutex_unlock(&c->sim->state);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;

	c = (t_coder *)arg;
	while (!sim_stopped(c->sim))
	{
		if (!take_dongles(c))
			break ;
		do_compile(c);
		log_state(c, "is debugging");
		sleep_ms(c->sim, c->sim->p.debug);
		log_state(c, "is refactoring");
		sleep_ms(c->sim, c->sim->p.refactor);
	}
	return (NULL);
}

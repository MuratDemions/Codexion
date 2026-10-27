/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:26 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:26 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state);
	stopped = sim->stop;
	pthread_mutex_unlock(&sim->state);
	return (stopped);
}

void	sim_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->state);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->state);
}

void	wake_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->p.n)
	{
		pthread_mutex_lock(&sim->coders[i].wait_mutex);
		pthread_cond_broadcast(&sim->coders[i].wake);
		pthread_mutex_unlock(&sim->coders[i].wait_mutex);
		i++;
	}
}

void	log_state(t_coder *c, char *msg)
{
	t_sim	*sim;

	sim = c->sim;
	pthread_mutex_lock(&sim->log);
	if (!sim_stopped(sim))
		printf("%ld %d %s\n", now_ms() - sim->start, c->id, msg);
	pthread_mutex_unlock(&sim->log);
}

void	log_burnout(t_sim *sim, int id)
{
	pthread_mutex_lock(&sim->log);
	if (!sim_stopped(sim))
	{
		sim_stop(sim);
		printf("%ld %d burned out\n", now_ms() - sim->start, id);
	}
	pthread_mutex_unlock(&sim->log);
}

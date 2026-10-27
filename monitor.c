/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:20 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:20 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	find_burnout(t_sim *sim)
{
	int		i;
	long	last;

	i = 0;
	while (i < sim->p.n)
	{
		pthread_mutex_lock(&sim->state);
		last = sim->coders[i].last_start;
		pthread_mutex_unlock(&sim->state);
		if (now_ms() - last > sim->p.burnout)
			return (i + 1);
		i++;
	}
	return (0);
}

static int	all_done(t_sim *sim)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	pthread_mutex_lock(&sim->state);
	while (i < sim->p.n)
	{
		if (sim->coders[i].compiles < sim->p.required)
			done = 0;
		i++;
	}
	pthread_mutex_unlock(&sim->state);
	return (done);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		id;

	sim = (t_sim *)arg;
	while (1)
	{
		if (all_done(sim))
			break ;
		id = find_burnout(sim);
		if (id)
		{
			log_burnout(sim, id);
			break ;
		}
		usleep(500);
	}
	sim_stop(sim);
	wake_all(sim);
	return (NULL);
}

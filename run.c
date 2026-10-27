/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:02 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:02 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_coders(t_sim *sim)
{
	int		i;
	t_coder	*c;

	i = 0;
	while (i < sim->p.n)
	{
		sim->coders[i].last_start = sim->start;
		i++;
	}
	i = 0;
	while (i < sim->p.n)
	{
		c = &sim->coders[i];
		if (pthread_create(&c->thread, NULL, coder_routine, c) != 0)
			return (i);
		i++;
	}
	return (i);
}

static void	join_coders(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	run_sim(t_sim *sim)
{
	int	started;
	int	ok;

	sim->start = now_ms();
	started = start_coders(sim);
	ok = (started == sim->p.n);
	if (ok)
		ok = (pthread_create(&sim->monitor, NULL, monitor_routine, sim) == 0);
	if (ok)
		pthread_join(sim->monitor, NULL);
	else
	{
		sim_stop(sim);
		wake_all(sim);
	}
	join_coders(sim, started);
	return (ok);
}

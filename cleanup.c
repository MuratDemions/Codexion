/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:56:18 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:56:18 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_dongles(t_sim *sim, int count)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		heap_free(&sim->dongles[i].queue);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

void	destroy_coders(t_sim *sim, int count)
{
	int	i;

	if (!sim->coders)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_cond_destroy(&sim->coders[i].wake);
		pthread_mutex_destroy(&sim->coders[i].wait_mutex);
		i++;
	}
	free(sim->coders);
	sim->coders = NULL;
}

void	destroy_sim(t_sim *sim)
{
	destroy_coders(sim, sim->p.n);
	destroy_dongles(sim, sim->p.n);
	pthread_mutex_destroy(&sim->state);
	pthread_mutex_destroy(&sim->log);
}

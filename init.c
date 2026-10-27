/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-26 15:55:30 by musipit           #+#    #+#             */
/*   Updated: 2026-10-26 15:55:30 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_sim(t_sim *sim)
{
	sim->stop = 0;
	sim->start = 0;
	sim->dongles = NULL;
	sim->coders = NULL;
	if (pthread_mutex_init(&sim->state, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->log, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->state);
		return (0);
	}
	if (init_dongles(sim) && init_coders(sim))
		return (1);
	destroy_sim(sim);
	return (0);
}

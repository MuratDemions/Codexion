/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr> #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-27 09:34:29 by musipit           #+#    #+#             */
/*   Updated: 2026-10-27 09:34:29 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongle(t_dongle *d)
{
	d->in_use = 0;
	d->free_at = 0;
	d->ticket = 0;
	d->users[0] = NULL;
	d->users[1] = NULL;
	if (!heap_init(&d->queue, 2))
		return (0);
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
	{
		heap_free(&d->queue);
		return (0);
	}
	return (1);
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->p.n);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->p.n)
	{
		if (!init_dongle(&sim->dongles[i]))
		{
			destroy_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:32 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:32 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	register_user(t_dongle *d, t_coder *c)
{
	if (d->users[0] == NULL)
		d->users[0] = c;
	else
		d->users[1] = c;
}

static void	assign_dongles(t_sim *sim, t_coder *c, int i)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % sim->p.n;
	c->second = NULL;
	c->first = &sim->dongles[left];
	register_user(&sim->dongles[left], c);
	if (left == right)
		return ;
	c->second = &sim->dongles[right];
	register_user(&sim->dongles[right], c);
	if (right < left)
	{
		c->first = &sim->dongles[right];
		c->second = &sim->dongles[left];
	}
}

static int	init_coder(t_sim *sim, int i)
{
	t_coder	*c;

	c = &sim->coders[i];
	c->id = i + 1;
	c->sim = sim;
	c->compiles = 0;
	c->last_start = 0;
	c->wake_at = 0;
	if (pthread_mutex_init(&c->wait_mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&c->wake, NULL) != 0)
	{
		pthread_mutex_destroy(&c->wait_mutex);
		return (0);
	}
	assign_dongles(sim, c, i);
	return (1);
}

int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->p.n);
	if (!sim->coders)
		return (0);
	i = 0;
	while (i < sim->p.n)
	{
		if (!init_coder(sim, i))
		{
			destroy_coders(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

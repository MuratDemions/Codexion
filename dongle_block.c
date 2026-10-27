/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_block.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:56:12 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:56:12 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongle	*other_dongle(t_coder *y, t_dongle *d)
{
	if (y->first == d)
		return (y->second);
	return (y->first);
}

int	is_blocked(t_coder *c, t_dongle *d)
{
	t_req	mine;
	t_req	theirs;
	t_coder	*y;

	y = d->users[0];
	if (y == c)
		y = d->users[1];
	if (y == NULL || y == c)
		return (0);
	if (!heap_find(&d->queue, c->id, &mine)
		|| !heap_find(&d->queue, y->id, &theirs))
		return (0);
	if (!req_less(&theirs, &mine))
		return (0);
	return (!other_dongle(y, d)->in_use);
}

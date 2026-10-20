/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:55:15 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:55:15 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_num(char *s, long *out)
{
	long	v;
	int		i;

	if (!s || !s[0] || strlen(s) > 10)
		return (0);
	v = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		v = v * 10 + (s[i] - '0');
		i++;
	}
	if (v > 2147483647L)
		return (0);
	*out = v;
	return (1);
}

static int	read_numbers(char **argv, long *v)
{
	int	i;

	i = 0;
	while (i < 7)
	{
		if (!parse_num(argv[i + 1], &v[i]))
		{
			fprintf(stderr, "Error: invalid argument '%s'\n", argv[i + 1]);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	read_sched(char *s, t_sched *sched)
{
	if (strcmp(s, "fifo") == 0)
		*sched = FIFO;
	else if (strcmp(s, "edf") == 0)
		*sched = EDF;
	else
	{
		fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n");
		return (0);
	}
	return (1);
}

static int	set_params(long *v, t_params *p)
{
	if (v[0] < 1 || v[0] > MAX_CODERS || v[1] < 1 || v[2] < 1 || v[5] < 1)
	{
		fprintf(stderr, "Error: argument out of range\n");
		return (0);
	}
	p->n = (int)v[0];
	p->burnout = v[1];
	p->compile = v[2];
	p->debug = v[3];
	p->refactor = v[4];
	p->required = (int)v[5];
	p->cooldown = v[6];
	return (1);
}

int	parse_args(int argc, char **argv, t_params *p)
{
	long	v[7];

	if (argc != 9)
	{
		fprintf(stderr, "Usage: %s number_of_coders time_to_burnout "
			"time_to_compile time_to_debug time_to_refactor "
			"number_of_compiles_required dongle_cooldown "
			"scheduler(fifo|edf)\n", argv[0]);
		return (0);
	}
	if (!read_numbers(argv, v) || !read_sched(argv[8], &p->sched))
		return (0);
	return (set_params(v, p));
}

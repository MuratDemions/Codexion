/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musipit <musipit@student.42kocaeli.com.tr>  #+#  +:+      +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-10-19 19:59:34 by musipit           #+#    #+#             */
/*   Updated: 2026-10-19 19:59:34 by musipit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>


# define MAX_CODERS 10000

typedef enum e_sched
{
	FIFO,
	EDF
}	t_sched;

typedef struct s_params
{
	int		n;
	long	burnout;
	long	compile;
	long	debug;
	long	refactor;
	int		required;
	long	cooldown;
	t_sched	sched;
}	t_params;


struct	s_sim;
struct	s_coder;


typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	long			last_start;
	int				compiles;
	pthread_mutex_t	wait_mutex;
	pthread_cond_t	wake;
	long			wake_at;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_params		p;
	long			start;
	int				stop;
	pthread_mutex_t	state;
	pthread_mutex_t	log;
	t_coder			*coders;
	pthread_t		monitor;
}	t_sim;

int		parse_args(int argc, char **argv, t_params *p);

#endif

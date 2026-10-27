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
# include <sys/time.h>

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

typedef struct s_req
{
	long	key;
	int		coder_id;
}	t_req;

typedef struct s_heap
{
	t_req	*data;
	int		size;
	int		cap;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				in_use;
	long			free_at;
	long			ticket;
	t_heap			queue;
	struct s_coder	*users[2];
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*first;
	t_dongle		*second;
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
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor;
}	t_sim;

long	now_ms(void);

int		init_dongle(t_dongle *d);
int		init_dongles(t_sim *sim);
int		parse_args(int argc, char **argv, t_params *p);
int		init_sim(t_sim *sim);
int		init_coders(t_sim *sim);
int		run_sim(t_sim *sim);
int		sim_stopped(t_sim *sim);
int		heap_init(t_heap *h, int cap);
int		heap_push(t_heap *h, t_req r);
int		heap_find(t_heap *h, int id, t_req *out);
int		req_less(t_req *a, t_req *b);
int		acquire_pair(t_coder *c);
int		is_blocked(t_coder *c, t_dongle *d);
int		span_indices(t_sim *sim, int id, int *idx);

void	lock_span(t_sim *sim, int *idx, int cnt);
void	unlock_span(t_sim *sim, int *idx, int cnt);
void	notify_span(t_coder *c);
void	enqueue_pair(t_coder *c);
void	release_pair(t_coder *c);
void	wake_coder(t_coder *c);
void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);
void	destroy_sim(t_sim *sim);
void	destroy_dongles(t_sim *sim, int count);
void	destroy_coders(t_sim *sim, int count);
void	swap_req(t_req *a, t_req *b);
void	sift_up(t_heap *h, int i);
void	sift_down(t_heap *h, int i);
void	heap_free(t_heap *h);
void	sim_stop(t_sim *sim);
void	wake_all(t_sim *sim);
void	log_state(t_coder *c, char *msg);
void	log_burnout(t_sim *sim, int id);
void	heap_remove(t_heap *h, int id);
void	ms_to_timespec(long ms, struct timespec *ts);
void	sleep_ms(t_sim *sim, long ms);

#endif

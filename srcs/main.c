/*
** main.c for philo in /home/marmus_a/Documents/rendu/Y2/S2/PSU/PSU_2016_philo
**
** Made by augustin.marmus
** Login   <augustin.marmus@epitech.eu>
**
** Started on  Mon Mar 13 14:12:53 2017 augustin.marmus
** Last update Sun Mar 19 12:06:54 2017 chalie_a
*/

#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdio.h>
#include	<semaphore.h>
#include	<pthread.h>
#include	"extern.h"
#include	"philo.h"

int	g_cycles;
int	g_nb_philo;
sem_t		g_sem;

static int	parse_args(int ac, char **av)
{
  int		i;

  i = 0;
  while (++i < ac)
    {
      if ((strcmp(av[i], "-p") == 0) && av[i + 1])
	g_nb_philo = atoi(av[i + 1]);
      else if ((strcmp(av[i], "-e") == 0) && av[i + 1])
	g_cycles = atoi(av[i + 1]);
    }
  if (g_cycles > 0 && g_nb_philo > 1)
    return (0);
  return (1);
}

void		after_sleep(t_philo *data)
{
  if (data->prev->state == SLEEP && sem_trywait(&g_sem))
    {
      pthread_mutex_lock(&(data->chopsticks));
      pthread_mutex_lock(&(data->next->chopsticks));
      lphilo_take_chopstick(&(data->chopsticks));
      lphilo_take_chopstick(&(data->next->chopsticks));
      data->nb_eat += 1;
      data->state = EAT;
      lphilo_eat();
      lphilo_release_chopstick(&(data->chopsticks));
      lphilo_release_chopstick(&(data->next->chopsticks));
      pthread_mutex_unlock(&(data->chopsticks));
      pthread_mutex_unlock(&(data->next->chopsticks));
      sem_post(&g_sem);
    }
  else if (sem_trywait(&g_sem))
    {
      pthread_mutex_lock(&(data->chopsticks));
      lphilo_take_chopstick(&(data->chopsticks));
      data->state = THINK;
      lphilo_think();
      lphilo_release_chopstick(&(data->chopsticks));
      pthread_mutex_unlock(&(data->chopsticks));
      sem_post(&g_sem);
    }
}

void		after_think(t_philo *data)
{
  sem_wait(&g_sem);
  pthread_mutex_lock(&(data->next->chopsticks));
  pthread_mutex_lock(&(data->chopsticks));
  lphilo_take_chopstick(&(data->next->chopsticks));
  lphilo_take_chopstick(&(data->chopsticks));
  data->nb_eat += 1;
  data->state = EAT;
  lphilo_eat();
  lphilo_release_chopstick(&(data->next->chopsticks));
  lphilo_release_chopstick(&(data->chopsticks));
  pthread_mutex_unlock(&(data->chopsticks));
  pthread_mutex_unlock(&(data->next->chopsticks));
  sem_post(&g_sem);
}

void		*main_loop(void *tmp)
{
  t_philo	*data;

  data = (t_philo *)tmp;
  while (check_end(data) == 0)
    {
      if (data->state == THINK)
	after_think(data);
      else if (data->state == EAT)
	{
	  lphilo_sleep();
	  data->state = SLEEP;
	}
      else if (data->state == SLEEP)
	after_sleep(data);
      usleep(100 * g_nb_philo);
    }
  return (NULL);
}

int			main(int ac, char **av)
{
  int			i;
  void			*handler;
  t_philo		*philos;
  t_philo		*tmp;

  if (parse_args(ac, av) != 0)
    return (1);
  RCFStartup(ac, av);
  philos = initialize_data();
  tmp = philos;
  i = -1;
  while (++i < g_nb_philo)
    {
      pthread_create(&(tmp->thread), NULL, main_loop, tmp);
      tmp = tmp->next;
    }
  i = -1;
  tmp = philos;
  while (++i < g_nb_philo)
    {
      pthread_join(tmp->thread, NULL);
      tmp = tmp->next;
    }
  RCFCleanup();
  return (0);
}

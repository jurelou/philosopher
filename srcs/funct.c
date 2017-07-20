/*
** main.c for philo in /home/marmus_a/Documents/rendu/Y2/S2/PSU/PSU_2016_philo
**
** Made by augustin.marmus
** Login   <augustin.marmus@epitech.eu>
**
** Started on  Mon Mar 13 14:12:53 2017 augustin.marmus
** Last update Sun Mar 19 11:51:45 2017 chalie_a
*/

#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdio.h>
#include	<semaphore.h>
#include	<pthread.h>
#include	"extern.h"
#include	"philo.h"

int		check_end(t_philo *list)
{
  t_philo	*start;

  start = list;
  if (list->nb_eat == g_cycles)
    return (1);
  list = list->next;
  while (list != start)
    {
      if (list->nb_eat == g_cycles)
	return (1);
      list = list->next;
    }
  return (0);
}

void			first_occur(t_philo *tmp)
{
  if ((tmp->id % 2) == 0)
    tmp->state = EAT;
  else
    tmp->state = SLEEP;
  if ((tmp->id) == (g_nb_philo - 1) && (g_nb_philo % 2) == 1)
    tmp->state = THINK;
}

t_philo			*new_philo(t_philo *curs, int g_nb_philo, int g_cycles, int id)
{
  t_philo		*tmp;
  t_philo		*start;

  start = curs;
  if (curs)
    while (curs->next)
      curs = curs->next;
  if ((tmp = malloc(1 * sizeof(t_philo))) == NULL)
    return (NULL);
  tmp->nb_eat = 0;
  tmp->nb_philo = g_nb_philo;
  tmp->cycles = g_cycles;
  tmp->id = id;
  tmp->prev = curs;
  first_occur(tmp);
  pthread_mutex_init(&(tmp->chopsticks), NULL);
  if (curs != NULL)
    curs->next = tmp;
  tmp->next = NULL;
  if (start)
    return (start);
  return (tmp);
}

t_philo		*initialize_data()
{
  t_philo		*start;
  t_philo		*res;
  int			i;

  i = 0;
  res = NULL;
  while (i < g_nb_philo)
    {
      res = new_philo(res, g_nb_philo, g_cycles, i);
      i++;
    }
  start = res;
  while (res->next != NULL)
    res = res->next;
  res->next = start;
  start->prev = res;
  sem_init(&g_sem, 0, (g_nb_philo / 2));
  return (start);
}

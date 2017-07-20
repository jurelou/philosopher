#ifndef		PHILO_H_
# define	PHILO_H_

#include	<semaphore.h>

extern int    g_cycles;
extern int    g_nb_philo;
extern sem_t           g_sem;

typedef enum	e_state
  {
    SLEEP,
    EAT,
    THINK
  }		t_state;

typedef struct	s_philo
{
  int			id;
  pthread_mutex_t       chopsticks;
  int			nb_eat;
  t_state		state;
  pthread_t		thread;
  int			nb_philo;
  int			cycles;
  struct s_philo	*next;
  struct s_philo	*prev;
}		t_philo;

int		check_end(t_philo *);
void		first_occur(t_philo *);
t_philo		*new_philo(t_philo *curs, int g_nb_philo, int g_cycles, int id);
t_philo		*initialize_data();

#endif		/* !PHILO_H_ */

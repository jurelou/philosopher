##                                                                                                      
## Makefile for  in /home/jurczy_l/PSU_2016_nmobjdump                                                   
##                                                                                                      
## Made by jurczyk louis                                                                                

## Login   <jurczy_l@epitech.net>                                                                       
##                                                                                                      
## Started on  Sun Feb 26 12:54:28 2017 jurczyk louis                                                   
##

CC	=	gcc

rm	=	rm -f

CFLAGS	=	-I include -g

LDFLAGS	=	-pthread -L ./ -lriceferee

NAME	=	philo

SRCS	=	srcs/main.c \
		srcs/funct.c

OBJS	=	$(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

debug:		$(OBJS)
		$(CC) $(OBJS) -o test

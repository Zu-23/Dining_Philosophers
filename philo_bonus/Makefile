CC=gcc
FLAGS= -Wall -Werror -Wextra
SRC= ../philo/time_func.c \
init_utils.c \
philo_and_watcher.c \
philo_status.c \
bonus_philosophers.c \


OBJ= $(SRC:.c=.o)
COMPILE= philo_bonus

all: $(COMPILE)
	
%.o: %.c 
	$(CC) $(FLAGS) -c $< -o $@
$(COMPILE): $(OBJ) philo_bonus.h
	$(CC) $(FLAGS) $(OBJ) -o $(COMPILE)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(COMPILE)
re: fclean all

.PHONY: all clean fclean re
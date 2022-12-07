CC=gcc
FLAGS= -Wall -Werror -Wextra -g -pthread
SRC= init_utils.c \
philo_and_watcher.c \
philo_status.c \
time_func.c \
philosophers.c

OBJ= $(SRC:.c=.o)
COMPILE= philo

all: $(COMPILE)
	
%.o: %.c 
	$(CC) $(FLAGS) -c $< -o $@
$(COMPILE): $(OBJ) philo.h
	$(CC) $(FLAGS) $(OBJ) -o $(COMPILE)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) $(COMPILE)
re: fclean all

.PHONY: all clean fclean re
CC=gcc
FLAGS= -Wall -Werror -Wextra
AR= ar rcs
SRC= init_utils.c \
philo_and_watcher.c \
philo_status.c \
philosophers.c \
time_func.c \

OBJ= $(SRC:.c=.o)
NAME= philo.a
COMPILE= philo

all: $(NAME) $(OBJ) philo.h
	$(CC) $(FLAGS) $(SRC) -o $(OBJ)

%.o: %.c
	$(CC) -C $< -o $@

$(COMPILE): $(NAME)
	$(CC) $(FLAGS) $(NAME) -o $(COMPILE)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME) $(COMPILE)
re: fclean all

.PHONY: all clean fclean re
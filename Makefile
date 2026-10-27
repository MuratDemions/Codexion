NAME		= codexion
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		= main.c parse.c init.c cleanup.c run.c init_dongles.c coder.c monitor.c \
			  dongle.c dongle_utils.c dongle_block.c dongle_span.c init_coders.c heap.c heap_utils.c log.c time_utils.c
OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.c codexion.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

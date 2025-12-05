CC = c++

CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lsimdjson

NAME = Directory

SRCS =	main.cpp \
		Directory.cpp \
		Config.cpp \
		Person.cpp \
		Socket.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	@$(MAKE) all

.PHONY: all clean fclean re

NAME = webserv
SRCS = ./server/main.cpp ./server/configSyntax.cpp ./helpers/helpers.cpp ./server/configParser.cpp \
		./server/virtualServer.cpp ./server/locationBlock.cpp
OBJS = $(SRCS:.cpp=.o)
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CPPFLAGS += -I./includes

all: $(NAME)

%.o:%.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
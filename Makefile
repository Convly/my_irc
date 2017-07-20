##
## Makefile for makefile in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/server/
##
## Made by JSX
## Login   <jean-sebastien.herbaux@epitech.eu>
##
## Started on  Wed May 31 16:16:02 2017 JSX
## Last update Sun Jun 11 18:53:27 2017 pazimor pazimor
##

NAME	=	server

RM	=	rm -f

CP	=	cp

SRCS	=	Srcs/server.c 			\
		Srcs/init.c	 		\
		Srcs/errors.c 			\
		Srcs/exec.c 			\
		Srcs/clientUtils.c 		\
		Srcs/roomUtils_2.c 		\
		Srcs/roomsUtils.c 		\
		Srcs/commands.c 		\
		Srcs/parser.c 			\
		Srcs/circularBuffer.c		\
		Srcs/wordtab.c	 		\
		Srcs/communication.c		\
		Srcs/queue.c			\
		Srcs/queueUtils.c		\
		Srcs/utils.c			\
		Srcs/cmd/nick.c 		\
		Srcs/cmd/user.c 		\
		Srcs/cmd/join.c 		\
		Srcs/cmd/quit.c 		\
		Srcs/cmd/names.c 		\
		Srcs/cmd/part.c 		\
		Srcs/cmd/privmsg.c 		\
		Srcs/cmd/kick.c 		\
		Srcs/cmd/topic.c 		\

OBJS	=	$(SRCS:.c=.o)

CC	=	gcc

CFLAGS	=	-W -Wall -Werror -Wextra -I./Includes/

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

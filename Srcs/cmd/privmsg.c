/*
** privmsg.c for privmsg.c in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sat Jun 10 19:59:03 2017 JSX
** Last update Sun Jun 11 22:51:03 2017 JSX
*/

#include "irc.h"

char*		getmsg(char** av,
		       char* msg)
{
  char*		tmp;
  int		it;
  int		k;
  char*		str;

  k = it = 0;
  str = av[0];
  while (k < 3)
    {
      while (str[it] == ' ' || str[it] == '\t')
	it++;
      k++;
      while (k < 3 && str[it] != ' ' && str[it] != '\t')
      	it++;
    }
  tmp = &(str[it]);
  if (*tmp == ':')
    tmp++;
  if (strlen(tmp) > RSIZE_XL - 50)
    tmp[RSIZE_XL - 50] = '\0';
  strcpy(msg, tmp);
  return (msg);
}

static int	throwInvalidsArgs(t_irc* irc,
				  int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "461 %s PRIVMSG :Not enough parameters",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 461;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
  return (461);
}

int		throwInvalidTarget(t_irc* irc,
				   int it,
				   char* target)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "401 %s %s :Target invalid",
	      irc->_clients[it].nick, target) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 401;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
  return (-4);
}

static int	sendToUser(t_irc* irc,
			   int it,
			   char** argv)
{
  int		i;
  char		msg[RSIZE_XL];
  char		r[RSIZE_XL];

  i = -1;
  bzero(r, RSIZE_XL);
  bzero(msg, RSIZE_XL);
  if (getmsg(argv, msg) == NULL)
    return (throwInvalidTarget(irc, it, argv[2]));
  while (++i < SOMAXCONN)
    {
      if (irc->_clients[i].fdtype == fdclient &&
	  !strcmp(irc->_clients[i].nick, argv[2]))
	{
	  if (sprintf(r, "PRIVMSG %s :%s", argv[2], msg) < 0)
	    xerror("Can't write : spritnf");
	  irc->_ecode = 0;
	  setReply(irc, it, r, i);
	  triggerReply(irc);
	  return (-4);
	}
    }
  return (throwInvalidTarget(irc, it, argv[2]));
}

int		privmsgEvent(t_irc* irc,
			     int it,
			     char** argv)
{
  if (wordTabSize(argv) < 4 && throwInvalidsArgs(irc, it))
    return (-4);
  if (*argv[2] == '#' || *argv[2] == '&')
    sendRoomMessage(irc, it, argv);
  else
    sendToUser(irc, it, argv);
  return (-4);
}

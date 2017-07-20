/*
** user.c for user in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Wed Jun  7 23:55:11 2017 JSX
** Last update Sun Jun 11 22:46:55 2017 JSX
*/

#include "irc.h"

void		accessConnect(t_irc* irc,
			      int it)
{
  char		r[RSIZE_XL];

  irc->_clients[it].isLog = 1;
  bzero(r, RSIZE_XL);
  if (sprintf(r, "001 %s :Welcome to my amazing irc",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : fprintf");
  irc->_ecode = 001;
  setReply(irc, it, r, TRIGGER_UONLY);
}

static int	throwInvalidsArgs(t_irc* irc,
				  int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "461 %s USER :Not enough parameters",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 461;
  return (setReply(irc, it, r, TRIGGER_UONLY));
}

static int	throwAlreadyRegistered(t_irc* irc,
				       int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "462 %s :You're already registered",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 462;
  return (setReply(irc, it, r, TRIGGER_UONLY));
}

int		userEvent(t_irc* irc,
			  int it,
			  char** argv)
{
  int		tcpy;

  if (irc->_clients[it].isLog)
    return (throwAlreadyRegistered(irc, it));
  else if (wordTabSize(argv) <= 5)
    return (throwInvalidsArgs(irc, it));
  bzero(irc->_clients[it].username, 64);
  tcpy = ((tcpy = strlen(argv[4])) > 64)? 64: tcpy;
  memcpy(irc->_clients[it].username, argv[4], tcpy);
  if (*(irc->_clients[it].username) && *(irc->_clients[it].nick))
    accessConnect(irc, it);
  return (-2);
}

/*
** nick.c for nick in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 19:22:57 2017 JSX
** Last update Sun Jun 11 22:45:51 2017 JSX
*/

#include "irc.h"

static int	checkNickNameValidity(t_irc* irc,
				      int it,
				      char* nick)
{
  int		i;
  char		reply[RSIZE_XL];

  i = -1;
  bzero(reply, RSIZE_XL);
  if (strlen(nick) > 9 && (irc->_ecode = 432))
    {
      sprintf(reply, "432 %s %s :Invalid nickname",
	      (irc->_clients[it].isLog)?
	      irc->_clients[it].nick: "NICK", nick);
      return (setReply(irc, it, reply, TRIGGER_UONLY));
    }
  while (++i < SOMAXCONN)
    {
      if (it != i && irc->_clients[i].fdtype == fdclient &&
	  !strcmp(nick, irc->_clients[i].nick))
	{
	  sprintf(reply, "433 %s %s :Nickname already in use",
		  (irc->_clients[it].isLog)?
		  irc->_clients[it].nick: "", nick);
	  irc->_ecode = 433;
	  return (setReply(irc, it, reply, TRIGGER_UONLY));
	}
    }
  return (-2);
}

static void	setNickname(t_irc* irc,
			    int it,
			    char* nick)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (irc->_clients[it].isLog)
    {
      if (sprintf(r, ":%s NICK %s", irc->_clients[it].nick, nick) < 0)
	xerror("Can't write : asprintf");
      irc->_ecode = 0;
      setReply(irc, it, r, TRIGGER_ALL);
    }
  bzero(irc->_clients[it].nick, 9);
  if (!memcpy(irc->_clients[it].nick, nick, strlen(nick)))
    xerror("Can't copy memory");
  if (!(irc->_clients[it].isLog) &&
      *(irc->_clients[it].username) && *(irc->_clients[it].nick))
    accessConnect(irc, it);
  if (fprintf(stderr, "[SERVER INFOS]: Client (%d) is now known as %s\n",
	      it, irc->_clients[it].nick) < 0)
    xerror("Can't write : fprintf");
}

int		nickEvent(t_irc* irc,
			  int it,
			  char** argv)
{
  int		e;
  char		reply[RSIZE_XL];

  e = -2;
  bzero(reply, RSIZE_XL);
  if (wordTabSize(argv) == 2)
    {
      sprintf(reply, "431 %s NICK :No nickname given",
	      (irc->_clients[it].isLog)? irc->_clients[it].nick: "");
      irc->_ecode = 431;
      return (setReply(irc, it, reply, TRIGGER_UONLY));
    }
  if ((e = checkNickNameValidity(irc, it, argv[2])) >= 0)
    return (e);
  setNickname(irc, it, argv[2]);
  return (e);
}

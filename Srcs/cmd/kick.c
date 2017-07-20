/*
** kick.c for kick.c in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sun Jun 11 17:51:53 2017 JSX
** Last update Sun Jun 11 22:51:30 2017 JSX
*/

#include "irc.h"

static int	throwInvalidsArgs(t_irc* irc,
				  int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "461 %s KICK :Not enough parameters",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 461;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
  return (-4);
}

static int	throwInvalidRoom(t_irc* irc,
				 int it,
				 char* channelName)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "403 %s %s :Invalid channel name",
	      irc->_clients[it].nick, channelName) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 403;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
  return (-4);
}

static int	kickEventAction(t_irc* irc,
				int it,
				char** av,
				int rid)
{
  char		r[RSIZE_XL];
  char*		room;
  char*		target;
  int		itt;

  bzero(r, RSIZE_XL);
  room = av[2];
  target = av[3];
  if ((itt = getClientIdByNick(irc, target)) == -1 ||
      irc->_rooms[rid].users[itt] != 1)
    return (throwInvalidTarget(irc, it, target));
  if (sprintf(r, ":%s KICK %s %s",
	      irc->_clients[it].nick, room, target) < 0)
    xerror("Can't write : sprintf");
  sendRoomAction(irc, it, rid, r);
  irc->_rooms[rid].users[itt] = 0;
  return (-4);
}

int		kickEvent(t_irc* irc,
			  int it,
			  char** av)
{
  char		r[RSIZE_XL];
  int		i;
  int		rid;
  char		*channel;

  if (wordTabSize(av) < 4)
    return (throwInvalidsArgs(irc, it));
  i = rid = -1;
  channel = av[2];
  bzero(r, RSIZE_XL);
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, channel))
	{
	  rid = i;
	  break;
	}
    }
  if (rid != -1)
    return (kickEventAction(irc, it, av, rid));
  else
    return (throwInvalidRoom(irc, it, channel));
}

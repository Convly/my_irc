/*
** roomUtils_2.c for roomutils in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sat Jun 10 17:04:14 2017 JSX
** Last update Sun Jun 11 22:36:40 2017 JSX
*/

#include "irc.h"

int		throwNotOnChannel(t_irc* irc,
				  int it,
				  char* channel)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "442 %s %s :You're not on that channel",
	      irc->_clients[it].nick, channel) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 442;
  setReply(irc, it, r, TRIGGER_UONLY);
  return (442);
}

int		removeClientFromRoom(t_irc* irc,
				     int it,
				     char* room)
{
  int		i;
  char		r[RSIZE_XL];

  i = -1;
  bzero(r, RSIZE_XL);
  if (sprintf(r, ":%s PART %s", irc->_clients[it].nick, room) < 0)
    xerror("Can't write : sprintf");
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, room))
	{
	  sendRoomAction(irc, it, i, r);
	  irc->_rooms[i].users[it] = 0;
	  return (-4);
	}
    }
  return (-1);
}

int			sendToAllChannelUsers(t_irc* irc, int roomId,
					      char* msg, int emitter)
{
  int			i;
  char			r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "PRIVMSG %s :%s", irc->_rooms[roomId].name, msg) < 0)
    xerror("Can't write : spritnf");
  i = -1;
  while (++i < SOMAXCONN)
    {
      if (irc->_rooms[roomId].users[i] == 1 && i != emitter)
	{
	  irc->_ecode = 0;
	  setReply(irc, emitter, r, i);
	  triggerReply(irc);
	}
    }
  return (-4);
}

int			sendRoomMessage(t_irc* irc,
					int it,
					char** argv)
{
  int			i;
  char			msg[RSIZE_XL];

  i = -1;
  bzero(msg, RSIZE_XL);
  if (getmsg(argv, msg) == NULL)
    return (throwInvalidTarget(irc, it, argv[2]));
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, argv[2]))
	{
	  if (irc->_rooms[i].users[it] != 1)
	    return (throwNotOnChannel(irc, it, argv[2]) &&
		    triggerReply(irc));
	  return (sendToAllChannelUsers(irc, i, msg, it));
	}
    }
  return (throwInvalidTarget(irc, it, argv[2]));
}

int			sendRoomAction(t_irc* irc,
				       int it,
				       int roomId,
				       char* r)
{
  int			i;

  i = -1;
  while (++i < SOMAXCONN)
    {
      if (irc->_rooms[roomId].users[i] == 1)
	{
	  irc->_ecode = 0;
	  setReply(irc, it, r, i);
	  triggerReply(irc);
	}
    }
  return (-4);
}

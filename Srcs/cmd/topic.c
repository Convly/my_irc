/*
** topic.c for topic in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sun Jun 11 19:19:14 2017 JSX
** Last update Sun Jun 11 22:51:50 2017 JSX
*/

#include "irc.h"

static int	throwInvalidsArgs(t_irc* irc,
				  int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "461 %s TOPIC :Not enough parameters",
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

static int	newTopic(t_irc* irc,
			 int it,
			 int rid,
			 char** av)
{
  char		r[RSIZE_XL];
  char		msg[RSIZE_XL];

  bzero(r, RSIZE_XL);
  bzero(msg, RSIZE_XL);
  if (getmsg(av, msg) == NULL)
    return (0);
  if (irc->_rooms[rid].topic != NULL)
    free(irc->_rooms[rid].topic);
  irc->_rooms[rid].topic = strdup(msg);
  if (sprintf(r, ":%s TOPIC %s :%s",
	      irc->_clients[it].nick, irc->_rooms[rid].name, msg) < 0)
    xerror("Can't write : spritnf");
  sendRoomAction(irc, it, rid, r);
  return (-4);
}

int		topicEvent(t_irc* irc,
			   int it,
			   char** av)
{
  int		i;
  int		rid;

  i = rid = -1;
  if (wordTabSize(av) < 4)
    return (throwInvalidsArgs(irc, it));
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, av[2]))
	{
	  rid = i;
	  break;
	}
    }
  if (rid == -1)
    return (throwInvalidRoom(irc, it, av[2]));
  else
    return (newTopic(irc, it, rid, av));
}

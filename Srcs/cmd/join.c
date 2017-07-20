/*
** join.c for join in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Thu Jun  8 19:15:36 2017 JSX
** Last update Sun Jun 11 22:47:17 2017 JSX
*/

#include "irc.h"

static int	throwInvalidsArgs(t_irc* irc,
				  int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "461 %s JOIN :Not enough parameters",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 461;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
  return (461);
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
  return (403);
}

static int	joinRooms(t_irc* irc,
			  int it,
			  char* subChans)
{
  if (*(subChans) == '#' || *(subChans) == '&')
    {
      addClientInRoom(irc, it, subChans);
      listRoom(irc, it, subChans);
    }
  else
    throwInvalidRoom(irc, it, subChans);
  return (1);
}

int		joinEvent(t_irc* irc,
			  int it,
			  char** argv)
{
  char		r[RSIZE_XL];
  int		i;
  char		*tmp;

  bzero(r, RSIZE_XL);
  i = 2;
  if (wordTabSize(argv) < 3 && throwInvalidsArgs(irc, it))
    return (-4);
  while (argv[i])
    {
      tmp = strtok(argv[i], ",");
      while (tmp)
	{
	  joinRooms(irc, it, tmp);
	  tmp = strtok(NULL, ",");
	}
      i++;
    }
  return (-4);
}

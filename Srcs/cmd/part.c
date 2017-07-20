/*
** part.c for part.c in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sat Jun 10 16:54:57 2017 JSX
** Last update Sun Jun 11 22:50:29 2017 JSX
*/

#include "irc.h"

static int	throwInvalidsArgs(t_irc* irc,
				  int it)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  if (sprintf(r, "461 %s PART :Not enough parameters",
	      irc->_clients[it].nick) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 461;
  setReply(irc, it, r, TRIGGER_UONLY);
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
  return (403);
}

static int	partRooms(t_irc* irc,
			  int it,
			  char* chan)
{
  if (*chan == '#' || *chan == '&')
    {
      if (removeClientFromRoom(irc, it, chan) == -1)
	throwInvalidRoom(irc, it, chan);
    }
  else
    throwInvalidRoom(irc, it, chan);
  return (1);
}

int		partEvent(t_irc* irc,
			  int it,
			  char** argv)
{
  char		r[RSIZE_XL];
  char		*tmp;

  bzero(r, RSIZE_XL);
  if (wordTabSize(argv) < 3 && throwInvalidsArgs(irc, it))
    return (0);
  tmp = strtok(argv[2], ",");
  while (tmp)
    {
      partRooms(irc, it, tmp);
      tmp = strtok(NULL, ",");
    }
  return (0);
}

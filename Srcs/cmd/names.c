/*
** names.c for names in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sat Jun 10 01:24:35 2017 JSX
** Last update Sun Jun 11 22:50:04 2017 JSX
*/

#include "irc.h"

static void	sendUsersInRoom(t_irc* irc,
				int it,
				int rid,
				char* room)
{
  char		r[RSIZE_XL];
  int		x;
  int		i;

  bzero(r, RSIZE_XL);
  if (sprintf(r, "353 %s %s :", irc->_clients[it].nick, room) < 0)
    xerror("Can't write : sprintf");
  i = x = -1;
  while (++i < SOMAXCONN)
    {
      if (irc->_rooms[rid].users[i] == 1)
	{
	  if (x != -1)
	    strcat(r, " ");
	  x = 1;
	  strcat(r, irc->_clients[i].nick);
	}
    }
  irc->_ecode = 353;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
}

int		listRoom(t_irc* irc,
			 int it,
			 char* room)
{
  char		r[RSIZE_XL];
  int		rid;
  int		i;

  i = rid = -1;
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, room))
	rid = i;
    }
  if (rid == -1)
    return (-1);
  sendUsersInRoom(irc, it, rid, room);
  bzero(r, RSIZE_XL);
  if (sprintf(r, "366 %s %s :End of name list",
	      irc->_clients[it].nick, room) < 0)
    xerror("Can't write : sprintf");
  irc->_ecode = 366;
  setReply(irc, it, r, TRIGGER_UONLY);
  triggerReply(irc);
  return (1);
}

static int	listActiveRooms(t_irc* irc,
				int it)
{
  int		i;

  i = -1;
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED)
	listRoom(irc, it, irc->_rooms[i].name);
    }
  return (-4);
}

int		namesEvent(t_irc* irc,
			   int it,
			   char** argv)
{
  char		r[RSIZE_XL];
  int		i;
  char		*tmp;

  bzero(r, RSIZE_XL);
  i = 2;
  irc->_clients[it].isLog = 1;
  if (wordTabSize(argv) < 3)
    return (listActiveRooms(irc, it));
  while (argv[i])
    {
      tmp = strtok(argv[i], ",");
      while (tmp)
	{
	  if (*tmp == '#' || *tmp == '&')
	    listRoom(irc, it, tmp);
	  tmp = strtok(NULL, ",");
	}
      i++;
    }
  return (-4);
}

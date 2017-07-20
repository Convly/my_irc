/*
** server.c for server in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/server/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 00:21:59 2017 JSX
** Last update Sun Jun 11 19:42:37 2017 pazimor pazimor
*/

#include "irc.h"

static void	irc_clean(t_irc* irc)
{
  int		it;

  it = -1;
  while (++it < SOMAXCONN)
    {
      if (irc->_clients[it].fdtype == fdclient)
	eraseClient(irc, it);
    }
  it = -1;
  while (++it < RSIZE_M)
    {
      if (irc->_rooms[it].state == SLUSED)
	deleteRoom(irc, it);
    }
  if (fprintf(stderr, "[SERVER INFOS]: Server cleaned successfully\n") < 0)
    xerror("can't write : fprintf");
}

int		main(int ac,
		     char **av,
		     char **env)
{
  t_irc	irc;

  if (!irc_init(&irc, ac, av, env))
    return (0);
  irc_exec(&irc);
  irc_clean(&irc);
  return (0);
}

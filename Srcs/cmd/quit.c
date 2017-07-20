/*
** quit.c for quit in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/cmd/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Fri Jun  9 02:08:13 2017 JSX
** Last update Sun Jun 11 22:47:49 2017 JSX
*/

#include "irc.h"

int		quitEvent(t_irc* irc,
			  int it,
			  char** argv)
{
  char		r[RSIZE_XL];

  bzero(r, RSIZE_XL);
  irc->_ecode = -84;
  if (irc->_clients[it].isLog)
    {
      if (sprintf(r, ":%s QUIT %s", irc->_clients[it].nick,
		  (wordTabSize(argv) > 2)? argv[2]: ":Bye bye") < 0)
	xerror("Can't write : fprintf");
      setReply(irc, it, r, TRIGGER_ALL);
    }
  else
    {
      if (sprintf(r, "QUIT %s",
		  (wordTabSize(argv) > 2)? argv[2]: ":Bye bye") < 0)
	xerror("Can't write : fprintf");
      setReply(irc, it, r, TRIGGER_UONLY);
    }
  return (-2);
}

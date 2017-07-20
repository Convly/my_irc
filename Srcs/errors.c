/*
** errors.c for errors in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/server/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 00:24:51 2017 JSX
** Last update Sun Jun 11 19:30:25 2017 pazimor pazimor
*/

#include "irc.h"

void	xerror(char* msg)
{
  fprintf(stderr, "[IRC] - Fatal error: %s\n",
	  (msg)? msg: "Unkown exception");
  exit(EXIT_FAILURE);
}

int	rerror(char* msg, int ret)
{
  if (fprintf(stderr, "[IRC] - error: %s\n",
	      (msg)? msg: "Unknown exception") < 0)
    xerror("can't write : fprintf");
  return (ret);
}

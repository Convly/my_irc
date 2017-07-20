/*
** utils.c for utils in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Thu Jun  8 03:44:55 2017 JSX
** Last update Sun Jun 11 19:42:57 2017 pazimor pazimor
*/

#include "irc.h"

int	strOccInStr(char* src,
		    char* tfind)
{
  int	k;
  char*	tmp;

  k = 0;
  tmp = src;
  while ((tmp = strstr(tmp, tfind)))
    {
      k++;
      tmp++;
    }
  return (k);
}

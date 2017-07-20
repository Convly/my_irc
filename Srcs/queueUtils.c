/*
** queueUtils.c for queueutils in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sun Jun 11 04:02:52 2017 JSX
** Last update Sun Jun 11 19:37:39 2017 pazimor pazimor
*/

#include "irc.h"

int		getQueueSize(t_queue* q)
{
  int		k;

  k = 0;
  while (q && q->next)
    {
      k++;
      q = q->next;
    }
  return (k);
}

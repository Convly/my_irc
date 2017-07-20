/*
** communication.c for communication in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Thu Jun  8 00:59:52 2017 JSX
** Last update Sun Jun 11 19:29:57 2017 pazimor pazimor
*/

#include "irc.h"

static void	callBackSigPipe(int e)
{
  if (fprintf(stderr, "[SERVER INFOS]: It's boken af...(%d)\n", e) < 0)
    xerror("Can't write : fprintf");
}

int		sendClientMessage(t_irc* irc,
				  t_talk t)
{
  int		i;
  int		it;
  int		trigger;

  i = 4;
  if (t.code == -1)
    return (-1);
  it = t.owner;
  trigger = t.trigger;
  while (i < SOMAXCONN)
    {
      if (irc->_clients[i].fdtype == fdclient &&
	  ((i != it && trigger == TRIGGER_ALL) ||
	   (i == it && (int)trigger < (int)TRIGGER_OTHER) ||
	   (i == (int)trigger) ||
	   (trigger == TRIGGER_ALL)))
	queuePushNode(irc->_clients[i].queue, &t);
      i++;
    }
  return (t.code);
}

int		writeClientMessage(t_irc* irc,
				   int it)
{
  t_queue*	q;

  q = irc->_clients[it].queue;
  if (!q || !(q->next))
    return (-1);
  if (dprintf(it, "%s\r\n", q->data->msg) < 0)
    xerror("Can't write : dprintf");
  if (q->data->code == -84 &&
      irc->_clients[q->data->owner].fdtype == fdclient)
    eraseClient(irc, q->data->owner);
  else
    irc->_clients[it].queue = queuePopNode(irc->_clients[it].queue);
  signal(SIGPIPE, callBackSigPipe);
  return (0);
}

/*
** queue.c for queue in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Sun Jun 11 03:41:26 2017 JSX
** Last update Sun Jun 11 19:36:56 2017 pazimor pazimor
*/

#include "irc.h"

t_talk*			getNewTalkPtr(t_talk* t)
{
  t_talk*		ptr;

  if (!(ptr = malloc(sizeof(t_talk))))
    xerror("Can't allocate memory");
  ptr->code = t->code;
  ptr->trigger = t->trigger;
  ptr->owner = t->owner;
  bzero(ptr->msg, RSIZE_XL);
  strcpy(ptr->msg, t->msg);
  return (ptr);
}

t_queue*		createQueue()
{
  t_queue*		q;

  if (!(q = malloc(sizeof(t_queue))))
    xerror("Can't allocate memory");
  q->next = NULL;
  q->data = NULL;
  return (q);
}

void			deleteQueue(t_queue* q)
{
  t_queue*		tmp;
  int			k;

  tmp = q;
  k = 0;
  while ((tmp = queuePopNode(tmp)))
    k++;
}

t_queue*		queuePopNode(t_queue* q)
{
  t_queue*		qt;

  qt = q;
  if (!q || !(q->next))
    return (NULL);
  q = q->next;
  free(qt->data);
  free(qt);
  qt = NULL;
  return (q);
}

void			queuePushNode(t_queue* q,
				      t_talk* t)
{
  t_queue*		nq;
  t_queue*		rnode;

  nq = createQueue();
  rnode = q;
  while (rnode && rnode->next)
    rnode = rnode->next;
  rnode->data = getNewTalkPtr(t);
  rnode->next = nq;
}

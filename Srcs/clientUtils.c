/*
** clientUtils.c for clientutils in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 02:45:15 2017 JSX
** Last update Sun Jun 11 22:34:29 2017 JSX
*/

#include "irc.h"

int		setReply(t_irc* irc,
			 int owner,
			 char* msg,
			 t_triggertype trigger)
{
  irc->_reply.code = irc->_ecode;
  bzero(irc->_reply.msg, RSIZE_XL);
  memcpy(irc->_reply.msg, msg, RSIZE_XL);
  irc->_reply.trigger = trigger;
  irc->_reply.owner = owner;
  return (irc->_ecode);
}

void		eraseClient(t_irc* irc,
			    int it)
{
  if (irc->_clients[it].queue)
    {
      deleteQueue(irc->_clients[it].queue);
      irc->_clients[it].queue = NULL;
    }
  irc->_clients[it].fdtype = fdfree;
  bzero(irc->_clients[it].nick, 32);
  if (!memset(&(irc->_clients[it].socket), 0,
	      sizeof(irc->_clients[it].socket)))
    xerror("Can't memset");
  if (irc->_clients[it].isLog)
    free(irc->_clients[it].cbuf.buffer);
  irc->_clients[it].isLog = 0;
  if (fprintf(stderr, "[SERVER INFOS]: Client disconnected\t(%d)\n",
	      it) < 0)
    xerror("can't write : fprintf");
  if (close(it) == -1)
    rerror("Failed to close socket", 0);
}

void			addNewClient(t_irc* irc)
{
  int			fc;
  int			fsize;
  struct sockaddr_in	sin;

  fsize = sizeof(sin);
  fc = accept(irc->_serverInfos.fd,
	      (struct sockaddr *)&sin, (socklen_t *)&fsize);
  irc->_clients[fc].fd = fc;
  irc->_clients[fc].fdtype = fdclient;
  irc->_clients[fc].isLog = 0;
  cbInit(&(irc->_clients[fc].cbuf), RSIZE_XL);
  irc->_clients[fc].queue = createQueue();
  if (!memcpy(&(irc->_clients[fc].socket), &sin, fsize))
    xerror("can't copy memory");
  if (fprintf(stderr, "[SERVER INFOS]: New client connected\t(%d)\n",
	      fc) < 0)
    xerror("can't write : fprintf");
}

int			clientRead(void* cbuf,
				   int fd)
{
  if (read(fd, (char*)cbuf, RSIZE_L) <= 0)
    {
      cbuf = NULL;
      return (0);
    }
  return (1);
}

int			getClientIdByNick(t_irc* irc,
					  char* nick)
{
  int			i;

  i = -1;
  while (++i < SOMAXCONN)
    {
      if (irc->_clients[i].fdtype == fdclient &&
	  !strcmp(irc->_clients[i].nick, nick))
	return (i);
    }
  return (-1);
}

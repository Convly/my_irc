/*
** exec.c for exec in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 02:12:17 2017 JSX
** Last update Sun Jun 11 19:31:39 2017 pazimor pazimor
*/

#include "irc.h"

static int		g_xexit = 1;

static void		handleExitSigInt(int e)
{
  if (fprintf(stderr, "[SERVER INFOS]: SIGINT received (%d)\n",
	      e) < 0)
    xerror("Can't write : fprintf");
  g_xexit = 0;
}

static void		resetSet(t_irc* irc)
{
  int			it;
  t_clientInfos		tmpClient;

  it = 0;
  FD_ZERO(&(irc->_fdset));
  FD_ZERO(&(irc->_wfdset));
  FD_SET(irc->_serverInfos.fd, &(irc->_fdset));
  while (it < SOMAXCONN)
    {
      tmpClient = irc->_clients[it];
      if (tmpClient.fdtype == fdclient && tmpClient.fd > 2)
	{
	  FD_SET(tmpClient.fd, &(irc->_fdset));
	  FD_SET(tmpClient.fd, &(irc->_wfdset));
	}
      it++;
    }
}

static void		selectFD(t_irc* irc)
{
  int			it;

  if (select(FD_SETSIZE, &(irc->_fdset), &(irc->_wfdset), NULL,
	     &(irc->_serverInfos.tv)) == -1)
    {
      rerror("Failed to execute select", 0);
      return;
    }
  it = -1;
  while (++it < SOMAXCONN)
    {
      if (FD_ISSET(it, &(irc->_fdset)))
	handleCommand(irc, it);
      while (irc->_clients[it].fdtype == fdclient &&
	     irc->_clients[it].queue->next &&
	     FD_ISSET(it, &(irc->_wfdset)))
	writeClientMessage(irc, it);
    }
}

void			irc_exec(t_irc* irc)
{
  signal(SIGINT, handleExitSigInt);
  while (irc->_run && g_xexit)
    {
      resetSet(irc);
      selectFD(irc);
    }
}

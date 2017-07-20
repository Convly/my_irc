/*
** commands.c for commands in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 03:26:39 2017 JSX
** Last update Sun Jun 11 19:28:25 2017 pazimor pazimor
*/

#include "irc.h"

void		retrieveFdCmd(t_irc* irc,
			      int it)
{
  char*		buff;
  int		size;

  if (!(buff = malloc(sizeof(char) * (RSIZE_L + 1))))
    xerror("Can't allocate more memory");
  bzero(buff, RSIZE_L + 1);
  if (irc->_clients[it].fdtype == fdclient &&
      !(irc->_clients[it].readEvent(buff, it)))
    eraseClient(irc, it);
  else
    {
      size = strlen(buff);
      if (strOccInStr(buff, "\r\n") == 0)
	{
	  buff[size] = '\r';
	  buff[size + 1] = '\n';
	}
      addStringInCircularBuffer(&(irc->_clients[it].cbuf), buff);
      commandParser(irc, it);
    }
  free(buff);
}

void		retrieveServerCmd(t_irc* irc)
{
  char		buff[RSIZE_M];

  bzero(buff, RSIZE_M);
  if (read(0, buff, RSIZE_M) <= 0)
    {
      if (fprintf(stderr,
		  "[SERVER INFOS]: About to stop the server...\n") < 0)
	xerror("Can't write : fprintf");
      irc->_run = 0;
      return;
    }
}

void		handleCommand(t_irc* irc,
			      int it)
{
  if (it == irc->_serverInfos.fd)
    addNewClient(irc);
  else
    retrieveFdCmd(irc, it);
}

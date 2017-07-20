/*
** init.c for init in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/server/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 00:31:38 2017 JSX
** Last update Sun Jun 11 22:34:00 2017 JSX
*/

#include "irc.h"

static int	initRooms(t_irc* irc)
{
  int		i;

  i = -1;
  while (++i < RSIZE_M)
    {
      irc->_rooms[i].state = SLFREE;
      irc->_rooms[i].topic = NULL;
      irc->_rooms[i].name = NULL;
      memset(irc->_rooms[i].users, -1, sizeof(irc->_rooms[i].users));
    }
  return (1);
}

static int	retrieveArgsInfos(t_irc* irc,
				  int ac,
				  char** av,
				  char** env)
{
  char		r[RSIZE_XL];

  if (!env || !*env)
    xerror("We've failed to retrieve the environement");
  if (ac != 2)
    xerror("Bad number of args for the execution");
  if (!strcmp(av[1], CMD_HELP))
    {
      fprintf(stdout, "USAGE: %s port\n", av[0]);
      return (0);
    }
  else if ((irc->_serverInfos.port = atoi(av[1])) < 1025)
    xerror("Bad port (must be a positive number > 1024)");
  irc->_bin.env = env;
  irc->_bin.args = av;
  irc->_bin.nbArgs = ac;
  irc->_bin.binaryName = av[0];
  irc->_nbClient = 0;
  irc->_run = 1;
  if (fprintf(stderr, "[SERVER INFOS]: Commons infos retrieved\n") < 0)
    xerror("can't write : fprintf");
  bzero(r, RSIZE_XL);
  irc->_ecode = -1;
  return (setReply(irc, -1, r, TRIGGER_ALL));
}

static void	createServer(t_irc* irc)
{
  int		sockSize;

  irc->_serverInfos.serverSocket.sin_family = AF_INET;
  irc->_serverInfos.serverSocket.sin_port = htons(irc->_serverInfos.port);
  irc->_serverInfos.serverSocket.sin_addr.s_addr = INADDR_ANY;
  if (!(irc->protocol = getprotobyname("TCP")))
    xerror("Can't retrieve TCP protocol");
  if ((irc->_serverInfos.fd = socket(AF_INET, SOCK_STREAM,
				     irc->protocol->p_proto)) == -1)
    xerror("Failed to create the server socket");
  sockSize = sizeof(irc->_serverInfos.serverSocket);
  if (setsockopt(irc->_serverInfos.fd, SOL_SOCKET, SO_REUSEPORT,
		 &(int){1}, sizeof(int)) < 0)
    xerror("Can set the socket options");
  if (bind(irc->_serverInfos.fd,
	   (const struct sockaddr*)&(irc->_serverInfos.serverSocket),
	   sockSize) == -1)
    xerror("Failed to bind the serverFd to the serverSocket");
  if (listen(irc->_serverInfos.fd, SOMAXCONN) == -1)
    xerror("Failed to listen to the server");
  irc->_serverInfos.tv.tv_sec = 20;
  irc->_serverInfos.tv.tv_usec = 0;
  if (fprintf(stderr, "[SERVER INFOS]: Server successfully created\n")
      < 0)
    xerror("can't write : fprintf");
}

static void	initClients(t_irc* irc)
{
  int		it;

  it = 0;
  while (it < SOMAXCONN)
    {
      irc->_clients[it].fdtype = fdfree;
      bzero(irc->_clients[it].nick, 9);
      bzero(irc->_clients[it].username, 64);
      irc->_clients[it].cbuf.buffer = NULL;
      irc->_clients[it].isLog = 0;
      irc->_clients[it].fd = -1;
      irc->_clients[it].readEvent = &clientRead;
      irc->_clients[it].queue = NULL;
      it++;
    }
}

int		irc_init(t_irc* irc,
			 int ac,
			 char** av,
			 char** env)
{
  if (!retrieveArgsInfos(irc, ac, av, env))
    return (0);
  createServer(irc);
  initClients(irc);
  initRooms(irc);
  return (1);
}

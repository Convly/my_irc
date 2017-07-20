/*
** roomsUtils.c for roomUtils in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Fri Jun  9 17:00:49 2017 JSX
** Last update Sun Jun 11 22:35:16 2017 JSX
*/

#include "irc.h"

int		addClientInRoomAction(t_irc* irc,
				      int it,
				      int roomId)
{
  int		r;
  char		reply[RSIZE_XL];
  int		tpc;

  r = irc->_rooms[roomId].users[it];
  irc->_rooms[roomId].users[it] = 1;
  bzero(reply, RSIZE_XL);
  if (sprintf(reply, ":%s JOIN %s", irc->_clients[it].nick,
	      irc->_rooms[roomId].name) < 0)
    xerror("Can't write : fprintf");
  sendRoomAction(irc, it, roomId, reply);
  tpc = (irc->_rooms[roomId].topic)? 1: 0;
  bzero(reply, RSIZE_XL);
  if (sprintf(reply, "%d TOPIC %s :%s", (tpc)? 332: 331,
	      irc->_rooms[roomId].name,
	      (tpc)? irc->_rooms[roomId].topic: "No topic is set") < 0)
    xerror("Can't write : fprintf");
  irc->_ecode = (tpc)? 332: 331;
  setReply(irc, it, reply, TRIGGER_UONLY);
  triggerReply(irc);
  return (r);
}

int		addClientInRoom(t_irc* irc,
				int it,
			       char* channel)
{
  int		i;

  i = -1;
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, channel))
	return (addClientInRoomAction(irc, it, i));
    }
  createRoom(irc, channel);
  addClientInRoom(irc, it, channel);
  return (2);
}

int*		getRoomsUserList(t_irc* irc,
				 char* channelName)
{
  int		i;

  i = -1;
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLUSED &&
	  !strcmp(irc->_rooms[i].name, channelName))
	return (irc->_rooms[i].users);
    }
  return (NULL);
}

int		deleteRoom(t_irc* irc,
			   int id)
{
  if (irc->_rooms[id].state == SLUSED)
    {
      if (fprintf(stderr,
		 "[SERVER INFOS]: Room %s is about to being closed\n",
		  irc->_rooms[id].name) < 0)
        xerror("Can't write : fprintf");
      irc->_rooms[id].state = SLFREE;
      if (irc->_rooms[id].topic)
	free(irc->_rooms[id].topic);
      if (irc->_rooms[id].name)
	free(irc->_rooms[id].name);
      memset(irc->_rooms[id].users, -1, sizeof(irc->_rooms[id].users));
    }
  return (0);
}

int		createRoom(t_irc* irc,
			   char* channel)
{
  int		i;

  i = -1;
  while (++i < RSIZE_M)
    {
      if (irc->_rooms[i].state == SLFREE)
    	{
	  if (fprintf(stderr, "[SERVER INFOS]: Creating new room: %s\n",
		      channel) < 0)
	    xerror("Can't write : fprintf");
	  irc->_rooms[i].state = SLUSED;
	  irc->_rooms[i].name = strdup(channel);
	  irc->_rooms[i].topic = NULL;
	  return (1);
	}
    }
  return (0);
}

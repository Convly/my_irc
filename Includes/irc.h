/*
** irc.h for irc in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/server/Includes/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 00:29:12 2017 JSX
** Last update Sun Jun 11 22:31:18 2017 JSX
*/

#ifndef IRC_H_
#define IRC_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

typedef	int(*fdfct_t)(void*, int);
typedef	enum	s_fdtype
{
  fdfree,
  fdclient,
  fdserver
}		t_fdtype;

typedef enum	s_triggertype
{
  TRIGGER_UONLY,
  TRIGGER_ALL,
  TRIGGER_OTHER
}		t_triggertype;

typedef enum	s_logtype
{
  LOG,
  NOLOG
}		t_logtype;

typedef enum	s_slotstate
{
  SLFREE,
  SLUSED
}		t_slotstate;

# define RSIZE_XL	4096
# define RSIZE_L	1024
# define RSIZE_M	512
# define RSIZE_S	1

# define ERR_T_SIZE	6

# define	CMD_HELP	"--help"
# define	CMD_NICK	"NICK"
# define	CMD_USER	"USER"
# define	CMD_JOIN	"JOIN"
# define	CMD_QUIT	"QUIT"
# define	CMD_NAMES	"NAMES"
# define	CMD_PART	"PART"
# define	CMD_PRIVMSG	"PRIVMSG"
# define	CMD_KICK	"KICK"
# define	CMD_TOPIC	"TOPIC"
# define	S_CMD_HELP	6
# define	S_CMD_NICK	4
# define	S_CMD_USER	4
# define	S_CMD_JOIN	4
# define	S_CMD_QUIT	4
# define	S_CMD_NAMES	5
# define	S_CMD_PART	4
# define	S_CMD_PRIVMSG	7
# define	S_CMD_KICK	4
# define	S_CMD_TOPIC	5

typedef struct		s_cbuf
{
  char*			buffer;
  char*			buffer_end;
  size_t		capacity;
  size_t		count;
  size_t		sz;
  char*			head;
  char*			tail;
}			t_cbuf;

typedef	struct		s_serverInfos
{
  int			fd;
  size_t		port;
  struct sockaddr_in	serverSocket;
  struct timeval	tv;
}			t_serverInfos;

typedef struct		s_talk
{
  char			msg[RSIZE_XL];
  int			code;
  t_triggertype		trigger;
  int			owner;
}			t_talk;

typedef struct		s_queue
{
  struct s_queue*	next;
  t_talk*		data;
}			t_queue;

typedef struct		s_clientInfos
{
  t_fdtype		fdtype;
  char			nick[9];
  char			username[64];
  struct sockaddr_in	socket;
  int			fd;
  int			isLog;
  t_cbuf		cbuf;
  fdfct_t		readEvent;
  t_queue*		queue;
}			t_clientInfos;

typedef	struct		s_binInfos
{
  char**		env;
  char**		args;
  int			nbArgs;
  char*			binaryName;
}			t_binInfos;

typedef struct		s_room
{
  t_slotstate		state;
  char*			topic;
  char*			name;
  int			users[SOMAXCONN];
}			t_room;

typedef	struct		s_irc
{
  t_binInfos		_bin;
  t_serverInfos		_serverInfos;
  t_clientInfos		_clients[SOMAXCONN];
  fd_set		_fdset;
  fd_set		_wfdset;
  size_t		_nbClient;
  int			_run;
  int			_ecode;
  t_talk		_reply;
  t_room		_rooms[RSIZE_M];
  struct protoent	*protocol;
}			t_irc;

typedef	struct		s_cmds
{
  int			id;
  char*			command;
  size_t		size;
  int			(*func)(t_irc*, int, char**);
  t_logtype		lg;
}			t_cmds;

extern t_cmds	g_cmdsTab[];

void	xerror(char*);
int	rerror(char*, int);

int	sendClientMessage(t_irc*, t_talk);
int	writeClientMessage(t_irc*, int);

int	irc_init(t_irc*, int, char**, char**);
int	setReply(t_irc*, int, char*, t_triggertype);

void	commandParser(t_irc*, int);
int	triggerReply(t_irc*);

void	irc_exec(t_irc*);

int	strOccInStr(char*, char*);
int	listRoom(t_irc*, int, char*);
char*	getmsg(char**, char*);

void	handleCommand(t_irc*, int);

void	addNewClient(t_irc*);
int	clientRead(void*, int);
int	clientWrite(void*, int);
void	eraseClient(t_irc*, int);
void	accessConnect(t_irc*, int);
int	getClientIdByNick(t_irc*, char*);

int	nickEvent(t_irc*, int, char**);
int	userEvent(t_irc*, int, char**);
int	joinEvent(t_irc*, int, char**);
int	quitEvent(t_irc*, int, char**);
int	namesEvent(t_irc*, int, char**);
int	partEvent(t_irc*, int, char**);
int	privmsgEvent(t_irc*, int, char**);
int	kickEvent(t_irc*, int, char**);
int	topicEvent(t_irc*, int, char**);

int	cbPopFront(t_cbuf*, char*);
int	cbPushBack(t_cbuf*, const char);
void	cbInit(t_cbuf*, size_t);
int	getStringFromCircularBuffer(t_cbuf*, char*);
int	addStringInCircularBuffer(t_cbuf*, char*);

int*	getRoomsUserList(t_irc*, char*);
int	addClientInRoom(t_irc*, int, char*);
int	addClientInRoomAction(t_irc*, int, int);
int	createRoom(t_irc*, char*);
int	deleteRoom(t_irc*, int);
int	removeClientFromRoom(t_irc*, int, char*);
int	throwInvalidTarget(t_irc*, int, char*);
int	sendRoomMessage(t_irc*, int, char**);
int	sendRoomAction(t_irc*, int, int, char*);

int		getQueueSize(t_queue*);
void		queuePushNode(t_queue*, t_talk*);
t_queue*	queuePopNode(t_queue*);
void		deleteQueue(t_queue*);
t_queue*	createQueue();
t_talk*		getNewTalkPtr(t_talk*);

int	wordsInString(char*);
char	**wordTab(char*, int);
int	wordTabSize(char**);
void	displayWordTab(char**);
int	deleteWordTab(char**);

#endif /* end of include guard: IRC_H_ */

/*
** parser.c for parser in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 15:57:26 2017 JSX
** Last update Sun Jun 11 22:38:19 2017 JSX
*/

#include "irc.h"

t_cmds	g_cmdsTab[] =
  {
    {1, CMD_NICK, S_CMD_NICK, &nickEvent, NOLOG},
    {2, CMD_USER, S_CMD_USER, &userEvent, NOLOG},
    {3, CMD_JOIN, S_CMD_JOIN, &joinEvent, LOG},
    {4, CMD_QUIT, S_CMD_QUIT, &quitEvent, NOLOG},
    {5, CMD_NAMES, S_CMD_NAMES, &namesEvent, LOG},
    {6, CMD_PART, S_CMD_PART, &partEvent, LOG},
    {7, CMD_PRIVMSG, S_CMD_PRIVMSG, &privmsgEvent, LOG},
    {8, CMD_KICK, S_CMD_KICK, &kickEvent, LOG},
    {9, CMD_TOPIC, S_CMD_TOPIC, &topicEvent, LOG},
    {-1, "", 0, NULL, NOLOG}
  };

int		triggerReply(t_irc* irc)
{
  t_talk	t;
  char*		triggerDup;

  t = irc->_reply;
  if (t.code == -1)
    return (-1);
  if (t.trigger == TRIGGER_UONLY)
    triggerDup = strdup("USER ONLY");
  else if (t.trigger == TRIGGER_ALL)
    triggerDup = strdup("ALL USERS");
  else
    triggerDup = strdup("OTHER USERS");
  if (fprintf(stderr, "[SERVER REPLY]: [%d][%s] => %s\n",
	      t.owner, triggerDup, t.msg) < 0)
    xerror("Can't write : fprintf");
  sendClientMessage(irc, t);
  free(triggerDup);
  return (t.code);
}

static void	handleRet(t_irc* irc,
			  int it,
			  int ret,
			  char** argv)
{
  char		reply[RSIZE_XL];

  bzero(reply, RSIZE_XL);
  if (ret == -1)
    {
      if (sprintf(reply, "421 %s %s :Unknown command",
		  (irc->_clients[it].isLog)? irc->_clients[it].nick: "",
		  argv[1]) < 0)
	xerror("Can't write : sprintf");
      irc->_ecode = 421;
      setReply(irc, it, reply, TRIGGER_UONLY);
    }
  else if (ret == -3)
    {
      if (sprintf(reply, "451 %s :You're not registered",
		  argv[1]) < 0)
	xerror("Can't write : sprintf");
      irc->_ecode = 451;
      setReply(irc, it, reply, TRIGGER_UONLY);
    }
}

static int	redirectCommand(t_irc* irc,
				int it,
				char* cmd)
{
  int		i;
  char**	argv;
  int		ret;
  char		reply[RSIZE_XL];

  i = ret = -1;
  bzero(reply, RSIZE_XL);
  irc->_ecode = -1;
  setReply(irc, it, reply, TRIGGER_ALL);
  if (wordTabSize((argv = wordTab(cmd, wordsInString(cmd)))) == 1)
    return (deleteWordTab(argv));
  displayWordTab(argv);
  while (g_cmdsTab[++i].size)
    {
      if (!strcmp(argv[1], g_cmdsTab[i].command))
	ret = (g_cmdsTab[i].lg == LOG && !(irc->_clients[it].isLog))?
	  -3: g_cmdsTab[i].func(irc, it, argv);
    }
  handleRet(irc, it, ret, argv);
  deleteWordTab(argv);
  return (ret != -4 && triggerReply(irc));
}

void		commandParser(t_irc* irc,
			      int it)
{
  char		tbuf[RSIZE_XL];
  int		ret;

  bzero(tbuf, RSIZE_XL);
  ret = 1;
  while (ret)
    {
      ret = getStringFromCircularBuffer(&(irc->_clients[it].cbuf), tbuf);
      if (strlen(tbuf))
	redirectCommand(irc, it, tbuf);
    }
}

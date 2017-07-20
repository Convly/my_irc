/*
** circularBuffer.c for circularbuffer in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Tue Jun  6 19:53:51 2017 JSX
** Last update Sun Jun 11 19:22:41 2017 pazimor pazimor
*/

#include "irc.h"

int		addStringInCircularBuffer(t_cbuf* cb,
					  char* string)
{
  int		i;
  int		ret;

  i = -1;
  ret = 1;
  while (string[++i] && ret)
    ret = cbPushBack(cb, string[i]);
  return ret;
}

int		getStringFromCircularBuffer(t_cbuf* cbuf,
					    char* string)
{
  int		ret;
  int		n;
  int		i;
  char		c[1];
  int		r;
  int		out;

  r = n = ret = i = out = 0;
  while (!out && (ret = cbPopFront(cbuf, c)))
    {
      n = out = (ret && r && *c == '\n')? 1: 0;
      if (ret && *c == '\r')
	r = 1;
      else if (ret)
	r = n = 0;
      if (ret && *c != '\r')
	string[i++] = *c;
    }
  if (strlen(string) > 1 && string[strlen(string) - 1] == '\n')
    string[strlen(string) - 1] = '\0';
  if (strlen(string) > 1 && string[strlen(string) - 1] == '\r')
    string[strlen(string) - 1] = '\0';
  return ((out)? 1: 0);
}

void		cbInit(t_cbuf *cb,
		       size_t capacity)
{
  cb->sz = 1;
  if (!(cb->buffer = malloc(capacity * cb->sz)))
    xerror("Can't allocate memory");
  bzero(cb->buffer, capacity);
  cb->buffer_end = (char*)cb->buffer + capacity * cb->sz;
  cb->capacity = capacity;
  cb->count = 0;
  cb->head = cb->buffer;
  cb->tail = cb->buffer;
}

int		cbPushBack(t_cbuf *cb,
			   const char data)
{
  char		*tmp;

  tmp = cb->head;
  if (tmp == cb->buffer_end)
    tmp = cb->buffer;
  if (tmp == cb->tail && cb->count)
    {
      rerror("[SERVER ERROR]: Circular buffer self eated (push)", 1);
      return (cb->count);
    }
  *tmp = data;
  cb->head = tmp + 1;
  cb->count++;
  return (cb->count);
}

int		cbPopFront(t_cbuf* cb,
			   char* item)
{
  if (cb->count == 0 || !*(cb->tail) || !(cb->buffer))
    return (0);
  *item = *(cb->tail);
  *(cb->tail) = '\0';
  if (cb->tail == cb->buffer_end)
    cb->tail = cb->buffer;
  else
    cb->tail += 1;
  cb->count--;
  return (cb->count);
}

/*
** strwordtab.c for strtowordtab in /home/jsx/Epitech/tek2/PSU/irc/PSU_2016_myirc/IRCServer/Srcs/
**
** Made by JSX
** Login   <jean-sebastien.herbaux@epitech.eu>
**
** Started on  Wed Jun  7 22:54:51 2017 JSX
** Last update Sun Jun 11 19:53:14 2017 pazimor pazimor
*/

#include "irc.h"

int		wordsInString(char *str)
{
  int		it;
  int		k;

  k = it = 0;
  while (str[it])
    {
      while (str[it] == ' ' || str[it] == '\t' || str[it] == '\n')
	it++;
      if (!str[it])
	return (k);
      k++;
      while (str[it] && str[it] != ' ' && str[it] != '\t' &&
	     str[it] != '\n')
	it++;
    }
  return (k);
}

char		**wordTab(char *str,
			  int size)
{
  char		**wordTab;
  char		*tmp;
  int		i;

  i = 1;
  if (!(wordTab = malloc(sizeof(char*) * (size + 2))))
    xerror("can't allocate memory");
  wordTab[0] = strdup(str);
  while ((tmp = strsep(&str, " ")))
    {
      wordTab[i] = strdup(tmp);
      if (wordTab[i][0])
	i++;
    }
  wordTab[i] = NULL;
  return (wordTab);
}

int		wordTabSize(char **tab)
{
  int		s;

  s = 0;
  while (tab && tab[s] != NULL)
    s++;
  return (s);
}

int		deleteWordTab(char** tab)
{
  int		i;

  i = -1;
  while (tab[++i])
    free(tab[i]);
  free(tab);
  return (1);
}

void		displayWordTab(char** tab)
{
  int		it;

  it = -1;
  while (tab[++it])
    fprintf(stderr, "|%d\t%s\n", it, tab[it]);
}

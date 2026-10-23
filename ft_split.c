/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmongili <lmongili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 17:45:45 by lmongili          #+#    #+#             */
/*   Updated: 2026/10/23 18:26:37 by lmongili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmongili <lmongili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 17:13:56 by lmongili          #+#    #+#             */
/*   Updated: 2025/11/29 11:31:57 by lmongili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>

char	*ft_strchr(const char *s, int c)
{
	if (c == 0)
		return ((char *)(s + strlen(s)));
	if (!s)
		return (NULL);
	while (*s != 0)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

static char	**ft_free(char **spltd)
{
	int	i;

	i = -1;
	while (spltd[++i] != NULL)
		free(spltd[i]);
	free(spltd);
	return (NULL);
}

static char	*find_word(char const *s, char c)
{
	char	*word;
	size_t	i;
	size_t	j;

	i = -1;
	if (!*s)
		return (NULL);
	if (*s == c)
		return (find_word(s + 1, c));
	while (++i < strlen((const char *)s))
	{
		if (((char *)s)[i] == c)
			break ;
	}
	j = i;
	word = calloc((i + 1), sizeof(char));
	i = -1;
	if (!word)
		return (NULL);
	while (++i < j)
		word[i] = ((char *)s)[i];
	return (word);
}

static	size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	w;
	char	*sub;

	i = -1;
	if (ft_strchr(s, c) == NULL && *s)
		return (1);
	else if (ft_strchr(s, c) == NULL && !*s)
		return (0);
	w = 0;
	while (s[++i] && i < strlen((char *)s))
	{
		if (s[i] != c && i == 0)
			w++;
		else if (s[i] == c)
		{
			sub = ft_strchr((const char *)&s[i], c) + 1;
			if (*sub && *sub != c)
				w++;
		}
	}
	return (w);
}

char	**ft_split(char const *s, char c)
{
	char	**spltd;
	size_t	i;
	size_t	j;

	spltd = calloc((count_words(s, c) + 1), sizeof(char *));
	if (!spltd)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count_words(s, c))
	{
		while (s[j] == c)
			j++;
		spltd[i] = find_word(&s[j], c);
		if (!spltd[i])
			return (ft_free(spltd));
		j += strlen(spltd[i]);
		i++;
	}
	return (spltd);
}

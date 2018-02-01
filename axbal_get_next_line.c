/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axbal <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 12:24:58 by axbal             #+#    #+#             */
/*   Updated: 2018/02/01 17:39:01 by lcabanes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <unistd.h> // TEST

int		find_line(char *buf, int ret)
{
	int		i;

	i = 0;
	if (buf == NULL)
		return (0);
	while (i < ret && buf[i]) // Pas besoin de verifier '&& buf[i]' (cf 2nd 'if')
	{
		if (buf[i] == '\n')
			return (1);
		if (buf[i] == '\0')
			return (2);
		i++;
	}
	return (0);
}

char	*fill_line(char *buf, char **line)
{
	int		i;
	char	*tmp;
	int		original_buf_length; //TEST

	original_buf_length = ft_strlen(buf); //TEST
	i = 0;
	if (!(buf)) // Condition toujours verifiee depuis 'if (save && [...])' et 'buf[ret] = '\0'' dans 'get_next_line'
	{
		*line = "";
		return (NULL);
	}
	while (buf[i] != '\0' && buf[i] != '\n')
		i++;
	buf[i] = '\0';
	*line = ft_strdup(buf);
	buf[i] = ' '; // Je crois pas qu'il faille s'inquieter  pour free
//	if (buf + i + 1) // Do you mean 'if (*(buf + i + 1)' ? Dangereux : on ne sait pas ce qu'il y a en memoire a cet endroit la dans le cas ou read n'aurait pas remplit ce champ. En l'absence de '\n' dans la ligne a relever, *(buf + i) vaut le 'buf[ret]' = '\0'' de 'get_next_line', et on ne connait pas *(buf + i + 1) (depend de la gestion de la memoire par l'ordinateur)
	if (1)
	{
		tmp = ft_strdup(buf + i + 1);
		free(buf);
		buf = ft_strdup(tmp);
		free(tmp);
	}
	else // On rentre jamais ici du coup
	{ //TEST
		free(buf); // Puis 'buf = NULL'
		buf = NULL; //TEST
	} //TEST
	return (buf == NULL ? NULL : buf);
}

char	*concat_buf(char *save, char *buf)
{
	char	*tmp;
	int		size;

	tmp = NULL;
	if (save != NULL)
	{
		tmp = ft_strdup(save);
		free(save);
		size = ft_strlen(tmp) + ft_strlen(buf) + 1;
		if (!(save = malloc(sizeof(char) * size)))
			return (NULL);
		save = ft_strcpy(save, tmp);
		save = ft_strcat(save, buf);
		free(tmp);
	}
	else
	{
		size = ft_strlen(buf) + 1; // Bah du coup tu t'en sers pas, de ca
		save = ft_strdup(buf);
	}
	return (save);
}

int		norm(int ret, int *stop, char *save, int mode)
{
	int		i;

	if (mode == 1)
	{
		if (ret == -1)
			return (-1);
		else
			*stop = 3;
		return (0);
	}
	if (mode == 2)
	{
		i = 0;
		while (save[i] != '\0') // En y reflechissant, c'est pas une boucle ca. Surtout en jetant un oeil aux conditions pour entrer dans la boucle.
			save[i++] = '\0';
	}
	return (0);
}

int		get_next_line(const int fd, char **line)
{
	int				stop;
	int				ret;
	char			buf[BUFF_SIZE + 1];
	static char		*save;

	stop = 0;
	if ((fd < 3 && fd != 0) || BUFF_SIZE <= 0 || line == NULL) // fd < 3 => fd != 0
		return (-1);
	if (save && find_line(save, ft_strlen(save)) == 1)
		stop = 1;
	while (stop == 0)
	{
		if ((ret = read(fd, buf, BUFF_SIZE)) > 0)
		{
			buf[ret] = '\0';
			stop = find_line(buf, ret);
			save = concat_buf(save, buf);
		}
		else if (norm(ret, &stop, NULL, 1) == -1)
			return (-1);
	}
	save = fill_line(save, line);
//	write(1, *line, ft_strlen(*line)); // TEST
//	write(1, "\n", 1); // TEST
	if (stop == 3 && ft_strlen(*line) == 0)
		return (norm(0, 0, save, 2));
	return (1);
}

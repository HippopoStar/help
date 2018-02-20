/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlevaufr <tlevaufr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/22 15:13:06 by tlevaufr          #+#    #+#             */
/*   Updated: 2018/02/19 22:18:36 by tlevaufr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

//Bon, j'ai tout lu, j'admet que ca me depasse parfois (parfois completement, meme),
//et que par rapport au choix de l'iteratif tu t'en sors admirablement.
//Meme si pour ma part je reste convaicu que c'est l'idee la plus etrange qu'on ait eu
//depuis l'invention du curling.
//Et par endroits le choix du nom de tes variables frole le cyber-fascisme.
//Ou en tout cas c'est pas tres gluten-free.

void		lst_del(const int fd, t_gnl_lst **list)
{
	t_gnl_lst *ptr;
	t_gnl_lst *ptr2;

	ptr = *list;
	if (ptr->fd == fd)
	{
		if (ptr->next)
		{
			*list = (ptr->next);
			free(ptr);
		}
		else
		{
			free(*list);
			*list = NULL;
		}
		return ;
	}
	while (ptr->next && ptr->next->fd != fd)
		ptr = ptr->next;
	ptr2 = (ptr->next && ptr->next->next) ? ptr->next->next : NULL;
	free(ptr->next);
	ptr->next = ptr2 ? ptr2 : NULL;
}

char		**concat_line(t_gnl_lst *file, char **line_adress)	//Oui bah va comment tu t'en
									//sers c'etait bien la peine
									//qu'il renvoie quelque chose.
									//(le type 'void' c'est pour
									//les faibles et les opprimes)
									//(C'est d'ailleurs a ca qu'on
									//les reconnait)
{
	char	*new_line;
	size_t	t;
	ssize_t	chars;
	char	*tmp;

	chars = 0;
	while (file->buf[chars + file->bytes_read] != '\n' && file->buf[chars +\
			file->bytes_read] && (chars + file->bytes_read) < file->read_value)
	//La deuxieme verification est suspecte (pour ne pas dire coupable, je respecte la
	//presomption d'innocence), et dans la troisieme faut peut-etre ajouter un '=' apres '<'
		chars++;
	t = 0;
	if (!(new_line = ft_strnew(chars)))
		return (NULL);
	while (chars-- > 0)
		new_line[t++] = file->buf[file->bytes_read++];
	new_line[t] = '\0';
	if (*line_adress)
	{
		tmp = *line_adress;
		*line_adress = ft_strjoin((*line_adress), new_line);
		ft_strdel(&tmp);
		ft_strdel(&new_line);
		return (line_adress);
	}
	*line_adress = new_line;
	return (line_adress);
}

int			make_line(t_gnl_lst *file, char **line_adress)	//Avant ca s'appelait 'list',
									//ou encore 'ptr',
									//maintenant ca s'appelle
									//'file'.
									//Qui peut predire comment
									//les maillons de cette liste
									//s'appeleront demain ?
{
	int		bypass_read;
	int		job_done;

	job_done = 0;
	bypass_read = file->bytes_read < file->read_value && file->bytes_read > 0;
	while (bypass_read ||\
				(file->read_value = read(file->fd, file->buf, BUFF_SIZE)))
	{
	/* --*/	job_done = 1;
	/* | */	if (file->read_value == -1)	//Ca y'est t'en as eu marre tu renvoies plus jamais 0.
	/* | */					//Je comprend : tu fais ca parce que t'es en colere.
	/* | */					//	if (job_done == 0 && [...])
	/* | */					//		return ([...]);
	/* | */					//(C'est une idee, j'en sais trop rien, tu commences
	/* | */					//a saouler tout le monde avec tes boucles while
	/* | */					//et tes invariants de boucles tout petes auxquels
	/* | */					//personne ne comprend quoi que ce soit)
	/* | */		return (-1);
	/* ->*/	file->bytes_read = bypass_read ? file->bytes_read : 0;
		concat_line(file, line_adress);
		bypass_read = file->bytes_read < BUFF_SIZE && file->bytes_read < file->read_value;
		//Ca y'est, 'bypass_read' depend plus de la meme condition.
		//Ca devait arriver.
		//POURQUOI !? POURQUOI ETRE AUSSI CORROMPU !?
		//Soit j'ai encore rien capte, soit dans l'idee il devait au moins y avoir un '||'
		//a la place du '&&' pour que ca paraisse ressembler a quelque chose d'a peu pres
		//plus ou moins coherent de loin, de dos, et dans le brouillard.
		if (file->read_value < BUFF_SIZE && file->bytes_read == file->read_value)
			return (1);	//Si il se remet a renvoyer 1 au bout d'un moment, ca vient
					//peut-etre de la.
		if (file->buf[file->bytes_read] == '\n')
		{
			file->bytes_read++;
			return (1);
		}
	}
	return (job_done);
}

t_gnl_lst	*new_or_find(const int fd, t_gnl_lst **list, int unfound)
{
	t_gnl_lst	*ptr;

	if (!(*list) || unfound == 1)
	{
		if (!(ptr = malloc(sizeof(t_gnl_lst))))
			return (NULL);
		ptr->fd = fd;
		ptr->read_value = BUFF_SIZE;
		ptr->bytes_read = BUFF_SIZE;
		ft_bzero(ptr->buf, BUFF_SIZE);
		ptr->next = unfound ? *list : NULL;	//Tu peux pas construire tes listes chainees
							//dans le sens des fleches, non ?
							//Comme tout le monde ?
							//C'est criminel de faire ca.
							//CRI-MI-NEL
		*list = ptr;
		return (*list);
	}
	ptr = *list;
	while (ptr->next && ptr->fd != fd)
		ptr = ptr->next;
	if (fd == ptr->fd)
		return (ptr);
	return (new_or_find(fd, list, 1));
}

int			get_next_line(const int fd, char **line)
{
	static t_gnl_lst	*list = NULL;
	int					status;

	if (!line)
		return (-1);
	*line = NULL;
	status = make_line(new_or_find(fd, &list, 0), line);
	if (status == 0)
		lst_del(fd, &list);
	return (status);
}

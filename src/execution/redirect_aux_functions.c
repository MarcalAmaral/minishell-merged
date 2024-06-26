/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 19:30:55 by marvin            #+#    #+#             */
/*   Updated: 2024/05/27 19:30:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_append(char **mat_append, char *ref)
{
	int	i;

	i = 0;
	while (mat_append[i])
	{
		if (ft_strcmp(mat_append[i], ref) == 0)
		{
			free(ref);
			return (1);
		}	
		i++;
	}
	free(ref);
	return (0);
}

char	**creat_append_mat(t_dlist *aux_t, int size_append)
{
	int		i;
	char	**appends;
	int		i_append;

	i_append = 0;
	i = 0;
	appends = ft_calloc(sizeof(char *), (size_append + 1));
	while (aux_t->next != NULL)
	{
		if (aux_t->tok->type == R_OUT || aux_t->tok->type == APPEND)
			i++;
		if (aux_t->tok->type == APPEND)
		{
			appends[i_append] = ft_itoa(i - 1);
			i_append++;
		}
		aux_t = aux_t->next;
	}
	return (appends);
}

char	**have_append(t_dlist *tokens)
{
	int		i;
	t_dlist	*aux_t;
	char	**appends;

	i = 0;
	aux_t = tokens;
	appends = NULL;
	while (aux_t->next != NULL)
		aux_t = aux_t->next;
	while (aux_t->tok->type != PIPE && aux_t->prev != NULL)
	{
		if (aux_t->tok->type == APPEND)
			i++;
		aux_t = aux_t->prev;
	}
	if (i > 0)
		appends = creat_append_mat(aux_t, i);
	return (appends);
}

int	files_out_control(t_ast *raiz, t_pipex *p)
{
	int	i;

	i = 0;
	while (raiz->files[1][i] != NULL)
	{
		if (raiz->files[2] != NULL)
		{
			if (check_append(raiz->files[2], ft_itoa(i)) > 0)
			{
				raiz->r_fds.r_fd_out = open(raiz->files[1][i],
						O_WRONLY | O_CREAT | O_APPEND, 0000666);
			}
			else
				raiz->r_fds.r_fd_out = open(raiz->files[1][i],
						O_WRONLY | O_CREAT | O_TRUNC, 0000666);
		}
		else
		{
			raiz->r_fds.r_fd_out = open(raiz->files[1][i],
					O_WRONLY | O_CREAT | O_TRUNC, 0000666);
		}
		redirect_out_error(raiz, p);
		i++;
	}
	return (raiz->r_fds.r_fd_out);
}

int	files_in_control(t_ast *raiz, t_pipex *p)
{
	int	i;

	i = 0;
	redirect_in_error(raiz, p);
	while (raiz->files[0][i] != NULL)
	{
		raiz->r_fds.r_fd_in = open(raiz->files[0][i], O_RDONLY);
		i++;
	}
	return (raiz->r_fds.r_fd_in);
}

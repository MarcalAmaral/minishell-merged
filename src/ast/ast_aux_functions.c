/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_aux_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parthur- <parthur-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 23:24:33 by parthur-          #+#    #+#             */
/*   Updated: 2024/05/17 01:49:27 by parthur-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_command_organizer(t_ast *raiz, t_pipex *p)
{
	p->fd_exec[1] = p->pipe_fd[1];
	p->fd_exec[0] = 0;
	exec_cmd(raiz->esq, p);
	closing_process(p, raiz);
}

void	standard_command_organizer(t_ast *raiz, t_pipex *p)
{
	if (raiz->dir->index != 3)
		p->fd_exec[1] = p->exit_fd;
	p->fd_exec[0] = p->input_fd;
	exec_cmd(raiz->dir, p);
	if (raiz->dir->index == 3)
		close(p->pipe_fd[1]);
}

t_dlist	*free_chunk_list(t_dlist *tokens)
{
	while (tokens->next != NULL)
		tokens = tokens->next;
	while (tokens->tok->type != PIPE && tokens->prev)
	{
		tokens = tokens->prev;
		free_tokens(tokens->next);
	}
	if (tokens->prev)
	{
		tokens = tokens->prev;
		free_tokens(tokens->next);
		tokens->next = NULL;
	}
	else
	{
		free_tokens(tokens);
		tokens = NULL;
	}
	return (tokens);
}

t_ast	*cria_no_arv(t_dlist *tokens, t_pipex *p, int i, int t)
{
	t_ast	*no_pipe;

	no_pipe = (t_ast *)malloc(sizeof(t_ast));
	no_pipe->type = PIPE;
	no_pipe->esq = NULL;
	no_pipe->dir = cria_no_cmd(tokens, p, i, t);
	no_pipe->path = NULL;
	no_pipe->cmd = NULL;
	no_pipe->index = 0;
	return (no_pipe);
}

t_ast	*adiciona_no(t_ast *raiz, t_ast *no)
{
	if (raiz == NULL)
		return (no);
	raiz->esq = adiciona_no(raiz->esq, no);
	return (raiz);
}

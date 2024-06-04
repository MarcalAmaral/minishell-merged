/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_aux_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 23:24:33 by parthur-          #+#    #+#             */
/*   Updated: 2024/06/04 15:18:11 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	closing_father(t_pipex *p, t_ast *raiz)
{
	ft_free_ast(raiz);
	ft_free_matrix_char(p->paths.mat_path);
	free(p);
}

void	closing_only_child(t_pipex *p, t_ast *raiz, t_dlist **tokens, int *fd)
{
	dup2(fd[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	ft_free_matrix_char(p->paths.mat_path);
	free(p);
	ft_free_ast(raiz);
	free_chunk_list(*tokens);
	free(tokens);
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

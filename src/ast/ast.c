/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 18:00:50 by parthur-          #+#    #+#             */
/*   Updated: 2024/05/23 17:37:29 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*cria_arvore(t_dlist **t, t_pipex *p)
{
	t_ast	*raiz;
	t_ast	*esq;
	t_dlist	*aux;
	int		i;

	i = t[0]->pipes;
	aux = t[0];
	raiz = NULL;
	while (i >= 0)
	{
		if (i > 0)
		{
			esq = cria_no_arv(aux, p, i, t[0]->pipes);
			aux = free_chunk_list(t[0]);
			raiz = adiciona_no(raiz, esq);
		}
		else
		{
			esq = cria_no_cmd(aux, p, i, t[0]->pipes);
			aux = free_chunk_list(t[0]);
			raiz = adiciona_no(raiz, esq);
		}
		i--;
	}
	return (raiz);
}

void	exec_cmd(t_ast *raiz, t_pipex *p)
{
	int		f_id;

	f_id = fork();
	if (f_id == 0)
	{
		if (raiz->index != 3)
			dup2(p->fd_exec[1], 1);
		if (raiz->index != 1)
			dup2(p->fd_exec[0], 0);
		close_fds(p->pipe_fd[1]);
		if (execve(raiz->path, raiz->cmd, hook_environ(NULL, 0)) == -1)
			exit(last_exit_status(-1));
	}
	if (raiz->index != 1)
		close(p->fd_exec[0]);
	if (raiz->index != 3)
		close(p->fd_exec[1]);
}

void	tree_exec(t_ast *raiz, t_pipex *p, int fd)
{
	p->exit_fd = fd;
	if (pipe(p->pipe_fd) == -1)
		return ;
	p->input_fd = p->pipe_fd[0];
	if (raiz->esq->type == PIPE)
	{
		p->f_id = fork();
		if (p->f_id == 0)
		{
			fd = p->pipe_fd[1];
			raiz->esq->first = raiz->first;
			tree_exec(raiz->esq, p, fd);
			closing_process(p, raiz);
		}
		waitpid(p->f_id, NULL, 0);
	}
	else
	{
		p->f_id = fork();
		if (p->f_id == 0)
			first_command_organizer(raiz, p);
		waitpid(p->f_id, NULL, 0);
	}
	standard_command_organizer(raiz, p);
}

void	closing_father(t_pipex *p, t_ast *raiz)
{
	ft_free_ast(raiz);
	ft_free_matrix_char(p->paths.mat_path);
	free(p);
}

void	closing_only_child(t_pipex *p, t_ast *raiz, t_dlist *tokens)
{
	ft_free_matrix_char(p->paths.mat_path);
	free(p);
	ft_free_ast(raiz);
	free_chunk_list(tokens);
}

void	ast_function(t_dlist **tokens)
{
	t_ast	*raiz;
	t_pipex	*p;

	tokens[0]->pipes = have_pipe(tokens[0]);
	p = (t_pipex *)malloc(sizeof(t_pipex));
	expansion(tokens);
	quote_removal(tokens);
	get_paths(p);
	if (tokens[0]->pipes > 0)
	{
		raiz = cria_arvore(tokens, p);
		free(tokens);
		raiz->first = raiz;
		tree_exec(raiz, p, STDOUT_FILENO);
		wait(NULL);
		closing_father(p, raiz);
	}
	else
	{
		raiz = cria_no_cmd(tokens[0], p, 0, 0);
		p->f_id = fork();
		if (p->f_id == 0)
		{
			if (execve(raiz->path, raiz->cmd, hook_environ(NULL, 0)) == -1)
				exit(last_exit_status(-1));
		}
		waitpid(-1, NULL, 0);
		closing_only_child(p, raiz, tokens[0]);
		free(tokens);
	}
}

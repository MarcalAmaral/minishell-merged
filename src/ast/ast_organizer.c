/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_organizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 19:28:57 by parthur-          #+#    #+#             */
/*   Updated: 2024/05/31 09:41:25 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	first_command_organizer(t_ast *root, t_pipex *p)
{
	p->fd_exec[1] = p->pipe_fd[1];
	p->fd_exec[0] = 0;
	exec_cmd(root->esq, p);
	closing_process(p, root);
}

void	standard_command_organizer(t_ast *root, t_pipex *p)
{
	if (root->dir->index != 3)
		p->fd_exec[1] = p->exit_fd;
	p->fd_exec[0] = p->input_fd;
	exec_cmd(root->dir, p);
	if (root->dir->index == 3)
		close(p->pipe_fd[1]);
}

void	brothers_functions(t_dlist **tokens, t_pipex *p)
{
	t_ast	*root;

	root = cria_arvore(tokens, p);
	free(tokens);
	root->first = root;
	tree_exec(root, p, STDOUT_FILENO);
	wait(NULL);
	closing_father(p, root);
}
// Função para dar free na estrutura do pipex;
// Enquadrar a execução de builtins;
void	only_child_functions(t_dlist **tokens, t_pipex *p)
{
	t_ast	*root;
	int		exit_status;

	root = cria_no_cmd(tokens[0], p, 0, 0);
	exit_status = command_not_found(root->path, root->cmd);
	if (!exit_status)
	{
		p->f_id = fork();
		if (p->f_id == 0)
		{
			root->r_fds = r_fds_control(root, p);
			if (root->r_fds.r_fd_out != 0)
				dup2(p->fd_exec[1], 1);
			if (root->r_fds.r_fd_in != 0)
				dup2(p->fd_exec[0], 0);
			close_fds(1024);
			if (execve(root->path, root->cmd, hook_environ(NULL, 0)) == -1)
			{
				hook_environ(NULL, 1);
				hook_pwd(NULL, 1);
				ft_free_ast(root);
				exit(last_exit_status(-1));
			}
		}
		waitpid(p->f_id, &exit_status, 0);
	}
	closing_only_child(p, root, tokens[0]);
	free(tokens);
}

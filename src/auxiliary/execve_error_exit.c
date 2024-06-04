/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_error_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 22:21:20 by myokogaw          #+#    #+#             */
/*   Updated: 2024/06/03 22:21:20 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_error_exit(t_ast *root)
{
	hook_environ(NULL, 1);
	hook_pwd(NULL, 1);
	ft_free_ast(root);
	exit(last_exit_status(-1));
	return ;
}
